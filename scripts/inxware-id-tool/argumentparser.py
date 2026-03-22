
from configurations import Configurations

class ArgumentParser(object):
    def __init__(self, args):
        self.args=iter(args)
        self.config=Configurations()
        self.printHelp=False
        self.error=""

    def read(self):
        for arg in self.args:
            # set root directory
            if arg == "-d":
                try:
                    self.config.root=next(self.args)
                except:
                    self.error="The directory path must be specified when using '-d'. e.g. -d <path to root directory>"
                    return False
            # set mode
            elif arg == "-m":
                try:
                    self.config.mode=next(self.args)
                except:
                    self.error="The mode must be specified when using '-m'. See help (-h) for further details."
                    return False
            # change extentions
            elif arg == "-e":
                try:
                    self.config.exts=next(self.args)
                except:
                    self.error="The extentions must be specified when using '-e'. See help (-h) for further details."
                    return False
            # save report to a file
            elif arg == "-rf":
                try:
                    self.config.reportfile=next(self.args)
                except:
                    self.error="The path for saving the report must be specified when using '-rf'. See help (-h) for further details."
                    return False
            # apply
            elif arg == "-apply":
                self.config.apply=True
            # keep old
            elif arg == "-keep":
                self.config.keepold=True
            # change hash
            elif arg == "-hash":
                try:
                    self.config.hash=next(self.args)
                except:
                    self.error="The hash type must be specified when using '-hash'. See help (-h) for further details."
                    return False
            # print hash of a string
            elif arg == "-genHash":
                try:
                    self.config.genHash=next(self.args)
                except:
                    self.error="The string for generating hash must be specified when using '-genHash'. See help (-h) for further details."
                    return False
            # print help
            elif arg == "-h":
                self.printHelp=True
            else:
                self.error="Unknown argument: %s" % arg
                return False
        return True

    def getConfigurations(self):
        return self.config

    def help(self):
        if self.printHelp:
            print("Inx EHS tool.")
            print("help in progress ... (TODO)")
            return True
        return False

    def getError(self):
        return self.error
