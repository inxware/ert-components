from flask import Flask
from flask import jsonify, request, abort
from zeroconf import ServiceInfo, Zeroconf
import sys, socket, time

# Script for simulating an MDNS enabled device publishing provisioning service to an app on the local network
# Usage: 
# iot-mdns.py <device ID> <device IP address> <provisioning port> <provisioning status> <organisation ID>
#

PORT=8080
MDSN_NAME="mDnsInxIoT "
global iot_port
iot_port=PORT
global iot_status
iot_status=None
global iot_organisation
iot_organisation='-1'

app = Flask(__name__)

@app.route('/post_mobile_status.cgi', methods=['GET','POST'])
def datarequest():
    data=request.json
    print(data)
    response={}
    if str(iot_status) == 'provisioned':
        response['status']=1
        response['localName']='IOT Test'
        response['orgID']=iot_organisation
    elif str(iot_status) == 'unprovisioned':
        response['status']=0
    else:
        pass
    print(response)
    return jsonify(response)

def setup_mdns(id, ip, port):
    protocol="_http._tcp.local."
    mdns_string="%s%s.%s" % (MDSN_NAME,id,protocol)
    print("mdns : %s" % mdns_string)
    desc = {}
    return ServiceInfo( protocol, mdns_string, socket.inet_aton(ip),
                        port, 0, 0, desc)

if __name__ == '__main__':
    if len(sys.argv) > 5:
        iot_id=sys.argv[1:][0]
        iot_ip=sys.argv[2:][0]
        iot_port=int(sys.argv[3:][0])
        iot_status=sys.argv[4:][0]
        iot_organisation=sys.argv[5:][0]
    else:
        raise NameError("IoT ID must be specified as an argument.")

    print("Setting-up mdns for iot device [id=%s] [ip=%s:%s] [status=%s] [orgID=%s]" % (iot_id,iot_ip,iot_port,iot_status,iot_organisation))
    info=setup_mdns(iot_id,iot_ip,iot_port)
    zeroconf = Zeroconf()
    zeroconf.register_service(info)
    try:
        app.run(host='0.0.0.0', port=iot_port)
    except KeyboardInterrupt:
        print("Stopping application ...")
    finally:
        print("Unregistering...")
        zeroconf.unregister_service(info)
        zeroconf.close()
