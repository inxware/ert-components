package com.inx.ehssupervisor.utils.comms;

public abstract class ServerRequest {

    public static Integer EXPECTED_UNKNOWN = -1;

    protected final IMessage request;
    private Integer expected = EXPECTED_UNKNOWN;

    public ServerRequest(IMessage request){
        this.request = request;
    }

    public ServerRequest setExpected(Integer expected){
        this.expected = expected;
        return this;
    }

    public Integer getExpected(){ return expected; }

    public abstract IMessage requestPost();
}
