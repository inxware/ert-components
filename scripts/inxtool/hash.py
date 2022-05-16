import binascii, re

def toEscHex(value):
    x=format(value, 'X')
    return re.sub('(..)', r'\\x\1', x)

def toHex(value, exp_len):
    x=format(value, 'X')
    while len(x) < exp_len:
        x = "0" + x
    return x

class Hash(object):
    def __init__(self, hashtype, value, usebin):
        self.hashtype = hashtype
        self.value=value
        self.usebin=usebin
        self.hash=self.calculate()

    def getHash(self):
        return self.hash

    def getValue(self):
        return self.value

    def calculate(self):
        raise "Override this function!"

    def getHashType(self):
        return self.hashtype

    def isInteger(self):
        if self.hashtype != None and self.hashtype[0] == 'i':
            return True
        return False

class Hash32CRC(Hash):
    def calculate(self):
        if self.value is not None:
            crc32=binascii.crc32(self.value)
            if self.usebin == True:
                return toEscHex(crc32)
            else:
                return toHex(crc32, 8)
        return None

class Hash16CRC(Hash):

    def __init__(self, hashtype, value, usebin):
        self.POLYNOMIAL = 0x1021
        self.PRESET = 0
        self._tab = [ self._initial(i) for i in range(256) ]
        super().__init__(hashtype, value, usebin)

    def _initial(self,c):
        crc = 0
        c = c << 8
        for j in range(8):
            if (crc ^ c) & 0x8000:
                crc = (crc << 1) ^ self.POLYNOMIAL
            else:
                crc = crc << 1
            c = c << 1
        return crc

    def _update_crc(self,crc, c):
        cc = 0xff & c

        tmp = (crc >> 8) ^ cc
        crc = (crc << 8) ^ self._tab[tmp & 0xff]
        crc = crc & 0xffff
        #print (crc)

        return crc

    def crc(self,str):
        crc = self.PRESET
        for c in str:
            crc = self._update_crc(crc, c)
        return crc

    def crcb(self,*i):
        crc = self.PRESET
        for c in i:
            crc = self._update_crc(crc, c)
        return crc

    def calculate(self):
        if self.value is not None:
            crc16=self.crc(self.value)
            if self.usebin == True:
                return toEscHex(crc16)
            else:
                return toHex(crc16, 4)
        return None

def HashFactory(value, type):
    if type == "32CRC" or type == "32crc" or type == "i32CRC" or type == "i32crc":
        return Hash32CRC(type, value,False)
    elif type == "b32CRC" or type == "b32crc":
        return Hash32CRC(type, value,True)
    elif type == "16CRC" or type == "16crc" or type == "i16CRC" or type == "i16crc":
        return Hash16CRC(type, value,False)
    elif type == "b16CRC" or type == "b16crc":
        return Hash16CRC(type, value,True)
    # add other types
    return None
