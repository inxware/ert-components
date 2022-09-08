import re
from hash import *

def extractStrings(line):
    return re.findall(b'"([^"]*)"', line)

class ProcessLine(object):
    def __init__(self, line, lines_buf):
        self.line=line
        self.report=None
        self.filepath=None
        self.processed=None
        self.config=None
        self.valuestring=None
        self.hashstring=None
        self.lines_buf=lines_buf

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
            self.report.addInfo(self.filepath, "=============================================\nAlready modified:")
            self.report.addInfo(self.filepath, self.line.decode("UTF-8"))
            return None
        filetext=filetext.replace(self.line, self.processed)
        if self.filepath is not None:
            self.report.addInfo(self.filepath, "=============================================\nText:")
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

class UpdateHashOnlyProcessLine(UpdateIdsProcessLine):
    
    def check(self, line):
        # Update IDs of the hash defines
        if b'#define' in line and b'0x' in line: # check if hex is defined
            if b'EHS_FB' in line \
            or b'FUNCTION_NAME_ID_' in line \
            or b'INXWARE_FB' in line:
                return True
        return False

    def check_str(self, line):
        # Update IDs of the hash defines
        if b'#define' in line and b'"' in line: # check if hex is defined
            if b'EHS_FB' in line \
            or b'FUNCTION_NAME_ID_' in line \
            or b'INXWARE_FB' in line:
                return True
        return False
    
    def replace(self, line, valuestring, hashstring):
        hash_start=line.find(b'0x')
        number_start=hash_start+2
        hash_val=line[hash_start:number_start+4]
        print("%s -> %s" % (hash_val, hashstring))
        return line.replace(hash_val, hashstring)

    def keepold(self, old, new):
        # no need to keep old in this case
        return new
            

    def process(self):
        line=self.line
        if line is None or line == b"":
            return False

        if self.check(line):
            line_with_string=None
            for i in reversed(self.lines_buf):
                if self.check_str(i):
                    line_with_string=i
                    break
            if line_with_string is None:
                print("Failed to find string id for (%s)"%line)
                return False
            strings=extractStrings(line_with_string)
            for value in strings:
                hashObject=HashFactory(value, self.config.getHash())
                if hashObject is not None:
                    self.hashstring=str('0x' + hashObject.getHash() ).encode("UTF-8")
                    self.valuestring=value
                    self.report.addSummaryInfo(self.valuestring,self.hashstring)
                    self.processed=self.keepold(line, self.replace(line, self.valuestring, self.hashstring))
                    return True
        return False


def ProcessLineFactory(line, lines_buf, type):
    if type == "updateid":
        return UpdateIdsProcessLine(line, lines_buf)
    if type == "updatefooid":
        return UpdateIdsFunctionsProcessLine(line, lines_buf)
    if type == "updatefoonewid":
        return UpdateIdsFunctionsNewProcessLine(line, lines_buf)
    if type == "updateinxwareid":
        return UpdateIdsInxWareProcessLine(line, lines_buf)
    if type == "update_hash_only":
        return UpdateHashOnlyProcessLine(line, lines_buf)
    return None
