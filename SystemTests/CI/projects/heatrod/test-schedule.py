import devman
import tests
import argparse


if __name__ == "__main__":
	parser = argparse.ArgumentParser(description='Run the set of schedule tests for a given device group')
	parser.add_argument('--group', help='group name that you to test usually the TEST_NAME, can be a csv', required=True)
	args = parser.parse_args()
	groupNames=args.group.split(',')
	testSuites=[]
	testSuite1=tests.TestSuite()
	testSuites.append(testSuite1)
	testSuite1.name='Schedule Tests'

	#get devices
	devmanApi = devman.Devman()
	for groupName in groupNames:
		deviceIds=devmanApi.getDeviceIds(groupName)
		
		for deviceId in deviceIds['deviceIds']:
			print(deviceId)
			testCase=tests.runTestSchedule(devmanApi,deviceId)
			testSuite1.addTestCase(testCase)

	#write the results out
	tests.writeJunitFile('schedule',testSuites)