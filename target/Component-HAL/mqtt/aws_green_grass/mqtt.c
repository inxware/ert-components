
/* All this needs to go into a target HAL layer */
#include <aws/io/channel.h>
#include <aws/io/channel_bootstrap.h>
#include <aws/io/event_loop.h>
#include <aws/io/logging.h>
#include <aws/io/socket.h>
#include <aws/io/socket_channel_handler.h>
#include <aws/io/tls_channel_handler.h>
#include <aws/common/allocator.h>
#include <aws/common/condition_variable.h>
#include <aws/common/mutex.h>
// #include <aws/io/io.h>
#include <aws/mqtt/mqtt.h>
#include <aws/mqtt/client.h>

#include "target_types.h"
#include "globals.h"
#include "hal-api.h"
#include "hal_mqtt.h"

/* 
  todo 2023 This should go in a shared MQTT header
*/
typedef enum
{
    EHS_MQTT_THREAD_STATE_INIT,
    EHS_MQTT_THREAD_STATE_CONNECTING,
    EHS_MQTT_THREAD_STATE_CONNECTED,
    EHS_MQTT_THREAD_STATE_DISCONNECTING,
    EHS_MQTT_THREAD_STATE_DISCONNECTED,
    EHS_MQTT_THREAD_STATE_INTERRUPTED
} EhsMQTTThreadState;

/*
    todo - not sure why this is called the test_....?
*/
struct test_context
{
    uint32_t connections;
    struct aws_allocator *allocator;
    struct aws_mutex lock;
    struct aws_tls_ctx *tls_ctx;
    struct aws_event_loop_group *el_group;
    struct aws_host_resolver *resolver;
    struct aws_client_bootstrap *bootstrap;
    struct aws_mqtt_client *client;
    struct aws_mqtt_client_connection *connection;
    struct aws_tls_connection_options tls_connection_options;
    EhsMQTTThreadState threadState;
    struct aws_condition_variable condition_variable;
};

ehs_char mqtt_error_msg_buffer[128];

/* 
todo2023 - shouldn't we be using this?
*/
static void clean_connection(struct test_context* tester)
{
    /*if(tester->connection){
    	aws_mqtt_client_connection_release(tester->connection);
    	tester->connection=NULL;
    }*/
    /*if(tester->client){
    	size_t refs = aws_atomic_fetch_sub(&tester->client->ref_count.ref_count, 1);
    	if(refs>0){
    		refs=aws_ref_count_release(&tester->client->ref_count);
    	}
    	if(refs==0){
    		tester->client=NULL;
    	}
    }*/
}

static void s_mqtt_on_connection_complete(
    struct aws_mqtt_client_connection *connection,
    int error_code,
    enum aws_mqtt_connect_return_code return_code,
    bool session_present,
    void *userdata)
{

    (void)connection;
    (void)error_code;
    (void)return_code;
    (void)session_present;

    struct test_context *tester = userdata;
    tester->connections--;
    if(error_code != AWS_ERROR_SUCCESS || return_code != AWS_MQTT_CONNECT_ACCEPTED || session_present != false)
    {
        tester->threadState=EHS_MQTT_THREAD_STATE_DISCONNECTED;
        clean_connection(tester);
        aws_condition_variable_notify_one(&tester->condition_variable);
        EhsSprintf(mqtt_error_msg_buffer, "falied to connect (error code : %d)", error_code);
        EhsMQTTReportError(mqtt_error_msg_buffer);
        return;
    }

    aws_mutex_lock(&tester->lock);
    tester->threadState=EHS_MQTT_THREAD_STATE_CONNECTED;
    EhsMQTTConnectEvent(true);
    aws_mutex_unlock(&tester->lock);
}

static void s_on_connection_interrupted(struct aws_mqtt_client_connection *connection, int error_code, void *userdata)
{

    (void)connection;
    struct test_context* tester=userdata;
    aws_mutex_lock(&tester->lock);
    tester->threadState = EHS_MQTT_THREAD_STATE_INTERRUPTED;
    EhsMQTTConnectEvent(false);
    aws_mutex_unlock(&tester->lock);
}

static void s_on_resubscribed(
    struct aws_mqtt_client_connection *connection,
    uint16_t packet_id,
    const struct aws_array_list *topic_subacks, /* contains aws_mqtt_topic_subscription pointers */
    int error_code,
    void *userdata)
{

    (void)connection;
    (void)packet_id;
    (void)userdata;

    if(error_code == AWS_ERROR_SUCCESS)
    {
        size_t num_topics = aws_array_list_length(topic_subacks);
        for (size_t i = 0; i < num_topics; ++i)
        {
            struct aws_mqtt_topic_subscription sub_i;
            aws_array_list_get_at(topic_subacks, &sub_i, i);
            //AWS_FATAL_ASSERT(sub_i.qos != AWS_MQTT_QOS_FAILURE);
        }
    }
    else
    {
        // TODO resubscribe failed, what do we do here?
    }
}

