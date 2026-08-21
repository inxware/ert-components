/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

//#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-inx-netsocket.h"
#include "hal_network.h"


#define _netsock_debug_printf {}//printf

//todo2022 remove these heade requirements and use the target component HAL...
#ifndef EHS_MINGW
// #define _GNU_SOURCE
#ifndef EHS_LWIP
#include <poll.h>
#endif // EHS_LWIP
#else
#include <winsock2.h>
#endif
#include "ehs_fb_types.h"
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct
{
    ehs_bool   bIsOpening; // we are intending to open this so stop any other attempts. Reset when connection is closed by client to server.
    ehs_sint32 port;
    ehs_sint32 tcp_udp;
    ehs_uint32 read_dwell_time_ms;
    ehs_uint32 data_send_size; /* amount of data to send to a port */
    ehs_uint32 data_recv_size; /* amount of data received over a socket */

    //EhsTgtTcpSocketType listen_socket; /*@todo remove */
    EhsTgtTcpSocketType connection_socket; /* This is the socket descriptor */
    EhsTgtTcpSockAddrInType * connection_addr; /* This is the address list struct for the target server*/
    // EhsTgtTcpSockAddrInType * negotiated_addr; /* This is the Local? address struct negotiated  from the list in connection_addr */
    //ehs_bool stopListeningNow; //flag this when you want to stop the server.
    ehs_bool closeConnectionNow; //flag this when you want to stop a connection (server or client mode) is set to 0 when actually closed.

    ehs_char data_recv_buf[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!!! These could be very large with pass through stuff enabled?
    ehs_char data_send_buf[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!!! These could be very large with pass through stuff enabled? Can we just make these bigger?
    ehs_char URL[EHS_STRING_LENGTH_MAX];  //TODO:STRINGLENGTH!
} inx_netSocket_state_type; //Reference this, maybe store your config parameters in here too.


//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(netSocket)

EHS_FB_FUNCTION_ENTRY("open", 0x01, netSocket_open)

EHS_FB_FUNCTION_ENTRY("close", 0x02, netSocket_close)

EHS_FB_FUNCTION_ENTRY("send", 0x03, netSocket_send)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_netSocket_ARG_open_port 1
#define INX_netSocket_ARG_open_url 2
#define INX_netSocket_ARG_open_errnoerrorno 1
#define INX_netSocket_ARG_open_data 2
#define INX_netSocket_ARG_open_sizeout_size 3
#define INX_netSocket_ARG_open_opened 3
#define INX_netSocket_ARG_open_erroropened_error 4 // error on open
#define INX_netSocket_ARG_open_received 5
#define INX_netSocket_ARG_open_closed 6
#define INX_netSocket_ARG_open_sent 7
#define INX_netSocket_ARG_open_send_error 2
#define INX_netSocket_ARG_open_errorclose_error 1 // error on close
#define INX_netSocket_ARG_send_datadata_in 1
#define INX_netSocket_ARG_send_errnoerrorno 1
#define INX_netSocket_ARG_send_size 2
#define INX_netSocket_ARG_send_sent 2
#define INX_netSocket_ARG_send_error 1
#define INX_netSocket_ARG_close_errorclose_error 1
#define INX_netSocket_ARG_close_closed 2
#define INX_netSocket_ARG_close_errnoerrorno 1
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER



/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_netSocket_URL
#define INX_FB_netSocket_port 8000
#define INX_FB_netSocket_tcp_udp 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(netSocket)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_netSocket_state_type);
}

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

void EhsNetSocketClientInitDynamic(inx_netSocket_state_type* inx_netSocket_state)
{
    inx_netSocket_state->data_recv_buf[0]='\0';
    inx_netSocket_state->data_send_buf[0]='\0';
    memset(inx_netSocket_state->data_send_buf,0,EHS_STRING_LENGTH_MAX);
    inx_netSocket_state->data_recv_size=0;
    inx_netSocket_state->data_send_size=0;
    inx_netSocket_state->connection_addr=NULL; // only used for UDP connections/sendTo
    //inx_netSocket_state->negotiated_addr=NULL; // only used for sendTo (from linked list)
    inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;
    inx_netSocket_state->closeConnectionNow = EHS_FALSE;
    inx_netSocket_state->bIsOpening = EHS_FALSE; // reset this so no new threads can start
}


EHS_FB_INIT_FUNCTION(netSocket)
{

    ehs_sint32 port;
    ehs_sint32 tcp_udp;
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_netSocket_state_type* inx_netSocket_state = (inx_netSocket_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters - the %s carries a field width from
     * sizeof, or sscanf writes as many characters as the parameter holds */
    {
        ehs_char szScanFmt[32];
        EhsSnprintf(szScanFmt, sizeof(szScanFmt), "%%%us %%d %%d",
                    (unsigned int)(sizeof(inx_netSocket_state->URL) - 1u));
        EhsSscanf(EHS_FB_INIT_PARAMETERS,szScanFmt,inx_netSocket_state->URL,&port,&tcp_udp);
    }
    inx_netSocket_state->port=port;
    inx_netSocket_state->tcp_udp=tcp_udp;
    inx_netSocket_state->read_dwell_time_ms=10; //default to 10ms (~10MBs), @todo make this a parameter to govern data rate.
    EhsNetSocketClientInitDynamic(inx_netSocket_state); // reset everything
    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}


//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(netSocket)
{
    inx_netSocket_state_type *inx_netSocket_state = (inx_netSocket_state_type*)EHS_FB_DESTROY_CONTEXT;
    
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    inx_netSocket_state->closeConnectionNow = EHS_TRUE;
    if (inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET )
    {
        EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket); // unblock the accept or receive calls
        inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;
    }
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

/* This thread is started by  open and does all session management.
 * It can receive connection data in tcp mode or just listen for udp packets in UDP mode.
 * it is stopped when the close falg is set: i.e. either a connection close or just stops listening.
 */

#ifdef EHS_MINGW
int inet_aton(const char *cp, struct in_addr *addr)
{
    addr->s_addr = inet_addr(cp);
    return (addr->s_addr == INADDR_NONE) ? 0 : 1;
}
#endif
/**
 * Send data from EHS to the client, tcp or udp - depending on socket params and destination address.
 *
 * @return true if we remain connected to the target
 *
 * !!!! Runs in the Open function's context so use Open macros !!!!!!!!!!
 */
EHS_FB_RUN_FUNCTION(netSocket_send_actual)
{
    inx_netSocket_state_type* inx_netSocket_state = (inx_netSocket_state_type*) EHS_FB_RUN_CONTEXT;
    ehs_bool bConnected = EHS_TRUE; /* assume that we stay connected */
    ssize_t data_size = 0 ;//@todo this thould be ehsified for winsock

    if (inx_netSocket_state->connection_socket == EHS_TGT_TCP_INVALID_SOCKET)
    {
        EHSH_LOG_ERROR("socket not created for tcpip - not trying");
        goto error_notopen;
    }
    /***** send bBuff to TCP/IP ****/
    /* @todo we do this in blocking mode, which would stop reading for large data, This could be done non-blocking, by keeping tabs on the amount sent each time? */
    if (inx_netSocket_state->tcp_udp == 0)   /* TCPIP */
    {
        data_size = send(inx_netSocket_state->connection_socket,(const ehs_char *) inx_netSocket_state->data_send_buf,(ehs_sint32) inx_netSocket_state->data_send_size,(ehs_sint32) 0); /* Send connection in blocking mode */
    }
    else     /* UDP mode use sendto */
    {
        /* @todo this might need the selected addrinfo rather than the list version */
        struct sockaddr_in si_other;
        memset((char *) &si_other, 0, sizeof(si_other));
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(inx_netSocket_state->port);
        if (inet_aton(inx_netSocket_state->URL, &si_other.sin_addr) == 0)
        {
            EHSH_LOG_ERROR ("inet_aton() failed");
            goto error_notopen;
        }
        int addr_size = sizeof(si_other);
        data_size = sendto(inx_netSocket_state->connection_socket,
                           (const ehs_char *) inx_netSocket_state->data_send_buf,
                           (ehs_sint32) inx_netSocket_state->data_send_size,
                           (ehs_sint32) 0,
                           (const struct sockaddr *) &si_other, addr_size);
        /*
        			_netsock_debug_printf("[%d],[%s],[%d],[%d],[%x],[%d] ERRONO=%s\n",data_size,
        					(const ehs_char *) inx_netSocket_state->URL,
        					(ehs_sint32) inx_netSocket_state->port,
        					(ehs_sint32) 0,
        					(const struct sockaddr *)  &si_other,
        					addr_size,strerror(errno));
        */
    }
    if (data_size != inx_netSocket_state->data_send_size)
    {
        EHSH_LOG_ERROR("socket send error size sent {%s} [UDP=%d] =%d, measured=%d",
                       inx_netSocket_state->data_send_buf,
                       inx_netSocket_state->tcp_udp,
                       (ehs_sint32)inx_netSocket_state->data_send_size,
                       (ehs_sint32)data_size);
        inx_netSocket_state->data_send_size =0;
        goto error_send;
    }
    else     /* Data Sent OK */
    {
        inx_netSocket_state->data_send_size = 0;
        goto good;
    }

error_notopen:
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    if (inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket);
        inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;
    }
    inx_netSocket_state->closeConnectionNow = EHS_TRUE;
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EHS_FB_FINISH_API2(INX_netSocket_ARG_open_send_error );
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    goto exitfunc;

error_send:
    /* todo we don't set an error number here */
    EHSH_LOG_ERROR("Setting close connection flag");
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    if (inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket);
        inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;
    }
    inx_netSocket_state->closeConnectionNow = EHS_TRUE;
    EhsTPMutex_unlock(EhsTPMutex_socketClient);

    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EHS_FB_FINISH_API2(INX_netSocket_ARG_open_send_error );
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    goto exitfunc;
good:
    /* Data has been written to the socket port, now assert the event */
    EHSH_LOG_INFO("Socket packet sent OK");
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EHS_FB_FINISH_API2(INX_netSocket_ARG_open_sent);//INX_netSocket_ARG_send_sent);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);

exitfunc:

    ;
}

