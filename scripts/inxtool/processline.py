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
            text=b"#ifndef EHRT1\n"
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

    def replace(self, line, valuestring, hashstring):
        return line.replace(b'"'+valuestring+b'"', hashstring)

    def process(self):
        line=self.line
        if line is None or line == b"":
            return False

        if self.check(line):
            strings=extractStrings(line)
            for value in strings:
                hashObject=HashFactory(value, self.config.getHash())
                if hashObject is not None:
                    if hashObject.isInteger():
                        self.hashstring=str('0x' + hashObject.getHash() ).encode("UTF-8")
                    else:
                    self.hashstring=str('"' + hashObject.getHash() + '"').encode("UTF-8")
                    self.valuestring=value
                    self.report.addSummaryInfo(self.valuestring,self.hashstring)
                    self.processed=self.keepold(line, self.replace(line, self.valuestring, self.hashstring))
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

class UpdateIdsFunctionsNewProcessLine(UpdateIdsFunctionsProcessLine):

    def replace(self, line, valuestring, hashstring):
        value=b'"'+valuestring+b'"'
        id_value=b'FUNCTION_NAME_ID_'+valuestring.upper()
        return line.replace(value, id_value)

    def keepold(self, old, new):
        if not self.config.getKeepOld():
            return new
        if new is not None and old is not None and \
           new != old:
            id_value=b'FUNCTION_NAME_ID_'+self.valuestring.upper()
            text=b"#ifndef EHRT1\n"
            text=text+b"#define "+id_value+b" "+b'"'+self.valuestring+b'"'
            text=text+b"\n#else\n"
            text=text+b"#define "+id_value+b" "+self.hashstring
            text=text+b"\n#endif\n"
            text=text+new
            return text
        return new

class UpdateIdsInxWareProcessLine(UpdateIdsProcessLine):

    def check(self, line):
        # Update IDs of the hash defines
        if b'INXWARE_FB' in line and b'#define' in line \
        and b'"' in line:
            return True
        return False

def ProcessLineFactory(line, type):
    if type == "updateid":
        return UpdateIdsProcessLine(line)
    if type == "updatefooid":
        return UpdateIdsFunctionsProcessLine(line)
    if type == "updatefoonewid":
        return UpdateIdsFunctionsNewProcessLine(line)
    if type == "updateinxwareid":
        return UpdateIdsInxWareProcessLine(line)
    return None
