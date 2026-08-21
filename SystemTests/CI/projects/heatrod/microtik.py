import telnetlib
import requests
import time

class MicrotikRouter:
    tn = None
    prompt = ''
    def __init__(self,host='192.168.88.1',user='jenkins',password='password'):
        self.tn = telnetlib.Telnet(host,23)
        self.readUntil('Login: ')
        self.writeLine(user)
        self.readUntil('Password: ')
        self.writeLine(password)
        self.prompt='['+user+'@MikroTik] > '
        self.waitForPrompt()

    def addIPToList(self,ip,list):
        self.writeCommand('ip firewall address-list add list='+list+' address='+ip)
    
    def addRuleToBlockDevman(self):
        self.removeRuleToFilterDevman() #always remove the rule first
        self.writeCommand('ip firewall filter print') #so that the rules are numbered correctly
        self.disableFastTrack()
        self.writeCommand('ip firewall filter print') #so that the rules are numbered correctly
        self.writeCommand('ip firewall filter add chain=forward dst-address-list=restricted action=drop src-address-list=devices place-before=1 comment="inx1"')
        self.addIPToList('178.79.179.5','restricted')#address of devman

    def disableFastTrack(self):
        self.writeCommand(':ip firewall filter disable numbers=[ip firewall filter find action=fasttrack-connection]')

    def disableWan(self):
        self.writeCommand('interface disable ether1')

    def enableWan(self):
        self.writeCommand('interface enable ether1')

    def readUntil(self,expected):
        response=self.tn.read_until(expected.encode('ascii'))
        #print(response)
        return response

    def removeRuleToFilterDevman(self):
        self.writeCommand(':ip firewall filter remove numbers=[ip firewall filter find comment="inx1"]')

    def removeFirewallFilter(self,number):
        self.writeCommand('ip firewall filter remove numbers='+str(number))

    def removeIPFromList(self,ip,list):
        self.writeCommand(':ip firewall address-list remove numbers=[ip firewall address-list find address='+ip+' list='+list+']')

    def waitForPrompt(self):
        response = self.readUntil(self.prompt)
        print(response)
        time.sleep(1)
        return response

    def write(self,input):
        self.tn.write(input.encode('ascii'))

    def writeCommand(self,input):
        print(input)
        self.writeLine(input)
        self.waitForPrompt()
    
    def writeLine(self,input):
        self.write(input+'\r\r\n')

def testTalkToDevman():
    try:
        print(requests.get('https://devman.inx-systems.com', timeout=10).text)
    except:
        print('failed')

if __name__ == "__main__":
    router = MicrotikRouter()
    print("I'm in, adding firewall rules")
    router.addRuleToBlockDevman()
    print('Test talking to devman')
    testTalkToDevman()
    print('Adding myself to devices list')
    router.addIPToList('192.168.88.254','devices')
    print('Test talking to devman')
    testTalkToDevman()
    print('Removing myself from devices list')
    router.removeIPFromList('192.168.88.254','devices')
    print('test talking to devman')
    testTalkToDevman()