import argparse, devman

def getSerialsAndPorts(group):
	devmanApi = devman.Devman()
	return devmanApi.getLPCLinkSerialsAndPorts(group)

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Output a list of lpclink serials for a given group name.')
	parser.add_argument('--group', help='group name that you want the serials for, usually the TEST_NAME', required=True)
	args = parser.parse_args()
	serials=getSerialsAndPorts(args.group)
	for serial,port in serials:
		print(serial+","+port)
	#print('611000000,2334')