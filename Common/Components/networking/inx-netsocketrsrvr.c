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
#include "inx-netsocketrsrvr.h"
//ICB HEADER MACRO END -- DO NOT ALTER

#include "hal_network.h"
#include <fcntl.h>
#ifndef EHS_MINGW
// #define _GNU_SOURCE
#ifndef EHS_LWIP
#include <poll.h>
#endif // EHS_LWIP
#warning "need to move poll.h to hal"
#endif

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */

#define _inx_socketsrver_printf(x,...) {} //printf


void debugConnections(EhsTgtTcpSockAddrInType * rp);

typedef struct
{
    ehs_bool bIsOpening; /* Logical mutex on this not opening when already open */
    ehs_uint32 read_dwell_time;
    ehs_sint32 Port;
    ehs_sint32 udp;
    ehs_uint32 data_send_size; /* amount of data to send to a port */
    ehs_uint32 data_recv_size; /* amount of data received over a socket */

    /* Dynamic data */
    ehs_sint32 listen_port;
    EhsTgtTcpSocketType listen_socket; /* Listening socket handle */
    EhsTgtTcpSocketType connection_socket; /* This is the socket descriptor */
    //ehs_bool stopListeningNow; //flag this when you want to stop the server.
    ehs_bool closeConnectionNow; //flag this when you want to stop a connection (server or client mode) is set to 0 when actually closed.
    /* buffers */
    struct sockaddr_in remoteConnectionSockAddr; // This stores the address of the remote host (needed for UDP sendto responses
    int remoteConnectionSockAddrSize; // populated with the size of the address struct
    int clientSocket; // the accepted socket used for TCP comms
    ehs_char data_recv_buf[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH?
    ehs_char data_send_buf[EHS_STRING_LENGTH_MAX]; //TODO:STRINGLENGTH!?
    ehs_char Interface[EHS_STRING_LENGTH_MAX];  //TODO:STRINGLENGTH!?
} inx_netsocketsrvr_state_type; //Reference this, maybe store your config parameters in here too.

//ICB STATE VAR MACRO END -- DO NOT ALTER
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(netsocketsrvr)

EHS_FB_FUNCTION_ENTRY("open", 0x01, netsocketsrvr_open)

EHS_FB_FUNCTION_ENTRY("close", 0x02, netsocketsrvr_close)

EHS_FB_FUNCTION_ENTRY("sendData", 0x03, netsocketsrvr_sendData)

EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

EHS_FB_THREAD_FUNCTION(netSocket_listen);

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_netsocketsrvr_ARG_open_IF 1
#define INX_netsocketsrvr_ARG_open_port 2
#define INX_netsocketsrvr_ARG_open_data 1
#define INX_netsocketsrvr_ARG_open_size 2
#define INX_netsocketsrvr_ARG_open_source 3
#define INX_netsocketsrvr_ARG_open_opened 1
#define INX_netsocketsrvr_ARG_open_error 2
#define INX_netsocketsrvr_ARG_open_received 3
#define INX_netsocketsrvr_ARG_open_closed 4
#define INX_netsocketsrvr_ARG_open_closeError 5
#define INX_netsocketsrvr_ARG_open_sent 6
#define INX_netsocketsrvr_ARG_open_sendError 7
#define INX_netsocketsrvr_ARG_close_closed 1
#define INX_netsocketsrvr_ARG_close_closeError 2
#define INX_netsocketsrvr_ARG_sendData_inData 1
#define INX_netsocketsrvr_ARG_sendData_inSize 2
//#define INX_netsocketsrvr_ARG_sendData_sent 1
//#define INX_netsocketsrvr_ARG_sendData_sendError 2

//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_netsocketsrvr_Interface 0
#define INX_FB_netsocketsrvr_Port 8000
#define INX_FB_netsocketsrvr_udp 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(netsocketsrvr)
{
    /* Uncomment the following if you need to parse the parameters to calculate memory required */
    /*
    ehs_char* Interface;
    ehs_sint32 Port;
    ehs_sint32 udp;
    EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %d %d",&Interface,&Port,&udp); */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_netsocketsrvr_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */



void EhsNetSocketServerInitDynamicVars(inx_netsocketsrvr_state_type* inx_netsocketsrvr_state)
{
    inx_netsocketsrvr_state->bIsOpening= EHS_FALSE;
    inx_netsocketsrvr_state->data_recv_buf[0]='\0';
    inx_netsocketsrvr_state->data_send_buf[0]='\0';
    inx_netsocketsrvr_state->data_recv_size=0;
    inx_netsocketsrvr_state->data_send_size=0;
    inx_netsocketsrvr_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;
    inx_netsocketsrvr_state->closeConnectionNow = EHS_FALSE;
    inx_netsocketsrvr_state->clientSocket = EHS_TGT_TCP_INVALID_SOCKET;
    memset(&inx_netsocketsrvr_state->remoteConnectionSockAddr, 0, sizeof(struct sockaddr_in));
}

EHS_FB_INIT_FUNCTION(netsocketsrvr)
{
    ehs_sint32 Port;
    ehs_sint32 tcp;
    ehs_bool bRet = EHS_TRUE; /* assume success */
    //this is the reference to the object data for this instance of the function block
    inx_netsocketsrvr_state_type* inx_netsocketsrvr_state = (inx_netsocketsrvr_state_type*)EHS_FB_INIT_CONTEXT;
    /* read the initialisation parameters - the %s carries a field width from
     * sizeof, or sscanf writes as many characters as the parameter holds */
    {
        ehs_char szScanFmt[32];
        EhsSnprintf(szScanFmt, sizeof(szScanFmt), "%%%us %%d %%d",
                    (unsigned int)(sizeof(inx_netsocketsrvr_state->Interface) - 1u));
        EhsSscanf(EHS_FB_INIT_PARAMETERS,szScanFmt,inx_netsocketsrvr_state->Interface,&Port,&tcp);
    }

    inx_netsocketsrvr_state->Port=Port;
    inx_netsocketsrvr_state->listen_port=Port;
    inx_netsocketsrvr_state->udp=tcp;
    inx_netsocketsrvr_state->read_dwell_time=2; //default to 2ms (avoid loosing UDP to buffer overflow though), @todo make this a parameter to govern data rate.

    EhsNetSocketServerInitDynamicVars( inx_netsocketsrvr_state);

    /* Add any further intialisation code here */
    return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

/* Close the listening socket by what ever means necessary .. */
void netsocketsrvr_forceclose(inx_netsocketsrvr_state_type *inx_netsocketsrvr_state)
{
    inx_netsocketsrvr_state->closeConnectionNow = EHS_TRUE; /* set this first so no transactions are attempted on this spoof connection */
//assumption is above flag will cause network thread to close sockets and exit

    if (inx_netsocketsrvr_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET )
    {
        /* We need to try and connect to our waiting (blocking accept) to unblock it before leaving */
        int sockfd = 0, socktype,sockptrot;
        struct sockaddr_in serv_addr;
        inx_netsocketsrvr_state->closeConnectionNow = EHS_TRUE; /* set this first so no transactions are attempted on this spoof connection */
        if (inx_netsocketsrvr_state->udp == 0)
        {
            socktype = SOCK_STREAM;
            sockptrot = IPPROTO_TCP;
        }
        else
        {
            socktype = SOCK_DGRAM;
            sockptrot = 0;
        }
        /* We need to connect to the socket to unblock it to close it */
        if((sockfd = socket(AF_INET, socktype, 0)) > -1 )
        {
            memset(&serv_addr, '0', sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(inx_netsocketsrvr_state->listen_port);
            serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0)
            {
                // do nothing - we must close it anyway.
            }
            EhsSvcTgtTcp_closeConnection(sockfd); // unblock the accept or receive calls
        }
        /* Now we cann hopefully close the listening socket thread and exit */
        EhsSvcTgtTcp_closeConnection(inx_netsocketsrvr_state->connection_socket); // unblock the accept or receive calls

        inx_netsocketsrvr_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;
    }
    EhsNetSocketServerInitDynamicVars(inx_netsocketsrvr_state);
}


//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(netsocketsrvr)
{
    inx_netsocketsrvr_state_type *inx_netsocketsrvr_state = (inx_netsocketsrvr_state_type*)EHS_FB_DESTROY_CONTEXT;

    netsocketsrvr_forceclose(inx_netsocketsrvr_state);
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION open MACRO START -- DO NOT ALTER
/**
 * Definition of netsocketsrvr_open.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(netsocketsrvr_open)
{
    inx_netsocketsrvr_state_type* inx_netsocketsrvr_state = (inx_netsocketsrvr_state_type*)EHS_FB_RUN_CONTEXT;

    ehs_sint32 port = 0;

    // Your code here
    if (EHS_FB_IN_CONNECTED_API2(INX_netsocketsrvr_ARG_open_IF))
        EhsStrcpy(inx_netsocketsrvr_state->Interface,EHS_FB_IN_S_API2(INX_netsocketsrvr_ARG_open_IF));
    if (EHS_FB_IN_CONNECTED_API2(INX_netsocketsrvr_ARG_open_port))
        inx_netsocketsrvr_state->listen_port =  EHS_FB_IN_I_API2(INX_netsocketsrvr_ARG_open_port) ;

    //EhsTgtTcpSockAddrInType connection; /* used to construct the address of the EhsTgtTcpSocketType we're creating */
    ehs_bool ret = EHS_FALSE; /* assume initialisation fails */
    ehs_uint8 retVal;

    /*TCPIP MODE */
    /* same for both: if (inx_netSocket_state->tcp_udp == EHS_FALSE) {*/
    /* Check if we are closed */
    if (inx_netsocketsrvr_state->bIsOpening == EHS_TRUE || inx_netsocketsrvr_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        EHSH_LOG_WARNING("X1 - INX_netsocketsrvr_ARG_open_error - already open");
        EHS_FB_FINISH(INX_netsocketsrvr_ARG_open_error);
    }
    else
    {
        inx_netsocketsrvr_state->bIsOpening = EHS_TRUE;
        /* connect to server */
        inx_netsocketsrvr_state->bIsOpening = EHS_TRUE;
        EHS_FB_START_THREAD(netSocket_listen,-90);
    }
}//ICB FUNCTION open MACRO END -- DO NOT ALTER THIS LINE



//ICB FUNCTION close MACRO START -- DO NOT ALTER
/**
 * Definition of netsocketsrvr_close.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(netsocketsrvr_close)
{
    inx_netsocketsrvr_state_type* inx_netsocketsrvr_state = (inx_netsocketsrvr_state_type*)EHS_FB_RUN_CONTEXT;

    // Your code here

    //_inx_socketsrver_printf("X3 - Setting inx_netsocketsrvr_state->closeConnectionNow = EHS_FALSE; \n");

    if (inx_netsocketsrvr_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET )
    {
        _inx_socketsrver_printf("Try to force close the socket descriptor\n");
        netsocketsrvr_forceclose(inx_netsocketsrvr_state);
    }
    else
    {
        EhsNetSocketServerInitDynamicVars(inx_netsocketsrvr_state);
        EHS_FB_FINISH(INX_netsocketsrvr_ARG_close_closed);
    }

    //Thread asserts the closed event - not done here before actual close
    //EHS_FB_FINISH(INX_netsocketsrvr_ARG_close_closed);
    //EHS_FB_FINISH(INX_netsocketsrvr_ARG_close_closeError);
}//ICB FUNCTION close MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION sendData MACRO START -- DO NOT ALTER
/**
 * Definition of netsocketsrvr_sendData.
 * writes output data to the buffer that will be output by the listening thread
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(netsocketsrvr_sendData)
{
    inx_netsocketsrvr_state_type* inx_netsocketsrvr_state = (inx_netsocketsrvr_state_type*)EHS_FB_RUN_CONTEXT;
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    if (inx_netsocketsrvr_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)   // only allw of a socket is opened - even in UDP case
    {
        if (EHS_FB_IN_CONNECTED_API2(INX_netsocketsrvr_ARG_sendData_inData))
        {
            EhsStrcpy(inx_netsocketsrvr_state->data_send_buf,EHS_FB_IN_S_API2(INX_netsocketsrvr_ARG_sendData_inData)) ;
        }
        if (EHS_FB_IN_CONNECTED_API2(INX_netsocketsrvr_ARG_sendData_inSize))
        {
            inx_netsocketsrvr_state->data_send_size = EHS_FB_IN_I_API2(INX_netsocketsrvr_ARG_sendData_inSize) ;
        }
        else
        {
            inx_netsocketsrvr_state->data_send_size = EhsStrlen(inx_netsocketsrvr_state->data_send_buf);
        }
    }
    else
    {
        EHSH_LOG_ERROR("Could not send data in closed socket");
        EHS_FB_FINISH(INX_netsocketsrvr_ARG_open_sendError);
    }
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
}

/* This is the actual socket sending function called from the listening loop after a receive. Pass it the accepted socket for TCP connections */
EHS_FB_RUN_FUNCTION(ActualSendData)
{
    inx_netsocketsrvr_state_type* inx_netsocketsrvr_state =  (inx_netsocketsrvr_state_type*)EHS_FB_RUN_CONTEXT;
    ehs_bool ret = EHS_FALSE;
    int data_size=0;


    if (inx_netsocketsrvr_state->connection_socket == EHS_TGT_TCP_INVALID_SOCKET )
    {
        EHSH_LOG_ERROR("Socket is not open cannot send data from socket server");
        ret = EHS_FALSE;
    }
    else if ( inx_netsocketsrvr_state->udp == 0 && inx_netsocketsrvr_state->clientSocket == EHS_TGT_TCP_INVALID_SOCKET)
    {
        EHSH_LOG_ERROR("client socket not available for TCP mode reponse");
        ret = EHS_FALSE;
    }
    else
    {
        if (inx_netsocketsrvr_state->udp == 0)   /* TCPIP */
        {
            if ( inx_netsocketsrvr_state->clientSocket == EHS_TGT_TCP_INVALID_SOCKET)
            {
                EHSH_LOG_ERROR("socket not created for tcpip - not trying");
                ret = EHS_FALSE;
            }
            else
            {
                data_size = send(inx_netsocketsrvr_state->clientSocket,(const ehs_char *) inx_netsocketsrvr_state->data_send_buf,(ehs_sint32) inx_netsocketsrvr_state->data_send_size,(ehs_sint32) 0); /* Send connection in blocking mode */
            }
        }
        else     /* UDP mode use sendto and use the listening socket*/
        {
            data_size = sendto(inx_netsocketsrvr_state->connection_socket,(const ehs_char *) inx_netsocketsrvr_state->data_send_buf,(ehs_sint32) inx_netsocketsrvr_state->data_send_size,(ehs_sint32) 0,(struct sockaddr *)&(inx_netsocketsrvr_state->remoteConnectionSockAddr),inx_netsocketsrvr_state->remoteConnectionSockAddrSize);
        }
        if (data_size != inx_netsocketsrvr_state->data_send_size)
        {
            EHSH_LOG_ERROR("listening socket send error size sent =%d, measured=%d",inx_netsocketsrvr_state->data_send_size,data_size);
            EhsTPMutex_lock(EhsTPMutex_fbIO);
            EHS_FB_FINISH(INX_netsocketsrvr_ARG_open_sendError);
            EhsTPMutex_unlock(EhsTPMutex_fbIO);
        }
        else
        {
            EhsTPMutex_lock(EhsTPMutex_fbIO);
            EHS_FB_FINISH(INX_netsocketsrvr_ARG_open_sent);
            EhsTPMutex_unlock(EhsTPMutex_fbIO);
        }
    }
    inx_netsocketsrvr_state->data_send_size = 0 ;// don't try again
}

//ICB FUNCTION receiveData MACRO START -- DO NOT ALTER
/**
 * Definition of netsocketsrvr_receiveData.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
/*EHS_FB_RUN_FUNCTION(netsocketsrvr_receiveData)
{
	inx_netsocketsrvr_state_type* inx_netsocketsrvr_state = (inx_netsocketsrvr_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
}//ICB FUNCTION receiveData MACRO END -- DO NOT ALTER THIS LINE
*/

EHS_FB_THREAD_FUNCTION(netSocket_listen)
{

    //@todo this is posix only
#ifndef EHS_MINGW
    struct pollfd  pfd;
#endif
    ehs_sint32 sckt_err = 0;
    ehs_bool start_reading = EHS_FALSE;
    inx_netsocketsrvr_state_type* inx_netSocket_state = (inx_netsocketsrvr_state_type*) EHS_FB_RUN_CONTEXT;
    /* todo2023 - try to avoid using this on the stack */
    ehs_uint8 bBuffIn[EHS_TGT_TCP_IN_BUFF_SIZE]; /* buffer for incoming data */
    Ehs_FB_ThreadStarted();
    ehs_sint32 nDataReceived = 0; /* indexes data_recv_buf below - never leave it garbage */
    struct sockaddr_in si_me;
    /* amount of data received by TCP/IP */
    ehs_uint8 *pData = bBuffIn;
    ehs_bool bDisconnect = EHS_FALSE; /* assume we want to stay connected */

    struct sockaddr_in *connection_addr = &(inx_netSocket_state->remoteConnectionSockAddr); /* This is the address list struct for the target server*/
    int * SenderAddrSize = &(inx_netSocket_state->remoteConnectionSockAddrSize);
    *SenderAddrSize = sizeof(struct sockaddr_in);

    if (inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        EHSH_LOG_ERROR("Already connected to socket - Disconecting and reconnecting");
        EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket);
    }
    inx_netSocket_state->data_send_size = 0;/* Just in case this is left untidy */

    /* One last Check - we shouldn't be here with an open socket, but then...*/
    if (inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
        EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket);
    inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;

    if (inx_netSocket_state->udp == 1)
    {
        inx_netSocket_state->connection_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); /* Open UDP Socket */
    }
    else
    {
        inx_netSocket_state->connection_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* Open TCP Socket */
        //fcntl(inx_netSocket_state->connection_socket, F_SETFL, fcntl(inx_netSocket_state->connection_socket, F_GETFL, 0) | O_NONBLOCK); // make this non-blocking soaccept dosn't block
#ifndef EHS_MINGW
        int yes=1;
        setsockopt(inx_netSocket_state->connection_socket,SOL_SOCKET,SO_REUSEADDR,(char*)&yes,sizeof(int)); // try to allow restarts after closing TCP listeners
#endif
    }
#ifdef EHS_MINGW
    DWORD timeout=1;
    /*struct timeval timeout;
    	timeout.tv_sec=0;
    	timeout.tv_usec=10000;
    	*/
    if (inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET)
    {
        if (setsockopt(inx_netSocket_state->connection_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)))
        {
            EHSH_LOG_ERROR("setsockopt failed");
        }
    }
#endif
    /* Bind to all interface at the given port */
    memset(&si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(inx_netSocket_state->listen_port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    if( bind(inx_netSocket_state->connection_socket, (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket); // socket is still open even if not bound
        inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET;
        EhsTPMutex_lock(EhsTPMutex_fbIO);
        EHS_FB_FINISH(INX_netsocketsrvr_ARG_open_error);
        EhsTPMutex_unlock(EhsTPMutex_fbIO);
    }
    else
    {
        if (inx_netSocket_state->udp == 0)   // listen if we are tcp connection
        {
            listen(inx_netSocket_state->connection_socket, 10);   // allow 10 pending connections
        }
        //@todo should write an abstracted simple poll with TO.
#ifndef EHS_MINGW
        pfd.fd = inx_netSocket_state->connection_socket;
        pfd.events =  POLLHUP | /*POLLRDHUP |*/ POLLERR |POLLNVAL ; /* check for hang up - not POLLIN |  as we want to read to*/
        pfd.revents = 0;
#endif
        /* we're good so pole for data (at a limited rate).*/
        EhsTPMutex_lock(EhsTPMutex_fbIO);
        EHS_FB_FINISH(INX_netsocketsrvr_ARG_open_opened);/* Yes We're ready to go! */
        EhsTPMutex_unlock(EhsTPMutex_fbIO);
        /* set up some client socket addresses (tcp only? */
        ehs_bool client_accepted = EHS_FALSE;
        //struct sockaddr_in client;
        //int c = sizeof(struct sockaddr_in);
        //int client_sock = EHS_TGT_TCP_INVALID_SOCKET; //connection to client todo -this is a bit inconsistent with the remote address stored in the sate struct...
        inx_netSocket_state->clientSocket = EHS_TGT_TCP_INVALID_SOCKET;
        /* loop while listening for connections */
        while (inx_netSocket_state->closeConnectionNow == EHS_FALSE && *bNewSodlFlagRef == EHS_FALSE)
        {
            /* We will only accpet one client */
            if ( client_accepted == EHS_FALSE && inx_netSocket_state->udp == 0 )  /* For TCP we need to accept a connection */
            {

                inx_netSocket_state->clientSocket = accept(inx_netSocket_state->connection_socket, (struct sockaddr *)connection_addr, (socklen_t*)SenderAddrSize);  // blocks
                client_accepted = EHS_TRUE;
                nDataReceived = 0;
            }
            if (inx_netSocket_state->closeConnectionNow != EHS_TRUE)
            {
                if (inx_netSocket_state->udp == 0)
                {
                    if (inx_netSocket_state->clientSocket  != EHS_TGT_TCP_INVALID_SOCKET )
                    {
#ifndef EHS_MINGW
                        nDataReceived = recv(inx_netSocket_state->clientSocket,inx_netSocket_state->data_recv_buf, EHS_STRING_LENGTH_MAX-1,0x40 );  // Get up to the largest amount of data
#else
                        nDataReceived = recv(inx_netSocket_state->clientSocket,inx_netSocket_state->data_recv_buf, EHS_STRING_LENGTH_MAX-1,0x00 );  // Get up to the largest amount of data
#endif
                        _inx_socketsrver_printf("TCP<---(%d),%d\n", nDataReceived,errno);

                        if (nDataReceived == -1 && errno != EAGAIN  && errno != 0 ) //&& errno != 0) {
                        {
                            EHSH_LOG_ERROR(" Socket receive error while Listening (%d=%s)",errno,strerror(errno));
                        }
                        else if (nDataReceived == 0 )
                        {
                            _inx_socketsrver_printf("Client disconnected\n");
                            inx_netSocket_state->clientSocket = EHS_TGT_TCP_INVALID_SOCKET;
                            client_accepted = EHS_FALSE;
                        }
                    }
                }
                else     /* for UDP */
                {

#ifndef EHS_MINGW
                    nDataReceived = recvfrom(inx_netSocket_state->connection_socket,(ehs_char*) inx_netSocket_state->data_recv_buf, EHS_STRING_LENGTH_MAX-1, 0x40, (struct sockaddr*) connection_addr,SenderAddrSize); // Get up to the largest amount of data
#else
                    nDataReceived = recvfrom(inx_netSocket_state->connection_socket,(ehs_char*) inx_netSocket_state->data_recv_buf, EHS_STRING_LENGTH_MAX-1, 0x00, (struct sockaddr*) connection_addr,SenderAddrSize); // Get up to the largest amount of data
#endif
                    _inx_socketsrver_printf("UDP<---\n");
                }
                if (nDataReceived > 0) /* did we receive data?  Write it to the output buffer with a terminiator and number of bytes*/
                {
                    EhsTPMutex_lock(EhsTPMutex_fbIO);
                    if (EHS_FB_OUT_CONNECTED_API2(INX_netsocketsrvr_ARG_open_data))
                    {
                        /* Note we are not expecting date lengths > EHS_STRING_LENGTH_MAX because the recvfrom function should limit this */
                        inx_netSocket_state->data_recv_buf[nDataReceived] = '\0'; /* this is probably unnescesery, but loosing 1 byte to save a missuse of the outpout as a null terminated string may save lives*/
                        EHS_FB_OUT_S_SETN_API2(INX_netsocketsrvr_ARG_open_data, inx_netSocket_state->data_recv_buf, nDataReceived+1);
                        //((ehs_char*) (EHS_FB_OUT_S_API2(INX_netsocketsrvr_ARG_open_data)))[nDataReceived] = '\0';
                    }
                    if (EHS_FB_OUT_CONNECTED_API2(INX_netsocketsrvr_ARG_open_size))
                        EHS_FB_OUT_I_API2(INX_netsocketsrvr_ARG_open_size) = nDataReceived ;

                    if (EHS_FB_OUT_CONNECTED_API2(INX_netsocketsrvr_ARG_open_source))
                        EHS_FB_OUT_S_SET_API2(INX_netsocketsrvr_ARG_open_source, inet_ntoa(connection_addr->sin_addr));

                    EHS_FB_FINISH_API2(INX_netsocketsrvr_ARG_open_received);
                    EhsTPMutex_unlock(EhsTPMutex_fbIO);
                    /*  "need to wait here for the data read flag before reading any more.*/
                }
                if (inx_netSocket_state->data_send_size > 0)
                {
                    EHS_FB_START_RUN_FUNCTION(ActualSendData);
                }
                inx_netSocket_state->data_send_size = 0;
                /* Throttle read rate using configured parameter Not using poll timeout and not sleep for posix*/
                EhsSleepUs(inx_netSocket_state->read_dwell_time*1000);
            }
        }// end of open loop
        _inx_socketsrver_printf("Exiting listen\n");
        if (inx_netSocket_state->clientSocket != EHS_TGT_TCP_INVALID_SOCKET )
        {
            EhsSvcTgtTcp_closeConnection(inx_netSocket_state->clientSocket);
            inx_netSocket_state->clientSocket = EHS_TGT_TCP_INVALID_SOCKET;
        }

        //EHSH_LOG_INFO("All done: closing flag=%d Closing connection",inx_netSocket_state->closeConnectionNow);
        if (inx_netSocket_state->connection_socket != EHS_TGT_TCP_INVALID_SOCKET )
        {
            EhsSvcTgtTcp_closeConnection(inx_netSocket_state->connection_socket); //close(sfd);
            inx_netSocket_state->connection_socket = EHS_TGT_TCP_INVALID_SOCKET; /* This is the state machine flag too */
        }
        inx_netSocket_state->closeConnectionNow = EHS_FALSE; //signal we have closed.
    }

    EhsNetSocketServerInitDynamicVars(inx_netSocket_state);
    EhsTPMutex_lock(EhsTPMutex_fbIO);
    EHS_FB_FINISH(INX_netsocketsrvr_ARG_open_closed); /* CAREFUL! if close is connected to open we can get a tight loop here! @todo consider a timer here to avoid tight loops */
    EhsTPMutex_unlock(EhsTPMutex_fbIO);
    Ehs_FB_ThreadComplete();
    EhsHThread_exit();
}


typedef struct sockaddr_in       sockaddr_in_t;
typedef struct sockaddr_in6      sockaddr_in6_t;

void debugConnections(EhsTgtTcpSockAddrInType * rp)
{
    /*
     ** Temporary character string buffers for host & service.
     */
    sockaddr_in6_t  *pSadrIn6;
    int i=0;
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
#ifdef EHS_LWIP
                 0 );
#else
                 NI_NUMERICHOST | NI_NUMERICSERV );
#endif
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
                 "(line %d): ERROR - Unknown protocol family (%d).\n",
                 __LINE__,
                 rp->ai_family );
        break;
        }  /* End DEFAULT case (unknown protocol family). */
    }  /* End SWITCH on protocol family. */
}  /* End IF verbose mode. */

