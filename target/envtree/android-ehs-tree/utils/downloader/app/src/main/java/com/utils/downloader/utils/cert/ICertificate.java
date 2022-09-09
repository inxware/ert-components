package com.utils.downloader.utils.cert;

import java.io.FileInputStream;
import java.io.FileOutputStream;

import javax.net.ssl.SSLContext;

public interface ICertificate {
    boolean store(FileOutputStream fos);
    boolean load(FileInputStream fis);
    String getRaw();
    String getId();
    boolean isValid();
    void setStorePassword(String password);
    SSLContext getSSLContext();
}
