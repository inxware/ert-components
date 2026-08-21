/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/


/** @file target_tcp.c
 * Definitions for target-specific tcp-related functions
 *
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section target_tcp
 * @anchor target_tcp
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on 2007-10-12
 * Last modified on $Date$
 *
 * This file contained no derogations to the MISRA standard.
 *
 * Note it is necessary to replace <sys/types.h> with <types.h> to lint this file successfully.
 */


/**
 * Provides access to the target-specific declarations of header files
 */
#define EHS_TARGET_CODE

//#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_NETWORK

/*****************************************************************************/
/* Included files */

#include <sys/types.h> /* required for recv */
//#include <string.h> /* required for memset */
#include <string.h> // todo2024 - needed for memset, but we should abstract this in the hal.

#include "globals.h"
#include "target_tcp.h"
#include "hal.h"
#include "hal_string.h"
#include "hal_logger.h"

// need to include these after ert hal for some reason?? e.g. setting gnu options?

/* rtnetlink interface enumeration is a LINUX facility, not part of POSIX
 * sockets - so it is unavailable on other platforms that use this otherwise
 * portable bsdsockets backend (macOS, and Zephyr with CONFIG_POSIX_API +
 * CONFIG_NET_SOCKETS). Expressed as a positive capability rather than a
 * growing list of #ifndef EHS_<os> exclusions at each of the two use sites.
 *
 * Safe to omit: EhsTgtTcp_IterateInterfaces() and rtnl_get_link_ipv4addr()
 * are self-contained diagnostics with no callers anywhere else in the tree. */
#if !defined(EHS_MACOS) && !defined(EHS_ZEPHYR_RTOS)
#define EHS_BSDSOCKETS_HAVE_NETLINK 1
#endif

#ifdef EHS_BSDSOCKETS_HAVE_NETLINK
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#endif
#include <arpa/inet.h>

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */
/*****************************************************************************/
/* Declare prototypes of local functions */
/*****************************************************************************/
/* Variables defined with file-scope */
/*****************************************************************************/
/* Variables defined with global-scope */
/*****************************************************************************/
/* Function definitions */



#define NIPQUAD(addr) \
        ((unsigned char *)&addr)[0], \
        ((unsigned char *)&addr)[1], \
        ((unsigned char *)&addr)[2], \
        ((unsigned char *)&addr)[3]

/* returns the IP address of the current interface handle if it matches the string- nlmsghdr
 * todo split this into one function that returns a list of interface strings and another that returns the ip address (perhaps with a cache of some type)
 * */
#ifdef EHS_BSDSOCKETS_HAVE_NETLINK
ehs_bool rtnl_get_link_ipv4addr(struct nlmsghdr *h,ehs_char * if_string_match,unsigned long *in4p_ret)
{
    struct ifaddrmsg *ifaddr;
    struct ifinfomsg *iface;
    struct rtattr *attribute;
    unsigned short *in4p;
    int len;
    ehs_bool bGotit=EHS_FALSE;

    in4p=NULL;
    iface = NLMSG_DATA(h);
    len = h->nlmsg_len - NLMSG_LENGTH(sizeof(*iface));
    for (attribute = IFLA_RTA(iface); RTA_OK(attribute, len); attribute = RTA_NEXT(attribute, len))
    {
        //
        switch(attribute->rta_type)
        {
        case IFLA_IFNAME:
            if (EhsStrcmp((char *) RTA_DATA(attribute),if_string_match) == 0 ) bGotit = EHS_TRUE;
            break;
        case IFA_ADDRESS:
        {
            struct in6_addr *in6p;
            //in4p = RTA_DATA(attribute);
            in4p = RTA_DATA(attribute);
            if (bGotit)
            {
                *in4p_ret=htonl(*in4p);
                //memcpy(in4p_ret,in4p,4);// todo this is messy!
                char str[64];
                inet_ntop(AF_INET,in4p,str,64);
            }
        }
        break;
        default:
            break;
        }
    }
    return bGotit;
}

