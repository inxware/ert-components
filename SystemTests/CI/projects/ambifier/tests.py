from hashlib import sha256
import datetime
import os,signal,sys
import requests
import time
import subprocess
import threading
import re
import json
from xml.sax.saxutils import escape

#websocket imports
import asyncio
import websockets

#inx imports
import devman
import microtik
import testTail

TEST_RESULTS_DIR_NAME='test-results'

def deviceGetSessionId(devmanApi,deviceId):
	ip=devmanApi.getDeviceIP(deviceId)
	#check we accept good session start request
	url='http://'+ip+'/post_session_start.cgi'
	certificate=devmanApi.getProvisioningCertificate(deviceId)
	postdata='certificate='+certificate
	response = requests.post(url,data=postdata)
	data=json.loads(response.text)
	return data['sessionId']

def hashSessionId(sessionId):
	tohash="privatekey"+str(sessionId)
	return sha256(tohash.encode('utf-8')).hexdigest()

async def websocket(uri,postdata,doSend,ignoreRegexes):
	keepReceiving=True
	uri = "ws://"+uri
	response = ""
	data='method=POST'
	for key in postdata:
		if len(data)>0:
			data=data+'&'
		data=data+key+'='+str(postdata[key])
	async with websockets.connect(uri) as websocket:
		if doSend:
			await websocket.send(data)
			print("> "+str(data))
		while keepReceiving:
			response = await websocket.recv()
			print("< "+str(response))
			#check to see if this should be ignored
			keepReceiving=False
			for ignoreRegex in ignoreRegexes:
				print("Testing "+str(response)+" against "+str(ignoreRegex))
				if re.search(ignoreRegex,str(response)) != None:
					print("ignoring")
					keepReceiving=True
	return response

def pretty_print_POST(req):
	"""
	At this point it is completely built and ready
	to be fired; it is "prepared".

	However pay attention at the formatting used in 
	this function because it is programmed to be pretty 
	printed and may differ from the actual request.
	"""
	print('{}\n{}\r\n{}\r\n\r\n{}'.format(
		'-----------START-----------',
		req.method + ' ' + req.url,
		'\r\n'.join('{}: {}'.format(k, v) for k, v in req.headers.items()),
		req.body,
	))

class TestCase:
	classname = ''
	name = ''
	time = ''
	systemout = ''
	systemerr = ''

class TestSuite:
	errors = 0
	failures = 0
	name = ''
	tests = 0
	testCases = []

	def addTestCase(self,testCase):
		if len(testCase.systemerr)>0:
			self.failures = self.failures + 1
			self.errors = self.errors + 1
		self.testCases.append(testCase)
		self.tests = len(self.testCases)

def writeTestCase(f,testCase):
	f.write('<testcase classname="'+testCase.classname+'" name="'+testCase.name+'" time="'+testCase.time+'">')
	f.write('<system-out>'+escape(testCase.systemout)+'</system-out>')
	f.write('<system-err>'+escape(testCase.systemerr)+'</system-err>')
	if len(testCase.systemerr)>0:
		f.write('<failure message="'+escape(testCase.systemerr)+'" type="WARNING">'+escape(testCase.systemerr)+'</failure>')
	f.write('</testcase>')
	
def writeTestSuite(f,testSuite):
	f.write('<testsuite errors="'+str(testSuite.errors)+'" failures="'+str(testSuite.failures)+'" name="'+testSuite.name+'" tests="'+str(testSuite.tests)+'">')
	for testCase in testSuite.testCases:
		writeTestCase(f,testCase)
	f.write('</testsuite>')

def writeJunitFile(testName,testSuites):
	if not os.path.exists(TEST_RESULTS_DIR_NAME):
		print("creating test results directory "+TEST_RESULTS_DIR_NAME)
		os.makedirs(TEST_RESULTS_DIR_NAME)

	fileName=TEST_RESULTS_DIR_NAME+'/TEST-'+testName+'.xml'
	print("writing test result file "+fileName)
	with open(fileName, 'w') as f:
		f.write('<?xml version="1.0" ?>')
		f.write('<testsuites>')
		for testSuite in testSuites:
			writeTestSuite(f,testSuite)
		f.write('</testsuites>')