static void s_on_connection_resumed(
    struct aws_mqtt_client_connection *connection,
    enum aws_mqtt_connect_return_code return_code,
    bool session_present,
    void *userdata)
{

    (void)connection;
    struct test_context *tester = userdata;
    tester->threadState=EHS_MQTT_THREAD_STATE_CONNECTED;
    if (!session_present)
    {
        uint16_t packet_id = aws_mqtt_resubscribe_existing_topics(connection, s_on_resubscribed, NULL);
        //AWS_FATAL_ASSERT(packet_id);
    }
}

static void s_mqtt_on_disconnect(struct aws_mqtt_client_connection *connection, void *userdata)
{
    (void)connection;

    struct test_context *tester = userdata;
    clean_connection(tester);
    aws_mutex_lock(&tester->lock);
    tester->threadState = EHS_MQTT_THREAD_STATE_DISCONNECTED;
    EhsMQTTConnectEvent(false);
    aws_mutex_unlock(&tester->lock);
    aws_condition_variable_notify_one(&tester->condition_variable);
}

/*static void s_on_puback(
    struct aws_mqtt_client_connection *connection,
    uint16_t packet_id,
    int error_code,
    void *userdata) {

    (void)connection;
    (void)packet_id;
    (void)error_code;

    AWS_FATAL_ASSERT(error_code == AWS_ERROR_SUCCESS);

    struct test_context *tester = userdata;
}*/

static void on_bootstrap_shutdown_complete(void* userdata)
{
    struct test_context *tester = userdata;
    aws_condition_variable_notify_one(&tester->condition_variable);
}

static void s_on_packet_received(
    struct aws_mqtt_client_connection *connection,
    const struct aws_byte_cursor *topic,
    const struct aws_byte_cursor *payload,
    bool dup,
    enum aws_mqtt_qos qos,
    bool retain,
    void *userdata)
{
    ehs_char topicStr[EHS_STRING_LENGTH_MAX];
    (void)connection;
    (void)topic;
    (void)dup;
    (void)qos;
    (void)retain;

    //AWS_FATAL_ASSERT(payload->len == PAYLOAD_LEN);
    //AWS_FATAL_ASSERT(0 == memcmp(payload->ptr, s_payload, PAYLOAD_LEN));

    bool notify = false;
    struct test_context *tester = userdata;

    if(topic->len<sizeof(topicStr) && payload->len<EHS_MQTT_SUBS_RECV_BUFF_MAX_LENGTH)
    {
        memcpy(topicStr,topic->ptr,topic->len);
        topicStr[topic->len]='\0';
        aws_mutex_lock(&tester->lock);
        EhsMQTTSubscribeEvent(topicStr,(char*)payload->ptr,payload->len);
        aws_mutex_unlock(&tester->lock);
    }
}

struct aws_allocator *allocator = NULL;
struct aws_logger logger= {0};

