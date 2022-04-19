

class Configurations(object):
    def __init__(self):
        # default
        self.root=""
        self.mode="updateid"
        self.exts="c,h"
        self.apply=False
        self.reportfile=None
        self.keepold=False
        self.hash="32CRC"

    def getRoot(self):
        return self.root

    def getMode(self):
        return self.mode

    def getExtentions(self):
        exts=self.exts
        exts=exts.replace(" ", "")
        if "," not in exts:
            return [ exts ]
        return exts.split(",")

    def getApply(self):
        return self.apply

    def getKeepOld(self):
        return self.keepold

    def getReportFile(self):
        return self.reportfile

    def getHash(self):
        return self.hash