def runTestNetworkStress(devmanApi,deviceId):
	testCase = TestCase()
	testCase.classname='inx.heatrod.tests.networkstress'
	testCase.name=deviceId

	try:
		deviceIp=devmanApi.getDeviceIP(deviceId)
		url = 'http://'+deviceIp+'/someurl'
		for i in range(0,100):
			#request a page
			try:
				response=requests.post(url,data={'somedata':'somedata'},timeout=10)
			except:
				print('attempt '+str(i))
		#check if device is still online
		time.sleep(60)
		online=devmanApi.getDeviceOnline(deviceId)
		if online:
			testCase.systemout = 'Device stayed online'
		else:
			raise Exception('Device went offline')
	except Exception as e:
		testCase.systemerr = str(e)
	return testCase

def runTestOnlineState(devmanApi,deviceId):
	testCase=TestCase()
	testCase.classname='inx.heatrod.tests.online'
	testCase.name=deviceId
	try:
		deviceStatus = devmanApi.getDeviceStatus(deviceId)
		#turn pulse_date in to a real date
		pulseDate=deviceStatus['device']['pulse_date']
		deviceDateTime = datetime.datetime.strptime(pulseDate, "%Y-%m-%d %H:%M:%S")
		deviceDateTimeDelta = datetime.datetime.now() - deviceDateTime
		if deviceDateTimeDelta > datetime.timedelta(minutes=1):
			testCase.systemerr = 'Device offline for longer than 1 minute. '+str(deviceDateTimeDelta)
		else:
			testCase.systemout = str(deviceDateTimeDelta)
	except Exception as e:
		testCase.systemerr = str(e)
	return testCase

def runTestOTA(devmanApi,deviceId):
	testCase=TestCase()
	testCase.classname='inx.heatrod.tests.ota'
	testCase.name=deviceId
	try:
		deviceStatus = devmanApi.getDeviceStatus(deviceId)
		#get the latest firmware version for this hardware variant
		firmwares=devmanApi.getAvailableFirmwares(deviceId)
		#if the current version for this device matches the latest firmware then choose the previous firmware version
		#look at the current version
		versionToFlash=firmwares[0]
		if deviceStatus['device']['ehs_version']==versionToFlash and len(firmwares)>1:
			versionToFlash=firmwares[1]
		print(versionToFlash)
		devmanApi.startOTA(deviceId,versionToFlash)
		#wait for it to finish
		count=0
		while deviceStatus['device']['ehs_version']!=versionToFlash and count<30:
			time.sleep(60)
			deviceStatus = devmanApi.getDeviceStatus(deviceId)
			count = count + 1
		if deviceStatus['device']['ehs_version'] == versionToFlash:
			testCase.systemout=deviceStatus['device']['ehs_version']
		else:
			testCase.systemerr = 'Device did not update'
	except Exception as e:
		testCase.systemerr = str(e)
		print(testCase.systemerr)
	return testCase

def runWebSocket(devmanApi,deviceId):
	testCase=TestCase()
	testCase.classname='inx.heatrod.tests.websocket'
	testCase.name=deviceId
	deviceIp=devmanApi.getDeviceIP(deviceId)
	#insert code here
	return testCase

