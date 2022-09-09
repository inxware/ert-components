package com.utils.downloader.utils.comms;

import com.utils.downloader.utils.cert.ICertificate;

public abstract class ServerRequest {

    public static Integer EXPECTED_UNKNOWN = -1;

    protected final IMessage request;
    protected ICertificate certificate = null;
    private Integer expected = EXPECTED_UNKNOWN;

    public ServerRequest(IMessage request){
        this.request = request;
    }

    public ServerRequest setExpected(Integer expected){
        this.expected = expected;
        return this;
    }

    public void setCertificate(ICertificate certificate){
        this.certificate = certificate;
    }

    public Integer getExpected(){ return expected; }

    public abstract IMessage requestPost();
}
