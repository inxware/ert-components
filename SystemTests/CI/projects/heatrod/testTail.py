import subprocess
import threading
import time
import re
import os,signal
from os import listdir
from os.path import isfile, join

def findLogFile(dirPath,deviceId):
    pattern = re.compile('.*'+deviceId+'.*')
    onlyfiles = [f for f in listdir(dirPath) if isfile(join(dirPath, f))]
    for f in onlyfiles:
        fullPath=dirPath+'/'+f
        with open(fullPath) as fp:
            line = fp.readline()
            while line:
                if pattern.match(line):
                    return fullPath
                line = fp.readline()
    return ''

def runProcess(pids,exe):    
    p = subprocess.Popen(exe, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    pids.append(p.pid)
    while(True):
        # returns None while subprocess is running
        retcode = p.poll() 
        line = p.stdout.readline()
        yield line.decode('utf-8')
        if retcode is not None:
            break

def threadFunction(tailPath,pids,passRegexes,errorRegexes,passes,errors):
    for line in runProcess(pids,['tail','-F',tailPath]):
        for regex in errorRegexes:
            if regex.match(line):
                errors.append(line)
        for regex in passRegexes:
            if regex.match(line):
                passes.append(line)

if __name__ == "__main__":
    passRegexes=[]
    passRegexes.append(re.compile('.*pass1.*'))
    errorRegexes=[]
    errorRegexes.append(re.compile('.*error1.*'))
    tailPath=findLogFile('/home/patrick/work/inx/logfiles','deviceId1')
    passes=[]
    errors=[]
    pids=[]
    t=threading.Thread(target=threadFunction,args=(tailPath,pids,passRegexes,errorRegexes,passes,errors))
    t.start()
    while(1):
        time.sleep(1)
        if len(errors)>0:
            #kill off our reading thread
            for pid in pids:
                os.kill(pid,signal.SIGTERM)
            t.join()
            for line in passes:
                print('pass:'+line)
            for line in errors:
                print('error:'+line)
            break