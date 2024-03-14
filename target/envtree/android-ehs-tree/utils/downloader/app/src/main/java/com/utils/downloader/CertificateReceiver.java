package com.utils.downloader;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.utils.downloader.utils.cert.CertificateManager;
import com.utils.downloader.utils.EHSS_Logger;

/* This class Allows new certificates to be broadcast (usuallly by the supervisor during boot)

todo2024:
This is insecure and very convoluted. It should be removed ASAP
-> we would be better off reading them from a file at installation time and then deleting them.
-> Only further updates the server certificates should be possible via a secure connection to devman or root access to the device via ADB)

*/

public class CertificateReceiver extends BroadcastReceiver {

    private static final String IP_ADDRESS = "ip_address";
    private static final String CA_CERT_FILE = "ca_cert";
    private static final String CLIENT_CERT_FILE = "client_cert";

    private static boolean IsNull(String str){
        return str == null || str.isEmpty();
    }

    @Override
    public void onReceive(Context context, Intent intent) {
         try {
             // add certificates
             final String ip_address = intent.getStringExtra(IP_ADDRESS);
             if(!IsNull(ip_address)) {
                 final String client_cert_file = intent.getStringExtra(CLIENT_CERT_FILE);
                 final String ca_cert_file = intent.getStringExtra(CA_CERT_FILE);
                 final Context final_context = context;
                 new Thread() {
                     @Override
                     public void run() {
                        /* These steps create the random number files in the app's normal filesapce not a secure areaa */
                         if(!IsNull(ca_cert_file)) {
                             EHSS_Logger.info("Storing ca certificates.");
                             CertificateManager.getInstance().addCaCert(final_context, ip_address, ca_cert_file);
                         }
                         if(!IsNull(client_cert_file)) {
                             EHSS_Logger.info("Storing client certificates.");
                             CertificateManager.getInstance().addClientCert(final_context, ip_address, client_cert_file);
                         }
                     }
                 }.start();
             }else{
                 EHSS_Logger.info("Failed to add certificates. Address must be specified.");
             }
        }catch(Exception e){
            EHSS_Logger.error(e.toString());
        }
    }
}
