import devman
import tests
import argparse


if __name__ == "__main__":
	parser = argparse.ArgumentParser(description='Run the set of release tests for a given device group')
	parser.add_argument('--group', help='group name that you to test usually the TEST_NAME', required=True)
	args = parser.parse_args()
	groupName=args.group
	testSuites=[]
	testSuite1=tests.TestSuite()
	testSuites.append(testSuite1)
	testSuite1.name='Settings Tests'

	#get devices
	devmanApi = devman.Devman()
	deviceIds=devmanApi.getDeviceIds(groupName)
	
	for deviceId in deviceIds['deviceIds']:
		print(deviceId)
		testCase=tests.runTestUpdateSettings(devmanApi,deviceId)
		testSuite1.addTestCase(testCase)

	#write the results out
	tests.writeJunitFile('settings',testSuites)