//#define EHS_SKT_DEBUG
#ifdef EHS_SKT_DEBUG
typedef struct sockaddr_in       sockaddr_in_t;
typedef struct sockaddr_in6      sockaddr_in6_t;
static const char *pgmName;           /* Program name (w/o directory). */
#endif




/* This is the main thread that stays open for the duration the socket is opened */
EHS_FB_THREAD_FUNCTION(netSocket_receive)
{
#ifdef EHS_SKT_DEBUG
    sockaddr_in6_t  *pSadrIn6;
    int i=0;
#endif
//@todo this is posix only
#ifndef EHS_MINGW
    struct pollfd  pfd;
#else
    int nfds;
    fd_set readfds;
    struct timeval tv;
    int rv;
#endif

    ehs_sint32 sckt_err = 0;
    ehs_bool start_reading = EHS_FALSE;
    ehs_char port_string[32];
    inx_netSocket_state_type* inx_netSocket_state = (inx_netSocket_state_type*) EHS_FB_RUN_CONTEXT;
    /* todo2023 - try to avoid using this on the stack */
    ehs_uint8 bBuffIn[EHS_TGT_TCP_IN_BUFF_SIZE]; /* buffer for incoming data */
    Ehs_FB_ThreadStarted();
    ehs_sint32 nDataReceived;
    /* amount of data received by TCP/IP */
    ehs_uint8 *pData = bBuffIn;
    //ehs_bool bDisconnect = EHS_FALSE; /* assume we want to stay connected */
    EhsTgtTcpSockAddrInType remote,hint, *rp;

    memset(&hint, 0, sizeof(hint));
    if (inx_netSocket_state->tcp_udp == 0)   // * TCPIP * /
    {
        hint.ai_socktype = SOCK_STREAM;
        hint.ai_protocol = IPPROTO_TCP; // 0 is any protocol
    }
    else
    {
        hint.ai_socktype = SOCK_DGRAM;
        hint.ai_protocol = 0;// is any protocol
    }
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    inx_netSocket_state->closeConnectionNow = EHS_FALSE;
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    EhsSprintf(port_string, "%d", inx_netSocket_state->port);/*todo put max width stuff in here */
    hint.ai_family = PF_INET;//PF_UNSPEC; //@todo this should be a parameter
    /* The following could be used but needs a hint struct without the name included and a return version. */
    int err;

    EhsTPMutex_lock(EhsTPMutex_socketClient);
    err = getaddrinfo(inx_netSocket_state->URL, port_string, &hint,&inx_netSocket_state->connection_addr); //  this doesn't like ANY other FDs being double closed and will exit the whole EHS if this happens while it is running
    EhsTPMutex_unlock(EhsTPMutex_socketClient);

    if (0 != err)
    {
        EHSH_LOG_ERROR("getaddrinfo failed for %s:%s connection error = %d",inx_netSocket_state->URL,port_string,err);
        goto connect_error; //@todo could do with some diagnostic here
    }
    /* Create EhsTgtTcpSocketType */
    /* One last Check - we shouldn't be here with an open socket, but then...*/
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    if (inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        EHSH_LOG_ERROR("Already connected to socket - Disconecting and reconnecting");
        EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket);
        inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;
    }
    EhsTPMutex_unlock(EhsTPMutex_socketClient);
    //inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;
    inx_netSocket_state->data_send_size = 0;/* Just in case this is left untidy */

    for (rp = inx_netSocket_state->connection_addr; rp != NULL; rp = rp->ai_next)
    {
        inx_netSocket_state->connection_socket = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol); /* Open Socket */
        //inx_netSocket_state->connection_socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP); /* Open Socket */
        if (inx_netSocket_state->connection_socket == EHS_TGT_TCP_INVALID_SOCKET || inx_netSocket_state->connection_socket == EHS_TGT_TCP_SOCKET_ERROR   )
        {
            EhsTPMutex_lock(EhsTPMutex_socketClient);
            if (inx_netSocket_state->connection_socket == EHS_TGT_TCP_SOCKET_ERROR   )
            {
                EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket); // only try to close if an error - not invalid as this might be a double close...
            }
            inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;
            EhsTPMutex_unlock(EhsTPMutex_socketClient);
            continue;
        }
        /* Socket must be OK if we're here*/
        if (inx_netSocket_state->tcp_udp == 1)   /* UDP mode  so don't connect*/
        {
            start_reading = EHS_TRUE;
            EHSH_LOG_INFO("Made UDP socket OK");
            EhsTPMutex_lock(EhsTPMutex_fbIO);
            EHS_FB_FINISH_API2(INX_netSocket_ARG_open_opened);
            EhsTPMutex_unlock(EhsTPMutex_fbIO);
            break;
        }
        else
        {
            if (connect(inx_netSocket_state->connection_socket,(struct sockaddr *) rp->ai_addr,rp->ai_addrlen) != 0)
            {
                //the following is done in connect_error
                //EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket);
                //inx_netSocket_state->connection_socket=EHS_TGT_TCP_INVALID_SOCKET;
                EHSH_LOG_ERROR("Could not create TCPIP connection to port %d - %s", inx_netSocket_state->port, strerror(errno));
                goto connect_error;
            }
            else
            {
                /* Opened socket OK */
                start_reading = EHS_TRUE;
                EhsTPMutex_lock(EhsTPMutex_fbIO);
                EHS_FB_FINISH_API2(INX_netSocket_ARG_open_opened);
                EhsTPMutex_unlock(EhsTPMutex_fbIO);

                break; /* exit loop and listen for data */
            }
        }
    }

    if (inx_netSocket_state->connection_socket == EHS_TGT_TCP_INVALID_SOCKET)
    {
        EHSH_LOG_ERROR("interface address didn't connect, closing the port");
        sckt_err = -1; //Could not open socket.
        goto connect_error;
    }
    /* Check if we exited with an open socket */
    if (!start_reading)   //inx_netSocket_state->connection_socket == EHS_TGT_TCP_INVALID_SOCKET) {
    {
        sckt_err = -1; //Could not open socket.
        EHSH_LOG_ERROR("Could not connect");
        goto connect_error;
    }
