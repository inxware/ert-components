package com.utils.downloader.utils.cert;

import android.content.Context;

import com.utils.downloader.utils.EHSS_Logger;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.UnrecoverableKeyException;
import java.security.cert.CertificateException;

import javax.net.ssl.KeyManager;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;

public class ClientCertificate implements ICertificate {

    public static final String ID = "client_cert";
    public static final String CERTIFICATE_STREAM_PASSWORD = "";
    public static final String CERTIFICATE_SSL_CONTEXT_PASSWORD = "";
    public static final String CERTIFICATE_STORE_PASSWORD = "wYCs22**XX&agYTx!PKF*sl?SzHs93k";

    private SSLContext sslContext;
    private CaCertificate caCertificate;
    private InputStream stream;
    private String streamPassword = CERTIFICATE_STREAM_PASSWORD;
    private String sslContextPassword = CERTIFICATE_SSL_CONTEXT_PASSWORD;
    private String storePassword = CERTIFICATE_STORE_PASSWORD;
    private boolean isValid = false;

    public ClientCertificate(InputStream stream) {
        this.stream = stream;
    }

    public ClientCertificate() {
    }

    @Override
    public void setStorePassword(String password){
        if(password == null){
            EHSS_Logger.debug("ClientCertificate - Password cannot be NULL.");
        }
        storePassword = password;
    }

    public boolean store(FileOutputStream fos){
        try {
            KeyStore keyStore = KeyStore.getInstance("PKCS12");
            keyStore.load(stream, streamPassword.toCharArray());
            // store away the keystore
            keyStore.store(fos, storePassword.toCharArray());
            isValid = createSSLContext(keyStore);
            return isValid;
        }catch (NoSuchAlgorithmException e){
            EHSS_Logger.debug(e.toString());
        }catch (IOException e){
            EHSS_Logger.debug(e.toString());
        }catch (CertificateException e){
            EHSS_Logger.debug(e.toString());
        }catch (KeyStoreException e){
            EHSS_Logger.debug(e.toString());
        }
        return false;
    }

    public boolean load(FileInputStream fis){
        try {
            KeyStore keyStore = KeyStore.getInstance("PKCS12");
            keyStore.load(fis, storePassword.toCharArray());
            isValid = createSSLContext(keyStore);
            return isValid;
        }catch (CertificateException e){
            EHSS_Logger.debug(e.toString());
        }catch (IOException e){
            EHSS_Logger.debug(e.toString());
        }catch (NoSuchAlgorithmException e){
            EHSS_Logger.debug(e.toString());
        }catch (KeyStoreException e){
            EHSS_Logger.debug(e.toString());
        }
        return false;
    }

    private boolean createSSLContext(KeyStore keyStore) {
        try {
            KeyManagerFactory kmf = KeyManagerFactory.getInstance("X509");
            kmf.init(keyStore, sslContextPassword.toCharArray());
            KeyManager[] keyManagers = kmf.getKeyManagers();
            sslContext = SSLContext.getInstance("TLS");
            if(caCertificate != null){
                // TODO - Re enable proper cert checking:
                //sslContext.init(keyManagers, caCertificate.getTrustManagers(), null);
                sslContext.init(keyManagers, caCertificate.getTrustAnyManagers(), null);
            }else{
                sslContext.init(keyManagers, null, null);
            }
            return true;
        }catch (UnrecoverableKeyException e){
            EHSS_Logger.debug(e.toString());
        }catch (KeyManagementException e){
            EHSS_Logger.debug(e.toString());
        }catch (NoSuchAlgorithmException e){
            EHSS_Logger.debug(e.toString());
        }catch (KeyStoreException e){
            EHSS_Logger.debug(e.toString());
        }
        return false;
    }

    public void setCaCertificate(CaCertificate ca){
        this.caCertificate = ca;
    }

    public String getRaw(){
        if(sslContext != null){
            return sslContext.toString();
        }
        return null;
    }

    public SSLContext getSSLContext(){
        if(sslContext != null){
            return sslContext;
        }
        return null;
    }

    public String getId(){
        return ID;
    }

    public boolean isValid(){
        return isValid;
    }
}
