from local_web_ui.provision_test import IProvisionInfoTest
import local_web_ui.test_utils as TestUtils
import requests

UI_MOBILE_ID=0
UI_MODE_MINI_DASHBOARD=1
UI_MODE_FULL_DASHBOARD=2
UI_MODE_FULL_SETTINGS=3
UI_MODE_DISABLE=4

'''These should contain a unique webpage content string'''
RESTRICTED_PAGE_INFO='This service is restricted.'
PROVISION_PAGE_INFO='' #TDOD
MOBILE_PROVISION_PAGE_INFO='<th style="padding:0.5em;text-align:left;">Provisioning</th>'
INDEX_PAGE_INFO='<div id="page_header" class="header_banner" style="min-height:4.5em;">'
MOBILE_INDEX_PAGE_INFO='<div id="page_header" class="header_banner">'
DASHBOARD_PAGE_INFO='<title>INX IoT Welcome</title>'
SETTINGS_PAGE_INFO='<title>INX Configurator</title>'
NETWORK_PAGE_INFO='<title>INX Network Configurator</title>'
MINIDASH_PAGE_INFO='<th style="padding:0.5em;text-align:left;">Mini-Dashboard</th>'


class ProvisionInfoTest_UI_Mobile(IProvisionInfoTest):
    def send_provision_info_test(self, id, ip_address):
        localName='Test UI Mobile Name'
        location='Test UI Mobile City'
        # TEST (1) - check that invalid ui modes id are rejected
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(-1))
        if provision_info is 0:
            print('IoT device (%s, %s) provision info is invalid as expected.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 1).' % (id,ip_address))
        # TEST (2) - check that invalid ui modes id are rejected
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(100))
        if provision_info is 0:
            print('IoT device (%s, %s) provision info is invalid as expected.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 2).' % (id,ip_address))
        # TEST (3) - send valid ui mode
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(UI_MOBILE_ID))
        if provision_info is 1:
            print('IoT device (%s, %s) provision info is valid.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 3).' % (id,ip_address))

    def request_local_page_test(self, id, ip_address):
        # Request - Provision. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'provision.html', RESTRICTED_PAGE_INFO)
        # Request - Minidash. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'minidash.html', RESTRICTED_PAGE_INFO)
        # Request - Index. Expect restricted.
        TestUtils.check_webpage_content(ip_address, '', RESTRICTED_PAGE_INFO)
        TestUtils.check_webpage_content(ip_address, 'index.html', RESTRICTED_PAGE_INFO)
        # Request - Mobile. Expect page.
        TestUtils.check_webpage_content(ip_address, 'mobile.html', MOBILE_INDEX_PAGE_INFO)
        # Request - Dashboard. Expect page.
        TestUtils.check_webpage_content(ip_address, 'welcome.html', DASHBOARD_PAGE_INFO)
        # Request - Settings. Expect page.
        TestUtils.check_webpage_content(ip_address, 'cgi.html', SETTINGS_PAGE_INFO)
        # Request - Network. Expect page.
        TestUtils.check_webpage_content(ip_address, 'network.html', NETWORK_PAGE_INFO)
        print("Local page request test - OK!")

class ProvisionInfoTest_UI_Minidash(IProvisionInfoTest):
    def send_provision_info_test(self, id, ip_address):
        localName='Test UI Minidash Name'
        location='Test UI Minidash City'
        # TEST (1) - check that invalid ui modes id are rejected
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(-1))
        if provision_info is 0:
            print('IoT device (%s, %s) provision info is invalid as expected.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 1).' % (id,ip_address))
        # TEST (2) - check that invalid ui modes id are rejected
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(100))
        if provision_info is 0:
            print('IoT device (%s, %s) provision info is invalid as expected.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 2).' % (id,ip_address))
        # TEST (3) - send valid ui mode
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(UI_MODE_MINI_DASHBOARD))
        if provision_info is 1:
            print('IoT device (%s, %s) provision info is valid.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 3).' % (id,ip_address))

    def request_local_page_test(self, id, ip_address):
        # Request - Provision. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'provision.html', RESTRICTED_PAGE_INFO)
        # Request - Minidash. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'minidash.html', RESTRICTED_PAGE_INFO)
        # Request - Index. Expect minidash page.
        TestUtils.check_webpage_content(ip_address, '', MINIDASH_PAGE_INFO)
        TestUtils.check_webpage_content(ip_address, 'index.html', MINIDASH_PAGE_INFO)
        # Request - Mobile. Expect minidash page.
        TestUtils.check_webpage_content(ip_address, 'mobile.html', MINIDASH_PAGE_INFO)
        # Request - Dashboard. Expect minidash page.
        TestUtils.check_webpage_content(ip_address, 'welcome.html', MINIDASH_PAGE_INFO)
        # Request - Settings. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'cgi.html', RESTRICTED_PAGE_INFO)
        # Request - Network. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'network.html', RESTRICTED_PAGE_INFO)
        print("Local page request test - OK!")