//@todo posix only bit should be macro'd
    //@todo shold write an abstracted simple poll with TO.
#ifndef EHS_MINGW
    pfd.fd = inx_netSocket_state->connection_socket;
    pfd.events =  POLLHUP | /*POLLRDHUP |*/ POLLERR |POLLNVAL ; /* check for hang up - not POLLIN |  as we want to read to*/
    pfd.revents = 0;
    struct timeval timeout;
    timeout.tv_sec=0;
    timeout.tv_usec=10000;

    // @TODO - make the send timeout a FB parameter or non-blocking
    struct timeval snd_timeout;
    if (inx_netSocket_state->tcp_udp == 1) 
    {
        // udp send timeout
        snd_timeout.tv_sec=5;
        snd_timeout.tv_usec=0;
    }
    else
    {
        // tcp send timeout
        snd_timeout.tv_sec=30;
        snd_timeout.tv_usec=0;
    }

    //if (setsockopt(inx_netSocket_state->connection_socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tvx,sizeof(struct timeval))
    if (inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        // socket recive timeout
        int err = setsockopt(inx_netSocket_state->connection_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        if (err)
        {
            EHSH_LOG_ERROR("clinet setsockopt SO_RCVTIMEO failed err=%d", err);
        }

        // socket send timeout
        err = setsockopt(inx_netSocket_state->connection_socket, SOL_SOCKET, SO_SNDTIMEO, &snd_timeout, sizeof(snd_timeout));
        if (err) 
        {
            EHSH_LOG_ERROR("clinet setsockopt SO_SNDTIMEO failed err=%d", err);
        }
    }
#else
    DWORD timeout=1;
    nfds = inx_netSocket_state->connection_socket+1;
    if (inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        if (setsockopt(inx_netSocket_state->connection_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(DWORD)))
        {
            EHSH_LOG_ERROR("setsockopt failed");
        }
    }
#endif
    /* we're good so poll for data (at a limited rate).*/
    while (inx_netSocket_state->closeConnectionNow == EHS_FALSE)
    {
        /* Send any data we have in the buffer */
        if (inx_netSocket_state->data_send_size > 0)
        {
            EHS_FB_START_RUN_FUNCTION(netSocket_send_actual);
        }
        if(inx_netSocket_state->closeConnectionNow==EHS_TRUE){
            EHSH_LOG_ERROR("Close requested in the thread!");
        }
        inx_netSocket_state->data_send_size=0;
        if (inx_netSocket_state->tcp_udp == 0)
        {

#ifndef EHS_MINGW

            nDataReceived = recv(inx_netSocket_state->connection_socket,inx_netSocket_state->data_recv_buf, EHS_STRING_LENGTH_MAX-1, 0x40 /*MSG_DONT_WAIT */); // Get up to the largest amount of data

#else
            nDataReceived = recv(inx_netSocket_state->connection_socket,inx_netSocket_state->data_recv_buf, EHS_STRING_LENGTH_MAX-1, 0x00 /*MSG_DONT_WAIT */); // Get up to the largest amount of data


#endif
            if (nDataReceived == EHS_TGT_TCP_SOCKET_ERROR && errno != EAGAIN && errno != 0)   // The socket has probably closed on us so exit and clean up
            {
                EHSH_LOG_ERROR("SOCKETERROR WHILE WAITING (%d=%d)",nDataReceived,EHS_TGT_TCP_SOCKET_ERROR);
                break;
            }
        }
        else     /* for UDP */
        {
            int slen = sizeof(rp);
            //_netsock_debug_printf("----> UDP recv \n");
            memset(&remote, 0, sizeof(remote));
#ifndef EHS_MINGW
            nDataReceived = recvfrom(inx_netSocket_state->connection_socket,(ehs_char*) inx_netSocket_state->data_recv_buf,EHS_STRING_LENGTH_MAX-1, 0x40,(struct sockaddr*) &remote,&slen); // Get up to the largest amount of data
#else
            nDataReceived = recvfrom(inx_netSocket_state->connection_socket,(ehs_char*) inx_netSocket_state->data_recv_buf,EHS_STRING_LENGTH_MAX-1, 0x00,(struct sockaddr*) NULL,0); // Get up to the largest amount of data
#endif

            //_netsock_debug_printf("<---- UDP recv \n");
        }
        if (nDataReceived > 0) /* did we receive data?  Write it to the output buffer with a terminiator and number of bytes*/
        {
            EhsTPMutex_lock(EhsTPMutex_fbIO);
            /* note data received should be EHS_STRING_LENGTH-1 from recv() */
            inx_netSocket_state->data_recv_buf[nDataReceived]='\0'; // just in case the user treats this as a NULL term string.
            if (EHS_FB_OUT_CONNECTED_API2(INX_netSocket_ARG_open_data)){
                EHS_FB_OUT_S_SETN_API2(INX_netSocket_ARG_open_data, inx_netSocket_state->data_recv_buf, nDataReceived+1);
                //((ehs_char*) (EHS_FB_OUT_S_API2(INX_netSocket_ARG_open_data)))[nDataReceived] = '\0';
            }
            if (EHS_FB_OUT_CONNECTED_API2(INX_netSocket_ARG_open_sizeout_size)){
                EHS_FB_OUT_I_API2(INX_netSocket_ARG_open_sizeout_size) = nDataReceived ;
            }
            EHS_FB_FINISH_API2(INX_netSocket_ARG_open_received);
            EhsTPMutex_unlock(EhsTPMutex_fbIO);
            /*  "need to wait here for the data read flag before reading any more.*/
        }
        else
        {
        }

        /* Throttle read rate using configured parameter Note using poll timeout and not sleep for posix*/
#ifndef EHS_MINGW //@todo MINGW is not a good ting here - we should have #ifdef POSIX
        if (poll(&pfd, 1, inx_netSocket_state->read_dwell_time_ms) > 0)
        {
            //Socket is closed by remote server. exit
            EhsTPMutex_lock(EhsTPMutex_socketClient);
            inx_netSocket_state->closeConnectionNow=EHS_TRUE;
            EhsTPMutex_unlock(EhsTPMutex_socketClient);
        }
#else
        tv.tv_sec = 0;
        tv.tv_usec = 100*inx_netSocket_state->read_dwell_time_ms;
        rv = select(nfds, &readfds, NULL, NULL,&tv);  // todo this fails with -1 - unknown issue
        switch (rv)
        {
        case -1: // error
            EhsSleepUs (1000*inx_netSocket_state->read_dwell_time_ms); // stop misery a bit longer
            //inx_netSocket_state->closeConnectionNow=EHS_TRUE;
            break;
        case 0: // timed out sd expected if no data
            break;
        default:
            // loop round and read it (but less of a gap... (todo)
            EhsSleepUs (100*inx_netSocket_state->read_dwell_time_ms);
            break;
        }
#endif

        if (*bNewSodlFlagRef == EHS_TRUE)
        {
            break; /* Abandon ship */
        }
    }// end of open loop
    EHSH_LOG_INFO("All done: closing flag=%d Closing connection",inx_netSocket_state->closeConnectionNow);
    goto good;

connect_error:
    /* Assume the connection socket is already set to EHS_TGT_TCP_INVALID_SOCKET, but just in case close it anyway */
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    if (inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket);
        inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;
    }
    EhsTPMutex_unlock(EhsTPMutex_socketClient);

    if (inx_netSocket_state->connection_addr)
    {
        freeaddrinfo(inx_netSocket_state->connection_addr);
    }
    inx_netSocket_state->connection_addr = NULL;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EHS_FB_OUT_I_API2(INX_netSocket_ARG_open_errnoerrorno) = sckt_err;
    EHS_FB_FINISH_API2(INX_netSocket_ARG_open_erroropened_error);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    goto exitfunc;

