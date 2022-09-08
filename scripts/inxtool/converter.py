import glob, os
from processline import *
from hash import *

class Report(object):
    def __init__(self, config):
        self.config=config
        self.report={}
        self.reportSummary={}
        self.reportDuplicates={}
        self.reportHashDuplicates={}
        self.totalSaved=0
        self.totalLost=0
        self.totalEqual=0
        self.detailed=False
        self.reportfile=None
        if self.config is not None:
            if self.config.getReportFile() is not None:
                self.reportfile=open(self.config.getReportFile(), 'w')
        else:
            self.reportfile=None

    def __del__(self):
        if self.reportfile is not None:
            self.reportfile.close()

    def addInfo(self, filepath, info):
        try:
            self.report[filepath]
        except:
            self.report[filepath]=[]
        self.report[filepath].append(info)

    def addSummaryInfo(self, name, hashvalue):
        # check for duplicated names
        if name in self.reportDuplicates:
            self.reportDuplicates[name]=self.reportDuplicates[name]+1
        else:
            self.reportDuplicates[name]=1

        # check for duplicated hash
        if hashvalue in self.reportHashDuplicates:
            self.reportHashDuplicates[hashvalue]=self.reportHashDuplicates[hashvalue]+1
        else:
            self.reportHashDuplicates[hashvalue]=1

        self.reportSummary[name]=hashvalue



    def outputSummary(self):
        self.write("\n************* REPORT SUMMARY *************\n")
        duplicated=0
        for name in self.reportDuplicates:
            if self.reportDuplicates[name] > 1:
                duplicated=duplicated+1
        self.write("Duplicated ID (count=%d) :" % duplicated)
        if self.detailed == True and duplicated > 0:
            for name in self.reportDuplicates:
                self.write("    %s" % name)
        duplicated=0
        for hash in self.reportHashDuplicates:
            if self.reportHashDuplicates[hash] > 1:
                duplicated=duplicated+1
        self.write("Duplicated Hash (count=%d) : " % duplicated)
        if self.detailed == True and duplicated > 0:
            for hash in self.reportHashDuplicates:
                self.write("    %s" % hash)
        for name in self.reportSummary:
            hash=self.reportSummary[name]
            if len(name) == len(hash):
                self.totalEqual=self.totalEqual+len(name)
            elif len(name) > len(hash):
                self.totalSaved=self.totalSaved+abs(len(name)-len(hash))
            elif len(name) < len(hash):
                self.totalLost=self.totalLost+abs(len(name)-len(hash))
        self.write("Total number of bytes ramin the same : %d" % self.totalEqual)
        self.write("Total number of bytes is added : %d" % self.totalLost)
        self.write("Total number of bytes is saved : %d" % self.totalSaved)
        self.write("Total gain of bytes : %d" % (self.totalSaved-self.totalLost))
        self.write("\n******************************************\n\n")

    def output(self):
        self.write("\n*** PROCESSED FILES REPORT ***\n")
        for file in self.report:
            self.write("File path: %s" % file)
            for info in self.report[file]:
                self.write(info)
            self.write("")

    def write(self, text):
        if self.reportfile is not None:
            self.reportfile.write(text + "\n")
        else:
            print(text + "\n")

class Converter(object):
    def __init__(self, config):
        self.config=config
        if self.config is not None:
            self.report=Report(config)
            self.mode=self.config.getMode()
        else:
            self.report=None
            self.mode=None
        self.error=""

    def run(self):
        genHash=self.config.getGenHashStr()
        if genHash is not None:
            hashObject=HashFactory(bytearray(genHash,'utf-8'), self.config.getHash())
            if hashObject is not None:
                hashstring=None
                if hashObject.isInteger():
                    hashstring=str('0x' + hashObject.getHash() ).encode("UTF-8")
                else:
                    hashstring=str('"' + hashObject.getHash() + '"').encode("UTF-8")
                print(hashstring.decode("utf-8"))
            return False
        root=self.config.getRoot()
        if root == "":
            self.error="Failed to process files. The root directory must be specified. See help (-h) for further details."
            return False
        if root.endswith('/'):
            root.rstrip('/')
        # Check all files
        exts=self.config.getExtentions()
        if exts is None:
            self.error="Failed to process files. The file extentions must be specified. See help (-h) for further details."
            return False
        for ext in exts:
            ext=ext.replace(".","") # remove dot
            for file in glob.iglob(root+"/**/*."+ext,recursive=True):
                self.process(file)
        return True

    def process(self, filepath):
        # read file and process each line of this file
        lines=[] # processed lines
        lines_buf=[]
        filetext=""
        with open(filepath, 'rb') as file:
            for line in file:
                if len(lines_buf) > 3:
                    lines_buf.pop(0)
                lines_buf.append(line)
                pl=ProcessLineFactory(line, lines_buf, self.mode)
                if pl is not None:
                    pl.config=self.config
                    pl.report=self.report
                    pl.filepath=filepath
                    if pl.process():
                        lines.append(pl)
                else:
                    self.error="Failed to process files. Unknown line process type. See help (-h) for further details."
                    return False
        with open(filepath, 'rb') as file:
            filetext=file.read()
        # apply changes to the file
        if len(lines) > 0:
            self.report.addInfo(filepath, "Total number of processed lines: %s" % len(lines))
            for line in lines:
                text=line.apply(filetext)
                if text is not None:
                    filetext=text
                    
            if self.config.getApply():
                with open(filepath, 'wb') as file:
                    file.write(filetext)

        return True

    def printReport(self):
        self.report.outputSummary()
        self.report.output()

    def getError(self):
        return self.error
