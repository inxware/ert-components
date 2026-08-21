
import requests, time, json
import local_web_ui.mdns_handler as mdns_handler
from local_web_ui.mdns_handler import MdnsHandler, MdnsFilter

cert_dir='local_web_ui/cert/'

def appendPayload(payload, key, value):
    if payload is None or payload is '':
        payload=''
    else:
        payload=payload + '&'
    payload=payload + key
    payload=payload + '='
    payload=payload + value
    return payload

def postRequest(ip_address,cgi,payload):
    try:
        endpoint="http://%s/%s" % (ip_address,cgi)
        response=requests.post(url=endpoint, data=payload).text
        print("Response:%s"%response)
        return response
    except:
        print("Failed to create request.")
    return None

def readProvisionCertificate(name):
    print("Read the certificate file.")
    try:
        file_path = '%s%s%s' % (cert_dir,name,'.pem');
        f = open(file_path, "r")
        certificate=f.read()
        certificate=certificate.replace('\r','')
        #print(certificate)
        return certificate
    except:
        print('Failed to read file')
    return None

def requestProvisionStatus(ip_address):
    certificate=appendPayload(None,'certificate',readProvisionCertificate('test'))
    return postRequest(ip_address,"post_mobile_status.cgi",certificate)

def setFactorySettings(ip_address):
    payload=appendPayload(None,'resetMode','3')
    return postRequest(ip_address,"post_reset.cgi",payload)

# check provision status
def checkProvisionStatus(ip_address):
    status=None
    response=requestProvisionStatus(ip_address)
    try:
        json_response=json.loads(response)
        status=json_response['status']
    except:
        print('Failed to read provision status json')
    return status

# check binded organisation
def checkOrganisationID(ip_address):
    orgID=None
    response=requestProvisionStatus(ip_address)
    try:
        json_response=json.loads(response)
        orgID=json_response['orgID']
    except:
        print('Failed to read provision status json')
    return orgID

def provision(ip_address,password,pin,certificate):
    provisioned=None
    failCount=None
    payload=appendPayload(None,'certificate',readProvisionCertificate(certificate))
    payload=appendPayload(payload,'password',password)
    payload=appendPayload(payload,'pin',pin)
    #print("payload=%s" % payload)
    response=postRequest(ip_address,'post_mobile_provision.cgi',payload)
    try:
        json_response=json.loads(response)
        provisioned=json_response['success']
        failCount=json_response['count']
    except:
        print('Failed to read provision json')
    return provisioned, failCount

def send_provision_info(ip_address,localName,location,localUIMode):
    provisioned=None
    payload=appendPayload(None,'localName',localName)
    payload=appendPayload(payload,'location',location)
    payload=appendPayload(payload,'localUIMode',localUIMode)
    #print("payload=%s" % payload)
    response=postRequest(ip_address,'post_mobile_provisioning_information.cgi',payload)
    try:
        json_response=json.loads(response)
        provisioned=json_response['success']
    except:
        print('Failed to read provision info json')
    return provisioned

def check_webpage_content(ip_address, html, expected):
    webpage = 'http://%s/%s' % (ip_address, html)
    page = requests.get(webpage)
    content=str(page.content)
    #print(content)
    if content.find(expected) < 0:
        error_msg='Webpage (%s) does not contain expected string (%s).' % (webpage, expected)
        raise NameError(error_msg)

def findIPAddress(id):
    ip_address = None
    handler = MdnsHandler()
    handler.start( MdnsFilter('mDnsInxIoT') )
    try:
        ip_address_find_count = 0
        while ip_address is None and ip_address_find_count < 5:
            info=handler.mdns_info(id)
            if info is not None:
                ip_address = mdns_handler.ipAddress(info)
                print("%s, %s" % (info.name, ip_address))
            else:
                time.sleep(2)
                ip_address_find_count = ip_address_find_count + 1
                print("Failed to find device %s (attempts=%s)" % (id,ip_address_find_count))
    finally:
        handler.stop()
        time.sleep(2) # wait to allow mdns to close
    return ip_address