/* todo This is really just a code snippt that need turning into a proper HAL function
 * for components or the kernel to list the interfaces available and provide P address
 */
void EhsTgtTcp_IterateInterfaces()
{
    struct sockaddr_nl local;

    EhsTgtTcpSocketType  fd;
    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    memset(&local, 0, sizeof(local));
    local.nl_family = AF_NETLINK;
    local.nl_pid = getpid(); /* can be obtained using getpid*/
    local.nl_groups = 0;

    if (bind(fd, (struct sockaddr *) &local, sizeof(local)) < 0)
    {
        //EHSH_ERROR("Can't bind the IP console socket");
    }
    {
        typedef struct nl_req_s nl_req_t;

        struct nl_req_s
        {
            struct nlmsghdr hdr;
            struct rtgenmsg gen;
        };
        struct sockaddr_nl kernel;
        struct msghdr rtnl_msg;
        struct iovec io;
        nl_req_t req;
        memset(&rtnl_msg, 0, sizeof(rtnl_msg));
        memset(&kernel, 0, sizeof(kernel));
        memset(&req, 0, sizeof(req));

        kernel.nl_family = AF_NETLINK; /* fill-in kernel address (destination) */

        req.hdr.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtgenmsg));
        req.hdr.nlmsg_type = RTM_GETLINK;
        req.hdr.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
        req.hdr.nlmsg_seq = 1;
        req.hdr.nlmsg_pid = getpid();
        req.gen.rtgen_family = AF_PACKET; /*  no preferred AF, we will get *all* interfaces */

        io.iov_base = &req;
        io.iov_len = req.hdr.nlmsg_len;
        rtnl_msg.msg_iov = &io;
        rtnl_msg.msg_iovlen = 1;
        rtnl_msg.msg_name = &kernel;
        rtnl_msg.msg_namelen = sizeof(kernel);
#define IFLIST_REPLY_BUFFER 16384
        sendmsg(fd, (struct msghdr *) &rtnl_msg, 0);
        {
            int end = 0;
            char reply[IFLIST_REPLY_BUFFER]; /* a large buffer */
            while (!end)
            {
                int len;
                struct nlmsghdr *msg_ptr;    /* pointer to current part */
                struct msghdr rtnl_reply;    /* generic msghdr structure */
                struct iovec io_reply;

                memset(&io_reply, 0, sizeof(io_reply));
                memset(&rtnl_reply, 0, sizeof(rtnl_reply));

                io.iov_base = reply;
                io.iov_len = IFLIST_REPLY_BUFFER;
                rtnl_reply.msg_iov = &io;
                rtnl_reply.msg_iovlen = 1;
                rtnl_reply.msg_name = &kernel;
                rtnl_reply.msg_namelen = sizeof(kernel);

                len = recvmsg(fd, &rtnl_reply, 0); /* read lots of data */
                if (len)
                {
                    for (msg_ptr = (struct nlmsghdr *) reply; NLMSG_OK(msg_ptr, len); msg_ptr = NLMSG_NEXT(msg_ptr, len))
                    {
                        switch(msg_ptr->nlmsg_type)
                        {
                        case 3:        /* this is the NLMSG_DONE end msg */
                            end++;
                            break;
                        case 16:    /* this is the RTM_NEWLINK msg */
#ifdef DONTDOTHIS
                            if (rtnl_get_link_ipv4addr(msg_ptr,"wlan0",&connection.sin_addr.s_addr))
                                LOGI("Yeah!!!");
                            else EHSH_LOG_ERROR("Could not find wlan for debugger");
#endif
                            break;
                        default:    /* for education only,
		                         should not happen here */
                            break;
                        }
                    }
                }
            }

        }
    }/*subscope */
    close(fd);
}
#endif /* EHS_BSDSOCKETS_HAVE_NETLINK - end of netlink-only functions */


