import sys
from argumentparser import ArgumentParser
from converter import Converter

def main():
    # Parse the arguments
    parser=ArgumentParser(sys.argv[1:])
    if not parser.read():
        print( parser.getError() )
        return
    if parser.help():
        return
    # Convert IDs
    converter=Converter( parser.getConfigurations() )
    if converter.run():
        converter.printReport()
    else:
        print( converter.getError() )

if __name__ == '__main__':
    main()