good:
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    if ( inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket);
        inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET; /* This is the state machine flag too */
    }
    EhsTPMutex_unlock(EhsTPMutex_socketClient);

    if (inx_netSocket_state->connection_addr)
        freeaddrinfo(inx_netSocket_state->connection_addr);
    inx_netSocket_state->connection_addr = NULL;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EHS_FB_OUT_I_API2(INX_netSocket_ARG_open_errnoerrorno) = 0;
    EHS_FB_FINISH_API2(INX_netSocket_ARG_open_closed);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);

exitfunc:
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    inx_netSocket_state->closeConnectionNow = EHS_FALSE; //signal we have closed.
    EhsNetSocketClientInitDynamic( inx_netSocket_state); // reset evrything for next time
    inx_netSocket_state->bIsOpening = EHS_FALSE;
    EhsTPMutex_unlock(EhsTPMutex_socketClient);

    Ehs_FB_ThreadComplete();
    EhsHThread_exit()
    ;
}

//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB FUNCTION open MACRO START -- DO NOT ALTER
/**
 * Definition of netSocket_open.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(netSocket_open)
{
    inx_netSocket_state_type* inx_netSocket_state = (inx_netSocket_state_type*) EHS_FB_RUN_CONTEXT;
    ehs_bool OK = EHS_TRUE;
    //EhsTgtTcpSockAddrInType connection; /* used to construct the address of the EhsTgtTcpSocketType we're creating */
    ehs_bool ret = EHS_FALSE; /* assume initialisation fails */
    ehs_uint8 retVal;
    /*TCPIP MODE */
    /* same for both: if (inx_netSocket_state->tcp_udp == EHS_FALSE) {*/
    /* Check if we are closed */
    if (inx_netSocket_state->bIsOpening == EHS_TRUE || inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        /* Need to be closed first, report state machine error */
        //if (EHS_FB_OUT_CONNECTED_API2(INX_netSocket_ARG_open_errnoerrorno))
        EhsTPMutex_lock(EhsTPMutex_fbIO);
        EHS_FB_OUT_I_API2(INX_netSocket_ARG_open_errnoerrorno) = -101; //-101 is an open error
        EhsTPMutex_unlock(EhsTPMutex_fbIO);

        EHSH_LOG_ERROR("Socket is already active - must be closed first (close connection state=%d)", inx_netSocket_state->closeConnectionNow);

        goto error;
    }
    else
    {
        EhsTPMutex_lock(EhsTPMutex_fbIO);
        if (EHS_FB_IN_CONNECTED_API2(INX_netSocket_ARG_open_url))
        {
            EhsStrcpy(inx_netSocket_state->URL,EHS_FB_IN_S_API2(INX_netSocket_ARG_open_url));
        }
        if (EHS_FB_IN_CONNECTED_API2(INX_netSocket_ARG_open_port))
        {
            inx_netSocket_state->port=EHS_FB_IN_I_API2(INX_netSocket_ARG_open_port);
        }
        EhsTPMutex_unlock(EhsTPMutex_fbIO);
        EhsTPMutex_lock(EhsTPMutex_socketClient);
        inx_netSocket_state->bIsOpening = EHS_TRUE;
        EhsTPMutex_unlock(EhsTPMutex_socketClient);
        /* connect to server and start listening for data and start function to send data*/
        EHSH_LOG_INFO("Opening Socket...");
        EHS_FB_START_THREAD(netSocket_receive,-90);
    }
    /* send nothing here, leave the thread to do so */
    return;
