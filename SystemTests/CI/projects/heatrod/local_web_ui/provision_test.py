# importing the requests library
import requests, time
import local_web_ui.test_utils as TestUtils
import local_web_ui.mdns_handler as mdns_handler
from local_web_ui.mdns_handler import MdnsHandler, MdnsFilter

class IProvisionInfoTest:
    def send_provision_info_test(self, id, ip_address):
        raise NameError('Failed tests (%s,%s). send_provision_info function must be overriden.' % (id,ip_address))

    def request_local_page_test(self, id, ip_address):
        raise NameError('Failed tests (%s,%s). request_local_page function must be overriden.' % (id,ip_address))

def provisionTest(id, ip_address, password, pin):
    if ip_address is not None:
        # TEST (1) - check that the device is not provisioned
        provisioned=TestUtils.checkProvisionStatus(ip_address)
        if provisioned is 0:
            print('IoT device (%s, %s) is unprovisioned as expected' % (id,ip_address))
        elif provisioned is None:
            raise NameError('IoT device (%s, %s) bad response.' % (id,ip_address))
        else:
            raise NameError('IoT device (%s, %s) should not be provisioned at this stage.' % (id,ip_address))
        # TEST (2) - attempt to provision with an invalid password
        provisioned,count = TestUtils.provision(ip_address,'random1234xxpassword',pin,'test')
        if provisioned is 0 and count is not None:
            print('IoT device (%s, %s) is unprovisioned as expected and failed count is %s' % (id,ip_address,count))
        else:
            raise NameError('Failed to provision IoT (%s, %s) device (test 1).' % (id,ip_address))
        # TEST (3) - attempt to provision with an invalid pin
        provisioned,count = TestUtils.provision(ip_address,password,'1234','test')
        if provisioned is 0 and count is not None:
            print('IoT device (%s, %s) is unprovisioned as expected and failed count is %s' % (id,ip_address,count))
        else:
            raise NameError('Failed to provision IoT (%s, %s) device (test 2).' % (id,ip_address))
        # TEST (4) - attempt to provision with an invalid certificate
        provisioned,count = TestUtils.provision(ip_address,password,pin,'bad_certificate_data')
        if provisioned is None and count is None:
            print('IoT device (%s, %s) is unprovisioned as expected and failed count is %s' % (id,ip_address,count))
        else:
            raise NameError('Failed to provision IoT (%s, %s) device (test 3).' % (id,ip_address))
        # TEST (5) - attempt to provision with success
        provisioned,count = TestUtils.provision(ip_address,password,pin,'test')
        if provisioned is 1:
            print('IoT device (%s, %s) credentials and certificate are valid.' % (id,ip_address))
        else:
            raise NameError('Failed to provision IoT (%s, %s) device (test 4).' % (id,ip_address))
    else:
        raise NameError('Failed to find an expected IoT (%s) device on the network via mDNS' % id)

def runTest(id, password, pin, provisionInfoTest=None,orgID=-1):
    test_passed=True
    test_error=None
    # start test
    handler = MdnsHandler()
    handler.start( MdnsFilter('mDnsInxIoT') )
    try:
        ip_address = None
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
        # (1) TEST - Provision
        provisionTest(id, ip_address, password, pin)

        # (2) TEST - Provision Info with an invalid
        if provisionInfoTest.send_provision_info_test(id, ip_address) is False:
            raise NameError('Failed send provision info test of (%s, %s) device.' % (id,ip_address))
        if provisionInfoTest.request_local_page_test(id, ip_address) is False:
            raise NameError('Failed request local page test of (%s, %s) device.' % (id,ip_address))

        # (3) TEST - check provision status
        provisioned=TestUtils.checkProvisionStatus(ip_address)
        if provisioned is 1:
            print('IoT device (%s, %s) status is provisioned' % (id,ip_address))
        else:
            raise NameError('Failed to confirm provision status (%s, %s) device.' % (id,ip_address))

        # (4) TEST - wait for the organisation # ID
        if orgID is not -1:
            org_find_count = 0
            while 1:
                deviceOrgID=TestUtils.checkOrganisationID(ip_address)
                if deviceOrgID is not None:
                    intOrgID=int(deviceOrgID)
                    if intOrgID is not -1:
                        if intOrgID is orgID:
                            print('IoT device (%s, %s) organisation ID is %s' % (id,ip_address,intOrgID))
                            break
                        else:
                            raise NameError('Failed due to unexpected org ID (exp:%s, acc:%s) (%s, %s) device.' % (orgID,intOrgID,id,ip_address))
                time.sleep(4)
                org_find_count = org_find_count + 1
                print("Failed to device %s organisation (attempts=%s)" % (id,org_find_count))
                if org_find_count > 10:
                    raise NameError('Failed to obtain an org ID of (%s, %s) device.' % (id,ip_address))

    except NameError as e:
        test_passed=False
        test_error=e
    finally:
        handler.stop()

    if test_passed is False:
        print("Test has failed!")
        print("Error %s" % test_error)
    else:
        print("Test PASSED")
    return test_passed, test_error

#runTest(id='5410',password='testPassword1234',pin='9490')

# request certificate every 2 seconds
#while 1:
#    requestCertificate()
#    time.sleep(2)

#requestCertificate()
