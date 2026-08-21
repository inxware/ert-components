import datetime
import requests
from json import JSONDecodeError
import hashlib

class Devman:
    baseUrl = ''
    apiToken = ''
    def __init__(self,baseUrl='https://devman.inx-systems.com',apiToken='securetoken98'):
        self.baseUrl=baseUrl
        self.apiToken = apiToken

    def getAvailableFirmwares(self,deviceId):
        status=self.getDeviceStatus(deviceId)
        return status['availableFirmwares']

    def getLastMeasurementTimeUnixSeconds(self,deviceId):
        status=self.getDeviceStatus(deviceId)
        date=datetime.datetime.strptime(status['device']['lastMeasurementsDate'],'%Y-%m-%d %H:%M:%S')
        date += (datetime.datetime.now() - datetime.datetime.utcnow())
        return date.timestamp()

    def getTimeBetweenMeasurementsSeconds(self,deviceId):
        status=self.getDeviceStatus(deviceId)
        return int(status['device']['timeBetweenMeasurements'])

    def getDeviceIds(self,group):
        url=self.baseUrl+'/api/tests/getDevicesForTestingV1.php'
        response = requests.post(url,json={'token':self.apiToken,'group':group})
        return response.json()

    def getDeviceIP(self,deviceId):
        status=self.getDeviceStatus(deviceId)
        return status['device']['ipaddr']

    def getDeviceOnline(self,deviceId):
        deviceStatus = self.getDeviceStatus(deviceId)
        pulseDate=deviceStatus['device']['pulse_date']
        deviceDateTime = datetime.datetime.strptime(pulseDate, "%Y-%m-%d %H:%M:%S")
        deviceDateTimeDelta = datetime.datetime.utcnow() - deviceDateTime
        if deviceDateTimeDelta > datetime.timedelta(minutes=3):
            return False
        else:
            return True

    def getDeviceManualSetPoint(self,deviceId):
        status=self.getDeviceStatus(deviceId)
        return status['controller']['manualSetPoint1']

    def getDeviceStatus(self,deviceId):
        url=self.baseUrl+'/api/tests/deviceStatusV1.php'
        response = requests.post(url, json={'deviceId':deviceId, 'token':self.apiToken})
        #print(deviceId+response.text)
        return response.json()

    def getLPCLinkSerialAndPort(self,deviceId):
        deviceStatus=self.getDeviceStatus(deviceId)
        if 'serial' not in deviceStatus['debugger']:
            deviceStatus['debugger']['serial']=''
        if 'port' not in deviceStatus['debugger']:
            deviceStatus['debugger']['port']=''
        return (deviceStatus['debugger']['serial'],deviceStatus['debugger']['port'])

    def getLPCLinkSerialsAndPorts(self,group):
        deviceIds=self.getDeviceIds(group)
        serialsAndPorts=[]
        for deviceId in deviceIds['deviceIds']:
            (serial,port)=self.getLPCLinkSerialAndPort(deviceId)
            serialsAndPorts.append((serial,port))
        return serialsAndPorts

    def getProvisioningCertificate(self,deviceId):
        deviceStatus=self.getDeviceStatus(deviceId)
        username=deviceStatus['debugger']['provisioning']['username']
        password=hashlib.md5(str(deviceStatus['debugger']['provisioning']['password']).encode('utf-8')).hexdigest()
        url='https://devport.inx-systems.com/api/iotapp/secure/requestcert.php'
        response = requests.post(url,json={'cert':'yes','username':username,'password':password})
        try:
            cert = response.json()['cert']
        except JSONDecodeError as jde:
            raise Exception('Could not get cert response.text='+response.text)
        return cert

    def rebootDevice(self,deviceId,factoryReset=0):
        url=self.baseUrl+'/api/tests/rebootDeviceV1.php'
        response = requests.post(url,json={'deviceId':deviceId,'token':self.apiToken,'factoryReset':factoryReset})
        return response.json()

    def sendDeviceSchedule(self,deviceId):
        url=self.baseUrl+'/api/tests/sendDeviceScheduleV1.php'
        response = requests.post(url,json={'deviceId':deviceId,'token':self.apiToken})
        return response.json()

    def setDeviceManualSetPoint(self,deviceId,manualSetPoint):
        url=self.baseUrl+'/api/tests/updateDeviceSettingsV1.php'
        response = requests.post(url,json={'deviceId':deviceId,'manualSetPoint':str(manualSetPoint),'token':self.apiToken})
        return response.json()

    def startOSUpdate(self,deviceId,osUpdate):
        url=self.baseUrl+'/api/tests/startOSUpdateV1.php'
        response = requests.post(url,json={'token':self.apiToken,'deviceId':deviceId,'version':osUpdate})
        print(response.text)
        return response.json()
    
    def startOTA(self,deviceId,versionToFlash):
        url=self.baseUrl+'/api/tests/startOTAV1.php'
        response = requests.post(url,json={'token':self.apiToken,'deviceId':deviceId,'version':versionToFlash})
        return response.json()