from zeroconf import ServiceBrowser, Zeroconf
import time, socket

class MdnsFilter:
    def __init__(self, mdns_filter_str):
        self.mdns_filter_str=mdns_filter_str

    def check(self, mdns_name):
        return mdns_name.find(self.mdns_filter_str) >= 0

class MdnsListener:
    def __init__(self, mdns_list, mdns_filter=None):
        self.mdns_list = mdns_list
        self.mdns_filter = mdns_filter

    def remove_service(self, zeroconf, type, name):
        print("Service %s removed" % (name,))
        if self.mdns_list is None:
            return
        for info in self.mdns_list:
            # TODO - remove the item from list when it disappears
            pass

    def add_service(self, zeroconf, type, name):
        info = zeroconf.get_service_info(type, name)
        print("Service %s added, service info: %s" % (name, info))
        if self.mdns_filter is not None and self.mdns_filter.check(name) is False:
            print('Service filtered out.')
            return
        if self.mdns_list is not None:
            print('Service added to a list.')
            self.mdns_list.append(info)

class MdnsHandler:
    def __init__(self):
        self.zeroconf = None
        self.listener = None
        self.browser = None
        self.mdns_list = []

    def start(self, mdns_filter=None):
        self.zeroconf = Zeroconf()
        self.listener = MdnsListener(self.mdns_list, mdns_filter)
        self.browser = ServiceBrowser(self.zeroconf, "_http._tcp.local.", self.listener)

    def stop(self):
        if self.zeroconf is not None:
            print("Stopping the mdns listener.")
            self.zeroconf.close()
            print("Mdns stopped.")

    def mdns_info(self, id=None):
        for info in self.mdns_list:
            if id is None or info.name.find(id) >= 0:
                return info
        return None

def ipAddress(info):
    if info is None:
        return None
    return socket.inet_ntoa(info.address)

'''
handler = MdnsHandler()
handler.start( MdnsFilter('mDnsInxIoT') )

try:
    while 1:
        info=handler.mdns_info('5410')
        if info is not None:
            print("%s, %s" % (info.name, ipAddress(info)) )
        time.sleep(1)
    #input("Press enter to exit...\n\n")
finally:
    handler.stop()
'''