def runTestProvision(devmanApi,deviceId):
	testCase = TestCase()
	testCase.classname='inx.heatrod.tests.provision'
	testCase.name=deviceId
	loop = asyncio.get_event_loop()

	#tell the device to factory reset
	devmanApi.rebootDevice(deviceId,factoryReset=1)
	time.sleep(2*60)

	#post_mobile_provision.cgi certificate
	ip=devmanApi.getDeviceIP(deviceId)
	#check we accept good session start request
	url='http://'+ip+'/post_session_start.cgi'
	certificate=devmanApi.getProvisioningCertificate(deviceId)
	postdata='certificate='+certificate
	try:
		response = requests.post(url,data=postdata)
		print(response.text)
	except:
		raise Exception("Could not perform good session start request.")
	try:
		data=json.loads(response.text)
	except:
		raise Exception("Response could not be parsed to json")
	if 'sessionId' in data:
		testCase.systemout += "Received sessionId="+str(data['sessionId'])+'. '
	else:
		raise Exception("Bad response to good session start request:"+response.text)

	#now we have a session try to provision
	deviceStatus=devmanApi.getDeviceStatus(deviceId)
	ip=devmanApi.getDeviceIP(deviceId)
	sessionId=data['sessionId']+1
	postdata={}
	postdata['sessionId']=hashSessionId(sessionId)
	postdata['certificate']=certificate
	postdata['pin']=deviceStatus['debugger']['provisioning']['pin']
	postdata['password']=deviceStatus['debugger']['provisioning']['password']
	postdata['url']='post_mobile_provision'
	url=ip+'/ws'
	response=''
	ignoreRegexes=[r'timeStamp']
	try:
		response=loop.run_until_complete(websocket(url,postdata,True,ignoreRegexes))
		print(response)
		response=json.loads(response)
	except Exception as e:
		print('Failed to talk to device for bad PIN test. '+str(e))
	if 'data' in response and response['data']['success']==1:
		testCase.systemout += 'Correct response to valid provision.'
	else:
		raise Exception("Bad response to provision "+str(response))
	sessionId=sessionId+1

	#now set the name and location
	time.sleep(0.25*60)
	postdata={}
	postdata['sessionId']=hashSessionId(sessionId)
	postdata['url']='post_mobile_provisioning_information'
	postdata['location']='Location '+deviceId
	postdata['localName']='Name '+deviceId
	postdata['localUIMode']='mobileOnly'
	try:
		response=loop.run_until_complete(websocket(url,postdata,True,ignoreRegexes))
		print(response)
		response=json.loads(response)
	except Exception as e:
		print('Failed to talk to device for bad PIN test. '+str(e))
	if 'data' in response and response['data']['success']==1:
		testCase.systemout += 'Correct response to valid provision.'
	else:
		raise Exception("Bad response to provision "+str(response))
	sessionId=sessionId+1
	
	time.sleep(0.25*60)
	#check that the webui now returns the dashboard
	url='http://'+ip+'/mobile.html'
	response = requests.get(url)
	if response.text.find('welcome')>-1:
		testCase.systemout = 'Ok'
	else:
		testCase.systemerr='Failed to get dashboard '+response.text
	return testCase

def runTestReconnection(devmanApi,router,deviceId,offlineTimeMinutes=2,testFileDirPath='/home/jenkins/iotlogging'):
	testCase = TestCase()
	testCase.classname='inx.heatrod.tests.reconnection.minutes'+str(offlineTimeMinutes)
	testCase.name=deviceId

	#find our log file so that we
	logFilePath=testTail.findLogFile(testFileDirPath,deviceId)
	if logFilePath=='':
		testCase.systemerr = 'Could not find log file in '+testFileDirPath
		return testCase

	#check online
	if devmanApi.getDeviceOnline(deviceId):
		#start reading the log file
		passRegexes=[re.compile('.*gNetworkState.*')]
		errorRegexes=[re.compile('.*boot.*')]
		passes=[]
		errors=[]
		pids=[]
		t=threading.Thread(target=testTail.threadFunction,args=(logFilePath,pids,passRegexes,errorRegexes,passes,errors))
		t.start()

		#block them from devman
		deviceIp = devmanApi.getDeviceIP(deviceId)
		router.addIPToList(deviceIp,'devices')
		print('blocked '+deviceId+' '+deviceIp+' waiting '+str(offlineTimeMinutes)+' minutes')
		#wait 2 minutes then check offline
		time.sleep(offlineTimeMinutes*60)
		if devmanApi.getDeviceOnline(deviceId):
			testCase.systemerr = 'Device did not go offline.'
		else:
			#unblock them
			router.removeIPFromList(deviceIp,'devices')
			print('unblocked '+deviceId+' '+deviceIp+' waiting for two minutes for device to come back online')
			#wait two minutes then check online
			time.sleep(120)
			if devmanApi.getDeviceOnline(deviceId):
				#now check the device did not reboot at any point
				if len(errors)==0:
					if len(passes)>=0:
						testCase.systemout = 'Ok'
					else:
						testCase.systemerr = 'Device did not log any passes.'
				else:
					testCase.systemerr = 'Device rebooted according to '+logFilePath
			else:
				testCase.systemerr = 'Device did not come back online.'

		router.removeIPFromList(deviceIp,'devices')
		for pid in pids:
			os.kill(pid,signal.SIGTERM)
		t.join(timeout=10.0)
	else:
		testCase.systemerr = 'Device not online to start with.'

	return testCase