error:
    /*
     if (EHS_FB_OUT_CONNECTED_API2(INX_netSocket_ARG_close_errnoerrorno))
     EHS_FB_OUT_I_API2(INX_netSocket_ARG_close_errnoerrorno) = -110;*/
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EHS_FB_FINISH_API2(INX_netSocket_ARG_open_erroropened_error);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);

}

//need to check for double closes on any file descriptor - socket or other..

//ICB FUNCTION open MACRO END -- DO NOT ALTER
//ICB FUNCTION close MACRO START -- DO NOT ALTER
/**
 * Definition of netSocket_close.
 * Signal the open thread to exit
 */
EHS_FB_RUN_FUNCTION(netSocket_close)
{
    inx_netSocket_state_type* inx_netSocket_state = (inx_netSocket_state_type*) EHS_FB_RUN_CONTEXT;
    // Your code here
    EhsTPMutex_lock(EhsTPMutex_socketClient);
    if ( inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        // this gets called from the socket thread
        //EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket); // unblock anything lingering
        //inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET; // we should set this so any other threads don't try to close it again..
        inx_netSocket_state->closeConnectionNow = EHS_TRUE; //signal to close - but  only the
        EhsTPMutex_unlock(EhsTPMutex_socketClient);
    }
    else
    {
        
        inx_netSocket_state->closeConnectionNow = EHS_TRUE; //just in case we have a thread still running.
        EHSH_LOG_ERROR("Failed to close network socket state (%d)", inx_netSocket_state->closeConnectionNow);
        EhsTPMutex_unlock(EhsTPMutex_socketClient);
        
        EhsTPMutex_lock(EhsTPMutex_fbIO);
        EHS_FB_FINISH_API2(INX_netSocket_ARG_close_errorclose_error);
        EhsTPMutex_unlock(EhsTPMutex_fbIO);
    }
}

