package com.utils.downloader.utils.cert;

import com.utils.downloader.utils.EHSS_Logger;
import com.utils.downloader.utils.EHSS_Utils;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.security.KeyStore;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;

import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;
import javax.net.ssl.X509TrustManager;


public class CaCertificate implements ICertificate {

    public static final String ID = "ca_cert";

    private TrustManagerFactory trustManagerFactory;
    private TrustManager[] trustAllCerts;// todo temp hack
    private InputStream stream;
    private boolean isValid = false;

    public CaCertificate(InputStream stream) {
        this.stream = stream;
    }

    public CaCertificate() {
    }

    @Override
    public void setStorePassword(String password){
    }

    public boolean store(FileOutputStream fos){
        try {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            EHSS_Utils.copyStream(stream, baos);
            byte[] bytes = baos.toByteArray();
            ByteArrayInputStream bais = new ByteArrayInputStream(bytes);
            KeyStore keyStore = getKeyStore(bais);
            if(keyStore != null) {
                isValid = createTrustFactory(keyStore);
                if(isValid){
                    bais.reset();
                    EHSS_Utils.copyStream(bais, fos);
                }
                return isValid;
            }
        }catch (Exception e){
            EHSS_Logger.debug(e.toString());
        }
        return false;
    }

    public boolean load(FileInputStream fis){
        try {
            KeyStore keyStore = getKeyStore(fis);
            if(keyStore != null) {
                isValid = createTrustFactory(keyStore);
                return isValid;
            }
        }catch (Exception e){
            EHSS_Logger.debug(e.toString());
        }
        return false;
    }

    private KeyStore getKeyStore(InputStream inputStream){
        try {
            CertificateFactory cf = CertificateFactory.getInstance("X.509");
            Certificate ca = cf.generateCertificate(inputStream);
            //System.out.println("ca=" + ((X509Certificate) ca).getSubjectDN());
            String keyStoreType = KeyStore.getDefaultType();
            KeyStore keyStore = KeyStore.getInstance(keyStoreType);
            keyStore.load(null, null);
            keyStore.setCertificateEntry("ca", ca);
            return keyStore;
        }catch (Exception e){
            EHSS_Logger.debug(e.toString());
        }
        return null;
    }

    private boolean createTrustFactory(KeyStore keyStore){
        // first make some don't care versions... todo - we don't want this usually...
        try {
            trustAllCerts = new TrustManager[] { 
                new X509TrustManager() {     
                    public java.security.cert.X509Certificate[] getAcceptedIssuers() { 
                        return new X509Certificate[0];
                    } 
                    public void checkClientTrusted( 
                        java.security.cert.X509Certificate[] certs, String authType) {
                        } 
                    public void checkServerTrusted( 
                        java.security.cert.X509Certificate[] certs, String authType) {
                    }
                }
            };
        } catch (Exception e) {
            EHSS_Logger.debug("ANY:" + e.toString());
        }
        
        try {
            String tmfAlgorithm = TrustManagerFactory.getDefaultAlgorithm();
            trustManagerFactory = TrustManagerFactory.getInstance(tmfAlgorithm);
            trustManagerFactory.init(keyStore);
            return true;
        } catch (Exception e) {
            EHSS_Logger.debug(e.toString());
        }




        return false;
    }

    public TrustManager [] getTrustManagers(){
        if(trustManagerFactory != null){
            return trustManagerFactory.getTrustManagers();
        }
        return null;
    }
/* Probably want to remove this  when sorted */
    public TrustManager [] getTrustAnyManagers(){
        if(trustAllCerts != null){
            return trustAllCerts;
        }
        return null;
    }

    public String getRaw(){
        return null;
    }

    public SSLContext getSSLContext(){
        return null;
    }

    public String getId(){
        return ID;
    }

    public boolean isValid(){
        return isValid;
    }
}