def runTestDutyCycleTimings(devmanApi,deviceId):
	testCase = TestCase()
	testCase.classname='inx.heatrod.tests.dutycycletimings'
	testCase.name=deviceId

	#use local webui to set EMR and duty cycle 30
	ip=devmanApi.getDeviceIP(deviceId)
	url='http://'+ip+'/post_hardware.cgi'
	try:
		try:
			response = requests.post(url,data={'controllerContactor':0,'emrMode':30})
		except:
			print('')
		time.sleep(60)
		#check with devman that our update frequency is within bounds
		seconds=devmanApi.getTimeBetweenMeasurementsSeconds(deviceId)
		if seconds<8 or seconds>12:
			raise Exception('timeBetweenMeasurements out of bounds '+str(seconds))
		#also check that the last set of measurements is within 20 seconds
		lastMeasurementTimeUnixSeconds=devmanApi.getLastMeasurementTimeUnixSeconds(deviceId)
		diff = time.time()-lastMeasurementTimeUnixSeconds
		if diff > 20:
			raise Exception('lastMeasurementsDate too old '+str(diff))
		
		#set SSR with duty cycle 1s
		try:
			response = requests.post(url,data={'controllerContactor':1,'ssrMode':10})
		except:
			print('')
		#sleep for a minute
		time.sleep(60)
		#check with devman that our update frequency is within bounds
		seconds=devmanApi.getTimeBetweenMeasurementsSeconds(deviceId)
		if seconds<8 or seconds>12:
			raise Exception('timeBetweenMeasurements out of bounds '+str(seconds))
		#also check that the last set of measurements is within 20 seconds
		lastMeasurementTimeUnixSeconds=devmanApi.getLastMeasurementTimeUnixSeconds(deviceId)
		diff = time.time()-lastMeasurementTimeUnixSeconds
		if diff > 20:
			raise Exception('lastMeasurementsDate too old '+str(diff))
		testCase.systemout='lastMeasurementTime='+str(lastMeasurementTimeUnixSeconds)+' timeBetweenMeasurements='+str(seconds)
	except Exception as e:
		testCase.systemerr = str(e)
	#put the original duty cycle mode back
	try:
		response = requests.post(url,data={'controllerContactor':0,'emrMode':30})
	except:
		print('')
	return testCase

def runTestSchedule(devmanApi,deviceId):
	testCase = TestCase()
	testCase.classname='inx.heatrod.tests.schedule'
	testCase.name=deviceId
	#test controllers 1 to 3
	#for each controller check the scheduleMode and check that the current temperature matches the expected temperature
	try:
		deviceStatus=devmanApi.getDeviceStatus(deviceId)
		for i in range(1,4):
			scheduleMode=int(deviceStatus['controller']['scheduleMode'+str(i)])
			manualSetPoint=float(deviceStatus['controller']['manualSetPoint'+str(i)])
			currentSetPoint=float(deviceStatus['controller']['currTempSetPoint'+str(i)])
			scheduldeSetPoint=float(deviceStatus['controller']['schedule'+str(i)+'CurrentTemperature'])
			#test disabled
			if scheduleMode==0 and float(0)!=currentSetPoint:
				raise Exception('Schedule disabled currTempSetPoint'+str(i)+' '+str(currentSetPoint)+' does not match 0')
				#test manual mode
			elif scheduleMode==1 and manualSetPoint!=currentSetPoint:
				raise Exception('currTempSetPoint'+str(i)+' '+str(currentSetPoint)+' does not match manualSetPoint'+str(i)+' '+str(manualSetPoint))
				#test schedule mode
			elif scheduleMode==2 and scheduldeSetPoint!=currentSetPoint:
				raise Exception('currTempSetPoint'+str(i)+' '+str(currentSetPoint)+' does not match scheduleSetPoint'+str(i)+' '+str(scheduldeSetPoint))
			else:
				testCase.systemout += 'scheduleMode'+str(i)+'='+str(scheduleMode)+' currTempSetPoint'+str(i)+' '+str(currentSetPoint)+' matches schedule'+str(i)+'CurrentTemperature '+str(scheduldeSetPoint)+' or manualSetPoint'+str(i)+' '+str(manualSetPoint)+'.'
	except Exception as e:
		testCase.systemerr = str(e)  
	return testCase

