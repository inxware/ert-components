import devman
import tests


if __name__ == "__main__":
    testSuites=[]
    testSuite1=tests.TestSuite()
    testSuites.append(testSuite1)
    testSuite1.name='WebUI Tests'

    #get devices
    devmanApi = devman.Devman()
    deviceIds=devmanApi.getDeviceIds('hrjwebui')
    
    for deviceId in deviceIds['deviceIds']:
        print(deviceId)
        testCase=tests.runTestWebUI(devmanApi,deviceId)
        testSuite1.addTestCase(testCase)

    #write the results out
    tests.writeJunitFile('webui',testSuites)