/**
 * A fault has been detected with TCP/IP. Get an error message corresponding to the current problem.
 *
 * @param[in] nErrCode Code of the error for which we require the error text
 * @return pointer to the error text
 */
const ehs_char * EhsTgtTcp_getErrorText(ehs_char *buff, EhsTgtTcpErrType nErrCode)
{
#if 0
    int szError;
    if (buff != NULL)
    {
        /* Warning there is a GNU specific version of the following that returns a pointer instead of an error code
         * This may only result in a compiler warning so has been disabled.
         */
        szError = strerror_r(nErrCode,buff,EHS_STRING_LENGTH_MAX);
        return szError;
    }
    else return -1;
#else
    EhsStrcpy(buff,"DISABLED ERROR CODE TRANSLATION");
#endif
    return buff;
}

/**
 * Target-specific initialisation of Tcpip. Also waits until the main
 * EHS thread is up and running.
 *
 * @return true if initialisation was successful
 */
ehs_bool EhsTgtTcp_init(void)
{
    /* no special initialisation action is required */
    return EHS_TRUE;
}

/**
 * Target-specific shutdown for the TCP/IP connection
 */
void EhsTgtTcp_term(void)
{
    /* no specific action required here */
}

/**
 * Target-specific non-blocking receive of Tcpip.
 *
 * @param[in] xRxSocket Socket to receive data on
 * @param[out] pData Pointer to memory to contain received data
 * @param[in] nData Maximum amount of data to return
 * @return number of bytes returned or EHS_TGT_TCP_SOCKET_ERROR
 */
ehs_sint32 EhsTgtTcp_recvNonblock(EhsTgtTcpSocketType xRxSocket, ehs_uint8* pData, ehs_uint32 nData)
{
    ehs_sint32 nDataReceived;

    memset(pData, 0, (size_t)nData); /*lint !e534 Safe to ignore return value here */
    nDataReceived = recv(xRxSocket, (ehs_char*)pData, nData, MSG_DONTWAIT);
    if ( nDataReceived == 0 )   // This is actually what linux returns when the streamhas been closed nicely
    {
        nDataReceived = EHS_TGT_TCP_SOCKET_ERROR;
    }
    else if (nDataReceived == -1)    // notewe could also check errno for nwouldblock or eagain... , but apparently -1 return is usually the same and thread safe
    {
        nDataReceived = 0;
    }
    return nDataReceived;
}


ehs_bool EhsSvcTgtTcp_closeConnection(EhsTgtTcpSocketType EhsSvcTcpSocketConnection)
{
    ehs_bool bSuccess = EHS_FALSE; /* assume operation failed */
    int retVal;

    if ( EhsSvcTcpSocketConnection != EHS_TGT_TCP_INVALID_SOCKET)
    {
        // shutdown is only needed for TCP
        retVal = shutdown(EhsSvcTcpSocketConnection, EHS_TGT_TCP_SD_BOTH);
        EhsSleep(EHS_TIME_us(EHS_TGT_TCP_SUSPENDTIME_us));
        if(retVal == -1){
            //EHSH_LOG_ERROR("Failed to shutdown the socket");
        }
        retVal = EHS_TGT_TCP_CLOSE_SOCKET(EhsSvcTcpSocketConnection);

        if (retVal != 0)
        {
            if (EhsTgtTcp_getErrorCode(EHS_TRUE) != EHS_TGT_TCP_ERR_NOTCONN)
            {
                EHSH_LOG_ERROR("EhsSvcTcp_closeConnection.shutdown (%d)", EhsTgtTcp_getErrorCode(EHS_FALSE));
            }
            else
            {
                /* Ignore this warning, as we're already shutdown */
                bSuccess = EHS_TRUE;
            }
        }
        else
        {
            bSuccess = EHS_TRUE;
        }

    }

    return bSuccess; // return success if already closed too
}