class ProvisionInfoTest_UI_Dashboard(IProvisionInfoTest):
    def send_provision_info_test(self, id, ip_address):
        localName='Test UI Dashboard Name'
        location='Test UI Dashboard City'
        # TEST (1) - check that invalid ui modes id are rejected
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(-20))
        if provision_info is 0:
            print('IoT device (%s, %s) provision info is invalid as expected.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 1).' % (id,ip_address))
        # TEST (2) - check that invalid ui modes id are rejected
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(1100))
        if provision_info is 0:
            print('IoT device (%s, %s) provision info is invalid as expected.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 2).' % (id,ip_address))
        # TEST (3) - send valid ui mode
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(UI_MODE_FULL_DASHBOARD))
        if provision_info is 1:
            print('IoT device (%s, %s) provision info is valid.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 3).' % (id,ip_address))

    def request_local_page_test(self, id, ip_address):
        # Request - Provision. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'provision.html', RESTRICTED_PAGE_INFO)
        # Request - Minidash. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'minidash.html', RESTRICTED_PAGE_INFO)
        # Request - Index. Expect dashboard page.
        TestUtils.check_webpage_content(ip_address, '', DASHBOARD_PAGE_INFO)
        TestUtils.check_webpage_content(ip_address, 'index.html', DASHBOARD_PAGE_INFO)
        # Request - Mobile. Expect dashboard page.
        TestUtils.check_webpage_content(ip_address, 'mobile.html', DASHBOARD_PAGE_INFO)
        # Request - Dashboard. Expect dashboard page.
        TestUtils.check_webpage_content(ip_address, 'welcome.html', DASHBOARD_PAGE_INFO)
        # Request - Settings. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'cgi.html', RESTRICTED_PAGE_INFO)
        # Request - Network. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'network.html', RESTRICTED_PAGE_INFO)
        print("Local page request test - OK!")

class ProvisionInfoTest_UI_FullSettings(IProvisionInfoTest):
    def send_provision_info_test(self, id, ip_address):
        localName='Test UI Full Settings Name'
        location='Test UI Full Settings City'
        # TEST (1) - check that invalid ui modes id are rejected
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(-10))
        if provision_info is 0:
            print('IoT device (%s, %s) provision info is invalid as expected.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 1).' % (id,ip_address))
        # TEST (2) - check that invalid ui modes id are rejected
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(111))
        if provision_info is 0:
            print('IoT device (%s, %s) provision info is invalid as expected.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 2).' % (id,ip_address))
        # TEST (3) - send valid ui mode
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(UI_MODE_FULL_SETTINGS))
        if provision_info is 1:
            print('IoT device (%s, %s) provision info is valid.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 3).' % (id,ip_address))

    def request_local_page_test(self, id, ip_address):
        # Request - Provision. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'provision.html', RESTRICTED_PAGE_INFO)
        # Request - Minidash. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'minidash.html', RESTRICTED_PAGE_INFO)
        # Request - Index. Expect dashboard page.
        TestUtils.check_webpage_content(ip_address, '', INDEX_PAGE_INFO)
        TestUtils.check_webpage_content(ip_address, 'index.html', INDEX_PAGE_INFO)
        # Request - Mobile. Expect dashboard page.
        TestUtils.check_webpage_content(ip_address, 'mobile.html', MOBILE_INDEX_PAGE_INFO)
        # Request - Dashboard. Expect dashboard page.
        TestUtils.check_webpage_content(ip_address, 'welcome.html', DASHBOARD_PAGE_INFO)
        # Request - Settings. Expect settings.
        TestUtils.check_webpage_content(ip_address, 'cgi.html', SETTINGS_PAGE_INFO)
        # Request - Network. Expect network.
        TestUtils.check_webpage_content(ip_address, 'network.html', NETWORK_PAGE_INFO)
        print("Local page request test - OK!")

class ProvisionInfoTest_UI_Disabled(IProvisionInfoTest):
    def send_provision_info_test(self, id, ip_address):
        localName='Test UI Disabled Name'
        location='Test UI Disabled City'
        # TEST (1) - check that invalid ui modes id are rejected
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(-100000))
        if provision_info is 0:
            print('IoT device (%s, %s) provision info is invalid as expected.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 1).' % (id,ip_address))
        # TEST (2) - check that invalid ui modes id are rejected
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(44))
        if provision_info is 0:
            print('IoT device (%s, %s) provision info is invalid as expected.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 2).' % (id,ip_address))
        # TEST (3) - send valid ui mode
        provision_info=TestUtils.send_provision_info(ip_address,localName=localName,location=location,localUIMode=str(UI_MODE_DISABLE))
        if provision_info is 1:
            print('IoT device (%s, %s) provision info is valid.' % (id,ip_address))
        else:
            raise NameError('Failed to provision info of IoT (%s, %s) device (test 3).' % (id,ip_address))

    def request_local_page_test(self, id, ip_address):
        # Request - Provision. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'provision.html', RESTRICTED_PAGE_INFO)
        # Request - Minidash. Expect restricted.
        TestUtils.check_webpage_content(ip_address, 'minidash.html', RESTRICTED_PAGE_INFO)
        # Request - Index. Expect dashboard page.
        TestUtils.check_webpage_content(ip_address, '', RESTRICTED_PAGE_INFO)
        TestUtils.check_webpage_content(ip_address, 'index.html', RESTRICTED_PAGE_INFO)
        # Request - Mobile. Expect dashboard page.
        TestUtils.check_webpage_content(ip_address, 'mobile.html', RESTRICTED_PAGE_INFO)
        # Request - Dashboard. Expect dashboard page.
        TestUtils.check_webpage_content(ip_address, 'welcome.html', RESTRICTED_PAGE_INFO)
        # Request - Settings. Expect settings.
        TestUtils.check_webpage_content(ip_address, 'cgi.html', RESTRICTED_PAGE_INFO)
        # Request - Network. Expect network.
        TestUtils.check_webpage_content(ip_address, 'network.html', RESTRICTED_PAGE_INFO)
        print("Local page request test - OK!")