void* EhsMqttClientLoop(void* args)
{
    EhsMqttDevmanMon_t* pEhsMqttDevmanMon = (EhsMqttDevmanMon_t*)args;
    struct aws_byte_cursor topic_cur;
    ehs_bool ignore_sodl_change=EHS_FALSE;
    ehs_bool connect=EHS_FALSE;
    ehs_bool subscribe=EHS_FALSE;
    char* host=NULL;
    ehs_uint16 port=0;
    ehs_bool tls=EHS_FALSE;
    char* clientid=NULL;
    char* username=NULL;
    char* password=NULL;
    char* clientCertFileName=NULL;
    char* clientKeyFileName=NULL;
    char* rootCAFileName=NULL;
    ehs_uint8 qos;
    ehs_char topic[EHS_STRING_LENGTH_MAX];
    ehs_char message[EHS_STRING_LENGTH_MAX];
    ehs_char canonicalAppdataPath[EHS_STRING_LENGTH_MAX];
    ehs_char canonicalClientCertFileName[EHS_STRING_LENGTH_MAX];
    ehs_char canonicalClientKeyFileName[EHS_STRING_LENGTH_MAX];
    ehs_char canonicalRootCAFileName[EHS_STRING_LENGTH_MAX];
    if(allocator==NULL)
    {
        allocator = aws_mem_tracer_new(aws_default_allocator(), NULL, AWS_MEMTRACE_BYTES, 0);
        aws_mqtt_library_init(allocator);
        struct aws_logger_standard_options logger_options =
        {
            .level = AWS_LL_ERROR,
            .file = stdout,
        };
        aws_logger_init_standard(&logger, allocator, &logger_options);
        aws_logger_set(&logger);
    }
    struct test_context tester;
    AWS_ZERO_STRUCT(tester);
    tester.allocator = allocator;
    aws_mutex_init(&tester.lock);
    aws_condition_variable_init(&tester.condition_variable);
    tester.el_group = aws_event_loop_group_new_default(allocator, 1, NULL);
    struct aws_host_resolver_default_options resolver_options =
    {
        .el_group = tester.el_group,
        .max_entries = 8,
    };
    tester.resolver = aws_host_resolver_new_default(allocator, &resolver_options);

    struct aws_client_bootstrap_options bootstrap_options =
    {
        .event_loop_group = tester.el_group,
        .host_resolver = tester.resolver,
        .on_shutdown_complete = on_bootstrap_shutdown_complete,
        .user_data = &tester,
    };
    tester.bootstrap = aws_client_bootstrap_new(allocator, &bootstrap_options);
    struct aws_tls_ctx_options tls_ctx_opt;
    ehs_bool run=EHS_TRUE;
    tester.connections=0;
    tester.client = aws_mqtt_client_new(allocator, tester.bootstrap);
    while(run)
    {
        // run devman mon loop if configured to be done using mqtt
        if(pEhsMqttDevmanMon && pEhsMqttDevmanMon->pMqttDevmanMonLoop){
            pEhsMqttDevmanMon->pMqttDevmanMonLoop(NULL);
        }
        ehs_bool isDevmanMon = (pEhsMqttDevmanMon != NULL) ? EHS_TRUE : EHS_FALSE;
        
        EhsMQTTConnectPoll(&connect,&host,&port,&tls,&clientid,&username,
                           &password,&clientCertFileName,&clientKeyFileName,&rootCAFileName);
        switch(tester.threadState)
        {
        case EHS_MQTT_THREAD_STATE_CONNECTING:
        case EHS_MQTT_THREAD_STATE_DISCONNECTING:
            //ignore these
            break;
        case EHS_MQTT_THREAD_STATE_INTERRUPTED:
            tester.threadState=EHS_MQTT_THREAD_STATE_DISCONNECTING;
            if(aws_mqtt_client_connection_disconnect(tester.connection, s_mqtt_on_disconnect, &tester))
            {
                tester.threadState=EHS_MQTT_THREAD_STATE_DISCONNECTED;
                clean_connection(&tester);
            }
            break;
        case EHS_MQTT_THREAD_STATE_INIT:
            if(connect)
            {
                tester.threadState=EHS_MQTT_THREAD_STATE_CONNECTING;
                // client id must be specified !
                if(clientid == NULL || EhsStrlen(clientid) <= 0){
                    EhsMQTTReportError("clientId is not specified");
                    break;
                }
                // host must be specified !
                if(host == NULL || EhsStrlen(host) <= 0){
                    EhsMQTTReportError("host url is not specified");
                    break;
                }
                // port must be specified !
                if(port == 0){
                    EhsMQTTReportError("port must be specified and larger than 0");
                    break;
                }
                if(tls==EHS_TRUE)
                {
                    ehs_bool hasClientCert = EHS_FALSE;
                    // get mqtt broker certificates directory
                    if(isDevmanMon == EHS_TRUE){
                        EhsTF_tryCanonicPath(canonicalAppdataPath, EHS_RUNTIME_DEVMAN_DIR, "core/certs", EHS_TRUE);
                    }else{
                        EhsHMetagetCurrentAppDir(canonicalAppdataPath);
                    }
                    
                    canonicalClientCertFileName[0]='\0';
                    if(clientCertFileName != NULL && clientCertFileName[0] != '\0'){
                        EhsStrcat(canonicalClientCertFileName,canonicalAppdataPath);
                        EhsStrcat(canonicalClientCertFileName,EHS_TD_FILES_SEPARATOR_STR);
                        EhsStrcat(canonicalClientCertFileName,clientCertFileName);
                        // check if certificate exists
                        if(EhsTF_exists(canonicalClientCertFileName) == 0){
                            printf("File (%s) does NOT exist !\n", clientCertFileName);
                            EhsMQTTReportError("cannot find client cert");
                            break;
                        }
                        hasClientCert = EHS_TRUE;
                    }
                    ehs_bool hasClientKey = EHS_FALSE;
                    canonicalClientKeyFileName[0]='\0';
                    if(clientKeyFileName != NULL && clientKeyFileName[0] != '\0'){
                        EhsStrcat(canonicalClientKeyFileName,canonicalAppdataPath);
                        EhsStrcat(canonicalClientKeyFileName,EHS_TD_FILES_SEPARATOR_STR);
                        EhsStrcat(canonicalClientKeyFileName,clientKeyFileName);
                        // check if certificate exists
                        if(EhsTF_exists(canonicalClientKeyFileName) == 0){
                            printf("File (%s) does NOT exist !\n", clientKeyFileName);
                            EhsMQTTReportError("cannot find client key");
                            break;
                        }
                        hasClientKey = EHS_TRUE;
                    }
                    ehs_bool hasCaCert = EHS_FALSE;
                    canonicalRootCAFileName[0]='\0';
                    if(rootCAFileName != NULL && rootCAFileName[0] != '\0'){
                        EhsStrcat(canonicalRootCAFileName,canonicalAppdataPath);
                        EhsStrcat(canonicalRootCAFileName,EHS_TD_FILES_SEPARATOR_STR);
                        EhsStrcat(canonicalRootCAFileName,rootCAFileName);
                        // check if certificate exists
                        if(EhsTF_exists(canonicalRootCAFileName) == 0){
                            printf("File (%s) does NOT exist !\n", rootCAFileName);
                            EhsMQTTReportError("cannot find ca cert");
                            break;
                        }
                        hasCaCert = EHS_TRUE;
                    }

                    ehs_sint32 aws_ret = AWS_OP_SUCCESS;
                    if(hasClientCert == EHS_TRUE || hasClientKey == EHS_TRUE){
                        aws_ret = aws_tls_ctx_options_init_client_mtls_from_path(&tls_ctx_opt, allocator, canonicalClientCertFileName, canonicalClientKeyFileName);
                    }else{
                        aws_tls_ctx_options_init_default_client(&tls_ctx_opt, allocator);
                    }

                    if(AWS_OP_SUCCESS == aws_ret)
                    {
                        if(AWS_OP_SUCCESS == aws_tls_ctx_options_set_alpn_list(&tls_ctx_opt, "x-amzn-mqtt-ca"))
                        {
                            if(hasCaCert == EHS_FALSE || AWS_OP_SUCCESS == aws_tls_ctx_options_override_default_trust_store_from_path(&tls_ctx_opt,NULL,canonicalRootCAFileName))
                            {
                                tester.tls_ctx = aws_tls_client_ctx_new(allocator, &tls_ctx_opt);
                                if(tester.tls_ctx != NULL)
                                {
                                    aws_tls_ctx_options_clean_up(&tls_ctx_opt);
                                    aws_tls_connection_options_init_from_ctx(&tester.tls_connection_options, tester.tls_ctx);
                                }
                                else
                                {
                                    EhsMQTTReportError("failed to create client context");
                                    break;
                                }
                            }
                            else
                            {
                                EhsMQTTReportError("failed to store ca cert");
                                break;
                            }
                        }
                        else
                        {
                            EhsMQTTReportError("failed to initalise tls options");
                            break;
                        }
                    }
                    else
                    {
                        EhsMQTTReportError("failed to initalise client cert and key");
                        break;
                    }
                }//if(tls==EHS_TRUE)
                tester.connection = aws_mqtt_client_connection_new(tester.client);
                struct aws_socket_options socket_options;
                AWS_ZERO_STRUCT(socket_options);
                socket_options.connect_timeout_ms = 3000;
                socket_options.type = AWS_SOCKET_STREAM;
                socket_options.domain = AWS_SOCKET_IPV6;
                struct aws_byte_cursor host_name_cur = aws_byte_cursor_from_c_str(host);
                struct aws_byte_cursor client_id_cur = aws_byte_cursor_from_c_str(clientid);
                struct aws_mqtt_connection_options conn_options =
                {
                    .host_name = host_name_cur,
                    .port = port,
                    .socket_options = &socket_options,
                    .tls_options = &tester.tls_connection_options,
                    .client_id = client_id_cur,
                    .keep_alive_time_secs = 30,
                    .ping_timeout_ms = 3000,
                    .on_connection_complete = s_mqtt_on_connection_complete,
                    .user_data = &tester,
                    .clean_session = true,
                };
                if(tls==EHS_FALSE)
                {
                    conn_options.tls_options=NULL;
                }
                if(AWS_OP_SUCCESS == aws_mqtt_client_connection_set_connection_interruption_handlers(tester.connection, s_on_connection_interrupted, &tester, s_on_connection_resumed, &tester))
                {
                    if((username && strlen(username)>0) || (password && strlen(password)>0))
                    {
                        struct aws_byte_cursor username_cur = aws_byte_cursor_from_c_str(username);
                        struct aws_byte_cursor password_cur = aws_byte_cursor_from_c_str(password);
                        aws_mqtt_client_connection_set_login(
                            tester.connection,
                            &username_cur,
                            &password_cur);
                    }
                    if(AWS_OP_SUCCESS == aws_mqtt_client_connection_connect(tester.connection, &conn_options))
                    {
                        tester.connections++;
                    }
                    else
                    {
                        clean_connection(&tester);
                    }
                }
                else
                {
                    //TODO failed to set interruption handlers
                }
            }
            break;
        case EHS_MQTT_THREAD_STATE_CONNECTED:
            if(connect)
            {
                if(EhsMQTTPublishWritePoll(topic,message,&qos)==EHS_TRUE)
                {
                    topic_cur = aws_byte_cursor_from_c_str(topic);
                    struct aws_byte_cursor message_cur = aws_byte_cursor_from_c_str(message);
                    aws_mqtt_client_connection_publish(
                        tester.connection,
                        &topic_cur,
                        qos,
                        false,
                        &message_cur,
                        NULL, //&s_on_puback,
                        &tester
                    );
                }
                if(EhsMQTTSubscribeWritePoll(topic,&subscribe,&qos)==EHS_TRUE)
                {
                    topic_cur = aws_byte_cursor_from_c_str(topic);
                    if(subscribe==EHS_TRUE)
                    {
                        aws_mqtt_client_connection_subscribe(
                            tester.connection,
                            &topic_cur,
                            qos,
                            &s_on_packet_received,
                            &tester,
                            NULL,
                            NULL,
                            &tester
                        );
                    }
                    else
                    {
                        aws_mqtt_client_connection_unsubscribe(
                            tester.connection,
                            &topic_cur,
                            NULL,
                            NULL);
                    }
                }
            }
            else
            {
                tester.threadState=EHS_MQTT_THREAD_STATE_DISCONNECTING;
                if(aws_mqtt_client_connection_disconnect(tester.connection, s_mqtt_on_disconnect, &tester))
                {
                    tester.threadState=EHS_MQTT_THREAD_STATE_DISCONNECTED;
                    clean_connection(&tester);
                }
            }
            break;
        case EHS_MQTT_THREAD_STATE_DISCONNECTED:
            aws_tls_connection_options_clean_up(&tester.tls_connection_options);
            clean_connection(&tester);
            aws_tls_ctx_release(tester.tls_ctx);
            tester.tls_ctx=NULL;
            tester.threadState=EHS_MQTT_THREAD_STATE_INIT;
            if (isDevmanMon == EHS_FALSE && *bNewSodlFlagRef == EHS_TRUE)
            {
                run=EHS_FALSE;
            }
            break;
        }
        EhsSleep(EHS_TIME_ms(100));
        if (isDevmanMon == EHS_FALSE && *bNewSodlFlagRef == EHS_TRUE)
        {
            switch(tester.threadState)
            {
            case EHS_MQTT_THREAD_STATE_CONNECTED:
                tester.threadState=EHS_MQTT_THREAD_STATE_DISCONNECTING;
                aws_mqtt_client_connection_disconnect(tester.connection, s_mqtt_on_disconnect, &tester);
                break;
            default:
                run=EHS_FALSE;
                break;
            }
        }
    }
    while(tester.connections>0)
    {
        aws_mutex_lock(&tester.lock);
        aws_condition_variable_wait(&tester.condition_variable,&tester.lock);
        aws_mutex_unlock(&tester.lock);
    }
    /*if(tester.connection){
    	if(AWS_OP_SUCCESS == aws_mqtt_client_connection_disconnect(tester.connection, s_mqtt_on_disconnect, &tester)){
    		aws_condition_variable_wait(&tester.condition_variable, &tester.lock);
    	}
    	aws_mqtt_client_connection_release(tester.connection);
    }*/
    if(tester.client)
    {
        aws_mqtt_client_release(tester.client);
    }
    if(tester.tls_ctx)
    {
        aws_tls_ctx_release(tester.tls_ctx);
    }
    //this is here twice because we end up with two references to the bootstrap
    size_t refs=aws_ref_count_release(&tester.bootstrap->ref_count);
    while(refs>0)
    {
        refs=aws_ref_count_release(&tester.bootstrap->ref_count);
    }
    aws_host_resolver_release(tester.resolver);
    aws_event_loop_group_release(tester.el_group);
    aws_thread_join_all_managed();
    aws_mutex_clean_up(&tester.lock);
    aws_condition_variable_clean_up(&tester.condition_variable);

    return NULL;
}