def runTestMultiRelay(devmanApi,deviceId):
	testCase = TestCase()
	testCase.classname='inx.heatrod.tests.multirelay'
	testCase.name=deviceId
	"""
	commented out till we upgrade from python 3.5
	loop = asyncio.get_event_loop()

	try:
		ip=devmanApi.getDeviceIP(deviceId)
		wsurl=ip+'/ws'
		#we need to fetch the array of multi relay tests
		deviceStatus=devmanApi.getDeviceStatus(deviceId)
		if 'multiRelayTests' in deviceStatus['debugger']:
			sessionId=deviceGetSessionId(devmanApi,deviceId)
			sessionId=sessionId+1
			for multiRelayTest in deviceStatus['debugger']['multiRelayTests']:
				postdata=multiRelayTest['payload']
				postdata['sessionId']=hashSessionId(sessionId)
				postdata['url']='post'
				response=loop.run_until_complete(websocket(wsurl,postdata,True))
				sessionId=sessionId+1
				#wait for devman to get updated settings
				time.sleep(5)
				#now ask for the current power out on the relays
				response=loop.run_until_complete(websocket(wsurl,'',False))
				print(response)
				newStatus=json.loads(response)
				comment=""
				if 'comment' in multiRelayTest:
					comment = multiRelayTest['comment']
				testCase.systemout += "\n"+comment+": "
				for key in multiRelayTest['expected']:
					if str(multiRelayTest['expected'][key]) != str(newStatus[key]):
						raise Exception('sent '+str(postdata)+' '+key+' did not match '+str(multiRelayTest['expected'][key])+' '+str(newStatus[key])+" received="+response)
					else:
						testCase.systemout += 'Updated '+key+' to '+str(newStatus[key])+'. '
	except Exception as e:
		exc_type, exc_obj, exc_tb = sys.exc_info()
		testCase.systemerr = str(exc_tb.tb_lineno)+":"+str(e)
	"""
	return testCase

