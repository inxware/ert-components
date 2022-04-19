import re
from hash import *

def extractStrings(line):
    return re.findall(b'"([^"]*)"', line)

class ProcessLine(object):
    def __init__(self, line):
        self.line=line
        self.report=None
        self.filepath=None
        self.processed=None
        self.config=None
        self.valuestring=None
        self.hashstring=None

    def process(self):
        raise("Override this function!")

    def apply(self, filetext):
        raise("Override this function!")


    def keepold(self, old, new):
        if not self.config.getKeepOld():
            return new
        if new is not None and old is not None and \
           new != old:
            text=b"#ifdef EHS_USE_STRING_IDS\n"
            text=text+old
            text=text+b"#else\n"
            text=text+new
            text=text+b"#endif\n"
            return text
        return new


class UpdateIdsProcessLine(ProcessLine):

    def check(self, line):
        # Update IDs of the hash defines
        if b'EHS_FB' in line and b'#define' in line \
        and b'"' in line:
            return True
        return False

    def process(self):
        line=self.line
        if line is None or line == b"":
            return False

        if self.check(line):
            strings=extractStrings(line)
            for value in strings:
                hashObject=HashFactory(value, self.config.getHash())
                if hashObject is not None:
                    self.hashstring=str('"' + hashObject.getHash() + '"').encode("UTF-8")
                    self.valuestring=b'"' + value + b'"'
                    self.report.addSummaryInfo(self.valuestring,self.hashstring)
                    self.processed=self.keepold(line, line.replace(self.valuestring, self.hashstring))
                    return True
        return False

    def apply(self, filetext):
        if self.line is None:
            return None
        if self.processed is None:
            return None
        # check if already replaced, to avoid duplicates of kept
        if self.config.getKeepOld() and filetext.find(self.processed) >= 0:
            self.report.addInfo(self.filepath, "Already modified:")
            self.report.addInfo(self.filepath, self.line.decode("UTF-8"))
            return None
        filetext=filetext.replace(self.line, self.processed)
        if self.filepath is not None:
            self.report.addInfo(self.filepath, "Text:")
            self.report.addInfo(self.filepath, self.line.decode("UTF-8"))
            self.report.addInfo(self.filepath, "Was replaced with:")
            self.report.addInfo(self.filepath, self.processed.decode("UTF-8"))
        return filetext

class UpdateIdsFunctionsProcessLine(UpdateIdsProcessLine):

    def check(self, line):
        if b'EHS_FB_FUNCTION' in line and b'#define' not in line \
        and b'"' in line:
            return True
        return False

def ProcessLineFactory(line, type):
    if type == "updateid":
        return UpdateIdsProcessLine(line)
    if type == "updatefooid":
        return UpdateIdsFunctionsProcessLine(line)
    return None