//ICB FUNCTION close MACRO END -- DO NOT ALTER
//ICB FUNCTION send MACRO START -- DO NOT ALTER
/**
 * Definition of netSocket_send.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 *
 *  'todo this should be threaded when tcpip as this can hand around?
 */
EHS_FB_RUN_FUNCTION(netSocket_send)
{
    inx_netSocket_state_type* inx_netSocket_state =
        (inx_netSocket_state_type*) EHS_FB_RUN_CONTEXT;
    /* sanity checks */
    if (inx_netSocket_state->bIsOpening != EHS_TRUE) goto connection_error;
    if (inx_netSocket_state->tcp_udp == 0 && inx_netSocket_state->connection_socket == EHS_TGT_TCP_INVALID_SOCKET)
    {
        _netsock_debug_printf("inx_netSocket_state->connection_socket == EHS_TGT_TCP_INVALID_SOCKET\n");
        goto connection_error;
    }
    //else if ( inx_netSocket_state->tcp_udp == 0)
    //	goto connection_error;
    // Your code here
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    if (EHS_FB_IN_CONNECTED_API2(INX_netSocket_ARG_send_size))
    {
        if (EHS_FB_IN_I_API2(INX_netSocket_ARG_send_size) > EHS_STRING_LENGTH_MAX)
        {
            EhsTPMutex_unlock(EhsTPMutex_fbIO);
            goto length_error;
        }
        else
        {
            inx_netSocket_state->data_send_size =EHS_FB_IN_I_API2(INX_netSocket_ARG_send_size); // this will trigger the send
        }

    }
    else     // get the length of the string if not set
    {
        if (EHS_FB_IN_CONNECTED_API2(INX_netSocket_ARG_send_datadata_in))
        {
            inx_netSocket_state->data_send_size = EhsStrlen(EHS_FB_IN_S_API2(INX_netSocket_ARG_send_datadata_in));
        }
    } //warning - can we get non-atomic data length and actual data size here in the threaded part? I guess we need to mutex the writing thread.
    if (EHS_FB_IN_CONNECTED_API2(INX_netSocket_ARG_send_datadata_in))   /* copy the whole string ignoring 0s if there are any on the input*/
    {
        EhsStrncpy(inx_netSocket_state->data_send_buf,EHS_FB_IN_S_API2(INX_netSocket_ARG_send_datadata_in),inx_netSocket_state->data_send_size);
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO); // @todo mutexes are  bit messy here ...
    goto all_good;
connection_error:
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    if (EHS_FB_OUT_CONNECTED_API2(INX_netSocket_ARG_send_errnoerrorno))
    {
        EHS_FB_OUT_I_API2(INX_netSocket_ARG_send_errnoerrorno) = -103;
    }
    EHS_FB_FINISH_API2(INX_netSocket_ARG_send_error);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    EHSH_LOG_ERROR("Connection Error");
    return;

length_error:
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    if (EHS_FB_OUT_CONNECTED_API2(INX_netSocket_ARG_send_errnoerrorno))
    {
        EHS_FB_OUT_I_API2(INX_netSocket_ARG_send_errnoerrorno) = -102;
    }

    EHS_FB_FINISH_API2(INX_netSocket_ARG_send_error);
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    EHSH_LOG_ERROR("Data Length Error");
    return;
all_good:
    /* outputs are sent by send thread */
    return;

}
//ICB FUNCTION send MACRO END -- DO NOT ALTER
//ICB FUNCTION receive MACRO START -- DO NOT ALTER


