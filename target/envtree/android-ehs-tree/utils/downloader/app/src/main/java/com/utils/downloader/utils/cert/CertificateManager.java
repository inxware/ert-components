package com.utils.downloader.utils.cert;

import android.content.Context;

import com.utils.downloader.utils.EHSS_Logger;
import com.utils.downloader.utils.EHSS_Utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;

public class CertificateManager {

    private final HashMap<String, ICertificate> clientCertificates = new HashMap<>();
    private final HashMap<String, ICertificate> caCertificates = new HashMap<>();
    private static CertificateManager s_certificateManager = null;

    public static CertificateManager getInstance(){
        if(s_certificateManager == null){
            s_certificateManager = new CertificateManager();
        }
        return s_certificateManager;
    }

    private CertificateManager(){
    }

    public void addCaCert(Context context, String ip_address, String ca_cert_file){
        if(ca_cert_file == null || ca_cert_file.isEmpty()){
            EHSS_Logger.info("Cert file is empty.");
            return;
        }
        if(context == null){
            EHSS_Logger.info("Context is null.");
            return;
        }
        try {
            FileInputStream fis = new FileInputStream ( new File(ca_cert_file) );
            addCaCert(context, ip_address, fis);
            fis.close();
        } catch (IOException ioe) {
            EHSS_Logger.debug(ioe.toString());
        } catch (Exception e){
            EHSS_Logger.debug(e.toString());
        }
    }

    public void addCaCert(Context context, String ip_address, InputStream ca_cert){
        if(ca_cert == null){
            EHSS_Logger.info("Cert stream is null.");
            return;
        }
        if(context == null){
            EHSS_Logger.info("Context is null.");
            return;
        }
        try{
            String md5_name = EHSS_Utils.md5(ip_address+"ca");
            ICertificate certificate = new CaCertificate(ca_cert);
            FileOutputStream fos = context.openFileOutput(md5_name, Context.MODE_PRIVATE);
            if(fos != null && certificate.store(fos) && certificate.isValid()){
                caCertificates.put(md5_name, certificate);
            }else {
                EHSS_Logger.info("Failed to store ca certificate.");
            }
            fos.close();
        }catch (Exception e){
            EHSS_Logger.debug(e.toString());
        }
    }

    public CaCertificate getCaCert(Context context, String ip_address){
        CaCertificate certificate = null;
        try{
            String md5_name = EHSS_Utils.md5(ip_address+"ca");
            if(!caCertificates.containsKey(md5_name) && context != null) {
                FileInputStream fis = (fileExists(context, md5_name)) ? context.openFileInput(md5_name) : null;
                if(fis != null) {
                    ICertificate certificate1 = new CaCertificate();
                    if (certificate1.load(fis) && certificate1.isValid()) {
                        caCertificates.put(md5_name, certificate1);
                    }
                    fis.close();
                }
            }
            if(caCertificates.containsKey(md5_name)) {
                certificate = (CaCertificate)caCertificates.get(md5_name);
            }
        }catch (Exception e){
            EHSS_Logger.error(e.toString());
        }
        return certificate;
    }

    public void addClientCert(Context context, String ip_address, String client_cert_file){
        if(client_cert_file == null || client_cert_file.isEmpty()){
            EHSS_Logger.info("Cert file is empty.");
            return;
        }
        if(context == null){
            EHSS_Logger.info("Context is null.");
            return;
        }
        try {
            FileInputStream fis = new FileInputStream ( new File(client_cert_file) );
            addClientCert(context, ip_address, fis);
            fis.close();
        } catch (IOException ioe) {
            EHSS_Logger.debug(ioe.toString());
        } catch (Exception e){
            EHSS_Logger.debug(e.toString());
        }
    }

    public void addClientCert(Context context, String ip_address, InputStream client_cert){
        if(client_cert == null){
            EHSS_Logger.info("Cert stream is null.");
            return;
        }
        if(context == null){
            EHSS_Logger.info("Context is null.");
            return;
        }
        try{
            String md5_name = EHSS_Utils.md5(ip_address);
            ClientCertificate certificate = new ClientCertificate(client_cert);
            CaCertificate ca = getCaCert(context, ip_address);
            if(ca != null){
                certificate.setCaCertificate(ca);
            }
            FileOutputStream fos = context.openFileOutput(md5_name, Context.MODE_PRIVATE);
            if(fos != null && certificate.store(fos) && certificate.isValid()){
                clientCertificates.put(md5_name, certificate);
            }else {
                EHSS_Logger.info("Failed to store certificate.");
            }
            fos.close();
        }catch (Exception e){
            EHSS_Logger.debug(e.toString());
        }
    }

    public ClientCertificate getClientCert(Context context, String ip_address){
        ClientCertificate certificate = null;
        try{
            String md5_name = EHSS_Utils.md5(ip_address);
            if(!clientCertificates.containsKey(md5_name) && context != null) {
                FileInputStream fis = (fileExists(context, md5_name)) ? context.openFileInput(md5_name) : null;
                if(fis != null) {
                    ClientCertificate certificate1 = new ClientCertificate();
                    CaCertificate ca = getCaCert(context, ip_address);
                    if(ca != null){
                        certificate1.setCaCertificate(ca);
                    }
                    if (certificate1.load(fis) && certificate1.isValid()) {
                        clientCertificates.put(md5_name, certificate1);
                    }
                    fis.close();
                }
            }
            if(clientCertificates.containsKey(md5_name)) {
                certificate = (ClientCertificate)clientCertificates.get(md5_name);
            }
        }catch (Exception e){
            EHSS_Logger.error(e.toString());
        }
        return certificate;
    }

    public boolean fileExists(Context context, String filename) {
        File file = context.getFileStreamPath(filename);
        if(file == null || !file.exists()) {
            return false;
        }
        return true;
    }
}
