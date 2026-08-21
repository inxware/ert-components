from xml.sax.saxutils import escape
import os
import time
#inx imports
import devman

TEST_RESULTS_DIR_NAME='test-results'

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

if __name__ == "__main__":
	testSuites=[]
	testSuite1=TestSuite()
	testSuite1.name = 'Update Tests'
	testSuites.append(testSuite1)
	devmanApi = devman.Devman()
	deviceIds=devmanApi.getDeviceIds('ci_x86_ambifier2_media')
	for deviceId in deviceIds['deviceIds']:
		print(deviceId)
		deviceStatus = devmanApi.getDeviceStatus(deviceId)
		previousVersion = deviceStatus['device']['ehs_version']
		devmanApi.startOSUpdate(deviceId,'jenkins_x86_ambifier2_vagrant')
		testCase=TestCase()
		testCase.classname='inx.player.tests.updated'
		testCase.name=deviceId
		time.sleep(60*5)
		deviceStatus = devmanApi.getDeviceStatus(deviceId)
		newVersion = deviceStatus['device']['ehs_version']
		if previousVersion==newVersion:
			testCase.systemerr += 'Version did not change '+previousVersion+' '+newVersion+'. '
		else:
			testCase.systemout += 'Version changed '+previousVersion+' '+newVersion+'. '
		isOnline=devmanApi.getDeviceOnline(deviceId)
		if isOnline:
			testCase.systemout += 'Device online. '
		else:
			testCase.systemerr += 'Device offline. '
		testSuite1.addTestCase(testCase)
	writeJunitFile('updated',testSuites)