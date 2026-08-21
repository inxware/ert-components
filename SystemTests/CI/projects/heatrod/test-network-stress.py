import devman
import tests
import microtik
import argparse

if __name__ == "__main__":
	parser = argparse.ArgumentParser(
		description='Run the set of release tests for group hr_network_stress')
	parser.add_argument(
		'--routerHost', help='IP address of the microtik router', required=False)
	args = parser.parse_args()
	testSuites = []
	testSuite1 = tests.TestSuite()
	testSuites.append(testSuite1)
	testSuite1.name = 'Network Stress Tests'

	# get devices
	devmanApi = devman.Devman()
	deviceIds = devmanApi.getDeviceIds('hr_network_stress')
	
	print('connecting to router...')
	if args.routerHost is not None:
		router = microtik.MicrotikRouter(host=args.routerHost)
	else:
		router = microtik.MicrotikRouter()
	
	#print('adding rules')
	#router.addRuleToBlockDevman()
	for deviceId in deviceIds['deviceIds']:
		print(deviceId)
		testCase = tests.runTestReconnection(devmanApi, router, deviceId)
		testSuite1.addTestCase(testCase)
		testCase = tests.runTestReconnection(devmanApi, router, deviceId, 5)
		testSuite1.addTestCase(testCase)
		testCase = tests.runTestReconnection(devmanApi, router, deviceId, 10)
		testSuite1.addTestCase(testCase)
		#testCase = tests.runTestNetworkStress(devmanApi, deviceId)
		#testSuite1.addTestCase(testCase)
		#testCase = tests.runTestWebUI(devmanApi, deviceId)
		#testSuite1.addTestCase(testCase)
	#router.removeRuleToFilterDevman()
	# write the results out
	tests.writeJunitFile('network-stress', testSuites)