def runTestUpdateSettings(devmanApi,deviceId,skipHardwarePINTest=False):
	testCase = TestCase()
	testCase.classname='inx.heatrod.tests.updatesettings'
	testCase.name=deviceId
	loop = asyncio.get_event_loop()
	ignoreRegexes=[r'timeStamp']
	try:
		ip=devmanApi.getDeviceIP(deviceId)

		#check bad url does not crash device
		url='http://'+ip+'/randomurl.cgi'
		try:
			response = requests.post(url)
			if response.text == "No session.":
				testCase.systemout += "Received correct response for bad url. "
			else:
				raise Exception("Bad response to bad url request:"+response.text)
		except:
			raise Exception("Could not perform bad url request.")

		#check we can't post before we have a valid session
		url='http://'+ip+'/post_hardware.cgi'
		postdata={'test':'test'}
		try:
			response = requests.post(url,data=postdata)
			if response.text == "No session.":
				testCase.systemout += "Received no session. Will try to create a session. "
			else:
				raise Exception("Bad response to no session request:"+response.text)
		except:
			raise Exception("Could not perform no session request.")

		#check we reject bad session start request
		url='http://'+ip+'/post_session_start.cgi'
		try:
			response = requests.post(url)
			if response.text == "Bad certificate.":
				testCase.systemout += "Received correct response. Will try to create a session with valid certificate. "
			else:
				raise Exception("Bad response to bad session start request:"+response.text)
		except:
			raise Exception("Could not perform bad session start request.")

		#check we accept good session start request
		url='http://'+ip+'/post_session_start.cgi'
		certificate=devmanApi.getProvisioningCertificate(deviceId)
		postdata='certificate='+certificate
		try:
			response = requests.post(url,data=postdata)
			print(response.text)
		except:
			raise Exception("Could not perform good session start request.")
		try:
			data=json.loads(response.text)
		except:
			raise Exception("Response could not be parsed to json")
		if 'sessionId' in data:
			testCase.systemout += "Received sessionId="+str(data['sessionId'])+'. '
		else:
			raise Exception("Bad response to good session start request:"+response.text)
		
		#check we reject an update with a bad session id
		url='http://'+ip+'/post.cgi'
		try:
			response = requests.post(url,data=postdata)
		except:
			raise Exception("Could not perform good session start request.")
		if response.text == "No session.":
			testCase.systemout += "Correctly rejected request with no sessionId. "
		else:
			raise Exception("Did not reject request with no sessionId. response="+response.text)
		
		#check we reject request with bad sessionId
		try:
			response = requests.post(url,data="sessionId=12&"+postdata)
		except:
			raise Exception("Failed to make request with bad session id")
		if response.text == "No session.":
			testCase.systemout += "Correctly rejected request with bad sessionId. "
		else:
			raise Exception("Did not reject request with bad sessionId. response="+response.text)

		#check we reject post request with good sessionId
		#as we only use web sockets for posting settings now
		sessionId=data['sessionId']+1
		hashedSessionId=hashSessionId(sessionId)
		try:
			response = requests.post(url,data="sessionId="+hashedSessionId+"&"+postdata)
		except:
			raise Exception("Failed to make request with good session id")
		if response.text == "Bad method.":
			testCase.systemout += "Correctly ignored request with good sessionId. "
		else:
			raise Exception("Rejected request with good sessionId. response="+response.text)
		sessionId=sessionId+1
		
		#check that we cannot make an update before submitting a valid hardware PIN
		if skipHardwarePINTest:
			testCase.systemout += 'Skipping hardware PIN test.'
		else:
			postdata={}
			url=ip+'/ws'
			postdata['sessionId']=hashSessionId(sessionId)
			postdata['url']='post'
			response=''
			try:
				response=loop.run_until_complete(websocket(url,postdata,True,ignoreRegexes))
				print(response)
				response=json.loads(response)
			except:
				print('Failed to talk to device for bad PIN test.')
			if 'data' in response and response['data']=='Please enter a valid PIN.':
				testCase.systemout += 'Correctly ignored request without a hardware PIN.'
			else:
				raise Exception("Bad response to settings update without a PIN "+str(response))
			sessionId=sessionId+1

			#now send an invalid PIN
			postdata['pin']='00000'
			postdata['sessionId']=hashSessionId(sessionId)
			response=''
			try:
				response=loop.run_until_complete(websocket(url,postdata,True,ignoreRegexes))
				response=json.loads(response)
			except:
				print('Failed to talk to device for bad PIN test.')
			if 'data' in response and response['data']=='Please enter a valid PIN.':
				testCase.systemout += 'Correctly ignored request without a valid PIN.'
			else:
				raise Exception("Bad response to settings update with invalid PIN "+str(response))
			sessionId=sessionId+1

			#now send a valid PIN
			deviceStatus=devmanApi.getDeviceStatus(deviceId)
			postdata['pin']=deviceStatus['debugger']['provisioning']['pin']
			postdata['sessionId']=hashSessionId(sessionId)
			response=''
			try:
				response=loop.run_until_complete(websocket(url,postdata,True,ignoreRegexes))
				response=json.loads(response)
			except:
				print('Failed to talk to device for bad PIN test.')
			if 'data' in response and len(response['data'].keys())==0:
				testCase.systemout += 'Correctly accepted valid PIN.'
			else:
				raise Exception("Bad response to settings update with valid PIN "+str(response))
			sessionId=sessionId+1

		#now update the hardware settings
		if 'hardwareSettings' in deviceStatus['debugger']:
			postdata=deviceStatus['debugger']['hardwareSettings']
			postdata['sessionId']=hashSessionId(sessionId)
			postdata['url']='post'
			try:
				response=loop.run_until_complete(websocket(url,postdata,True,ignoreRegexes))
				print(response)
				sessionId=sessionId+1
			except:
				print('')
			time.sleep(15)


		#devmanApi.sendDeviceSchedule(deviceId)
		#wait a minute for them to take
		#time.sleep(15)

		#reboot the device
		#devmanApi.rebootDevice(deviceId)

		#wait a minute for it to come back online
		#time.sleep(60)

		#check that settings are still correct
		#newSetPoint=devmanApi.getDeviceManualSetPoint(deviceId)
		#if float(newSetPoint)==float(manualSetPoint):
		#    testCase.systemout += 'Updated manual set point to '+str(newSetPoint)+'. '
		#else:
		#    raise Exception('Set point did not update correctly via devman '+str(newSetPoint)+'!='+str(manualSetPoint))

		#now try to do the same thing via the local web ui
		#manualSetPoint=manualSetPoint+1.0
		#url='http://'+ip+'/post.cgi'
		#postdata={'manualSetPoint':manualSetPoint,'msp':manualSetPoint}
		#try:
		#    response = requests.post(url,data=postdata)
		#except:
		#    print('')
		#wait for devman to pick up this change
		#time.sleep(15)
		
		#check that settings are still correct
		#newSetPoint=devmanApi.getDeviceManualSetPoint(deviceId)
		#if float(newSetPoint)==float(manualSetPoint):
		#    testCase.systemout += 'Updated manual set point to '+str(newSetPoint)+'. '
		#else:
		#    raise Exception('Set point did not update correctly via local UI '+str(newSetPoint)+'!='+str(manualSetPoint))

		if 'localSettingsUpdateTests' in deviceStatus['debugger']:
			url=ip+'/ws'
			for settingsUpdateTest in deviceStatus['debugger']['localSettingsUpdateTests']:
				postdata=settingsUpdateTest['payload']
				postdata['sessionId']=hashSessionId(sessionId)
				postdata['url']='post'
				try:
					response =loop.run_until_complete(websocket(url,postdata,True,ignoreRegexes))
					sessionId=sessionId+1
				except Exception as e:
					exc_type, exc_obj, exc_tb = sys.exc_info()
					print(str(exc_tb.tb_lineno)+":"+str(e))
				#wait for devman to get updated settings
				time.sleep(15)
				newStatus=devmanApi.getDeviceStatus(deviceId)
				for key in settingsUpdateTest['expected']:
					if str(settingsUpdateTest['expected'][key]) != str(newStatus['controller'][key]):
						raise Exception('sent '+str(postdata)+' '+key+' did not match '+str(settingsUpdateTest['expected'][key])+' '+str(newStatus['controller'][key])+" received="+response)
					else:
						testCase.systemout += 'Updated '+key+' to '+str(newStatus['controller'][key])+'. '
	except Exception as e:
		exc_type, exc_obj, exc_tb = sys.exc_info()
		testCase.systemerr = str(exc_tb.tb_lineno)+":"+str(e)
	return testCase