#ifdef EHS_SKT_DEBUG
{
    /*
    ** Temporary character string buffers for host & service.
    */
    char hostBfr[ 30 ];
    char servBfr[ 30 ];
    /*
    ** Display the address information just fetched.  Start with the
    ** common (protocol-independent) stuff first.
    */
    fprintf( stderr,
             "Address info:\n"
             "   ai_flags     = 0x%02X\n"
             "   ai_family    = %d (PF_INET = %d, PF_INET6 = %d)\n"
             "   ai_socktype  = %d (SOCK_STREAM = %d, SOCK_DGRAM = %d)\n"
             "   ai_protocol  = %d (IPPROTO_TCP = %d, IPPROTO_UDP = %d)\n"
             "   ai_addrlen   = %d (sockaddr_in = %d, "
             "sockaddr_in6 = %d)\n",
             rp->ai_flags,
             rp->ai_family,
             PF_INET,
             PF_INET6,
             rp->ai_socktype,
             SOCK_STREAM,
             SOCK_DGRAM,
             rp->ai_protocol,
             IPPROTO_TCP,
             IPPROTO_UDP,
             rp->ai_addrlen,
             sizeof( struct sockaddr_in ),
             sizeof( struct sockaddr_in6 ) );
    /*
    ** Display the protocol-specific formatted address.
    */
    getnameinfo( rp->ai_addr,
                 rp->ai_addrlen,
                 hostBfr,
                 sizeof( hostBfr ),
                 servBfr,
                 sizeof( servBfr ),
                 NI_NUMERICHOST | NI_NUMERICSERV );
    switch ( rp->ai_family )
    {
    case PF_INET:   /* IPv4 address record. */
    {
        sockaddr_in_t *pSadrIn = (sockaddr_in_t*) rp->ai_addr;
        fprintf( stderr,
                 "   ai_addr      = sin_family: %d (AF_INET = %d, "
                 "AF_INET6 = %d)\n"
                 "                  sin_addr:   %s\n"
                 "                  sin_port:   %s\n",
                 pSadrIn->sin_family,
                 AF_INET,
                 AF_INET6,
                 hostBfr,
                 servBfr );
        break;
        }  /* End CASE of IPv4 record. */
    case PF_INET6:   /* IPv6 address record. */
    {
        pSadrIn6 = (sockaddr_in6_t*) rp->ai_addr;
        fprintf( stderr,
                 "   ai_addr      = sin6_family:   %d (AF_INET = %d, "
                 "AF_INET6 = %d)\n"
                 "                  sin6_addr:     %s\n"
                 "                  sin6_port:     %s\n"
                 "                  sin6_flowinfo: %d\n"
                 "                  sin6_scope_id: %d\n",
                 pSadrIn6->sin6_family,
                 AF_INET,
                 AF_INET6,
                 hostBfr,
                 servBfr,
                 pSadrIn6->sin6_flowinfo,
                 pSadrIn6->sin6_scope_id );
        break;
        }  /* End CASE of IPv6 record. */
    default:   /* Can never get here, but just for completeness. */
    {
        fprintf( stderr,
                 "%s (line %d): ERROR - Unknown protocol family (%d).\n",
                 pgmName,
                 __LINE__,
                 rp->ai_family );
        break;
        }  /* End DEFAULT case (unknown protocol family). */
    }  /* End SWITCH on protocol family. */
}  /* End IF verbose mode. */
#endif