def runTestWebUI(devmanApi,deviceId):
	testCase = TestCase()
	testCase.classname='inx.heatrod.tests.webui'
	testCase.name=deviceId

	try:
		deviceIp=devmanApi.getDeviceIP(deviceId)
		url = 'http://'+deviceIp
		responseTimes=[]
		for i in range(0,10):
			#request a page
			time.sleep(1)
			response=requests.get(url,timeout=10)
			#check still online
			if response.text.find('restricted')>-1 or response.text.find('Mini-Dashboard')>-1 or response.text.find('welcome')>-1:
				responseTimeSeconds=response.elapsed.total_seconds()
				print('ok responseTimeSeconds='+str(responseTimeSeconds))
				responseTimes.append(responseTimeSeconds)
			else:
				raise Exception('Device went offline at request '+str(i))
		#assume if we got here all was fine
		avgSeconds = sum(responseTimes) / len(responseTimes)
		testCase.systemout = 'Made '+str(len(responseTimes))+' requests. Avg response time seconds='+str(avgSeconds)
	except Exception as e:
		testCase.systemerr = str(e)
	return testCase

if __name__ == "__main__":
	print("running tests")
	devmanApi = devman.Devman()
	testSuites=[]
	
	deviceIds=devmanApi.getDeviceIds('hrjreconnection')

	testSuite1=TestSuite()
	testSuite1.name = 'Online Tests'
	testSuites.append(testSuite1)

	for deviceId in deviceIds['deviceIds']:
		print(deviceId)
		testCase1=runTestOnlineState(devmanApi,deviceId)
		testSuite1.addTestCase(testCase1)

	writeJunitFile('online',testSuites)

	#now do reconnection test
	testSuites=[]
	testSuite1=TestSuite()
	testSuite1.name = 'Reconnection Tests'
	testSuites.append(testSuite1)

	print('connecting to router...')
	router=microtik.MicrotikRouter()
	print('adding rules')
	router.addRuleToBlockDevman()
	for deviceId in deviceIds['deviceIds']:
		print(deviceId)
		testCase=runTestReconnection(devmanApi,router,deviceId)
		testSuite1.addTestCase(testCase)
		testCase=runTestReconnection(devmanApi,router,deviceId,5)
		testSuite1.addTestCase(testCase)
		testCase=runTestReconnection(devmanApi,router,deviceId,10)
		testSuite1.addTestCase(testCase)
		testCase=runTestNetworkStress(devmanApi,deviceId)
		testSuite1.addTestCase(testCase)
		testCase=runTestWebUI(devmanApi,deviceId)
		testSuite1.addTestCase(testCase)
	router.removeRuleToFilterDevman()
	writeJunitFile('reconnection',testSuites)