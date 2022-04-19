//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-mqtt_client.h"
#include "inx-mqtt_subscribe.h"
#include "inx-mqtt_publish.h"
#include "ehs_main.h" // we run th main from here!
//ICB HEADER MACRO END -- DO NOT ALTER


#define INX_MQTT_PUBLISH_FIFO_SIZE 1
typedef struct inx_mqtt_client_state_mine
{
    ehs_char clientCertFileName[13];
    ehs_char clientKeyFileName[13];
    ehs_char rootCAFileName[13];
    ehs_bool connect;
    EhsFunctionInstanceDataType* pFIdata;
    struct inx_mqtt_client_state_mine* pNext;
    struct inx_mqtt_client_state_mine* pPrev;
} inx_mqtt_client_state_type_mine;

//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct
{
	ehs_char* client_cert;
	ehs_char* client_key;
	ehs_char* server_cert;
} inx_mqtt_client_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER
static inx_mqtt_client_state_type_mine* gpFirstWidget=NULL;
static inx_mqtt_client_state_type_mine* inxMQTTSubscribeGetLastWidget(){
	inx_mqtt_client_state_type_mine* widget=gpFirstWidget;
	while(widget!=NULL && widget->pNext!=NULL){
		widget=widget->pNext;
		if(widget==widget->pNext){
			EHSH_LOG_ERROR("inxMQTTSubscribeGetLastWidget infinite loop found");
			widget->pNext=NULL;
		}
	}
	return widget;
}
static void inxMQTTSubscribeRegisterWidget(inx_mqtt_client_state_type_mine* pState){
	if(gpFirstWidget==NULL){
		gpFirstWidget=pState;
		return;
	}

	inx_mqtt_client_state_type_mine* lastWidget=inxMQTTSubscribeGetLastWidget();
	if(lastWidget==NULL){
		gpFirstWidget=pState;
	}else{
		lastWidget->pNext=pState;
		pState->pPrev=lastWidget;
	}
}

/*static void inxMQTTSubscribeRemoveWidget(inx_mqtt_client_state_type_mine* pState){
	if(pState->pPrev!=NULL){
		pState->pPrev->pNext=pState->pNext;
	}
	if(pState->pNext!=NULL){
		pState->pNext->pPrev=pState->pPrev;
	}
	//do we need to update the first entry pointer?
	if(gpFirstWidget==pState){
		gpFirstWidget=pState->pNext;
	}
}*/
//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(mqtt_client)
EHS_FB_FUNCTION_ENTRY("connect", mqtt_client_connect)
EHS_FB_FUNCTION_ENTRY("disconnect", mqtt_client_disconnect)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_mqtt_client_ARG_connect_password 1
#define INX_mqtt_client_ARG_connect_tls 2
#define INX_mqtt_client_ARG_connect_username 3
#define INX_mqtt_client_ARG_connect_clientid 4
#define INX_mqtt_client_ARG_connect_port 5
#define INX_mqtt_client_ARG_connect_host 6
#define INX_mqtt_client_ARG_connect_finishdisconnect 1
#define INX_mqtt_client_ARG_connect_finishconnect 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
#ifdef EHS_MINGW
#define EHS_MQTT_CLIENT_EXPORT __declspec(dllexport)
#else
#define EHS_MQTT_CLIENT_EXPORT // nothing
#endif
EHS_MQTT_CLIENT_EXPORT void EhsMQTTConnectEvent(ehs_bool connect);
EHS_MQTT_CLIENT_EXPORT void EhsMQTTConnectPoll(ehs_bool* connect,char** host,uint16_t* pPort,uint8_t* pTLS,char** clientid,char** username,char** password,char** clientCertFileName,char** clientKeyFileName,char** rootCAFileName);
#if EHS_LWIP
#else //if EHS_LWIP
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

typedef enum {
	EHS_MQTT_THREAD_STATE_INIT,
	EHS_MQTT_THREAD_STATE_CONNECTING,
	EHS_MQTT_THREAD_STATE_CONNECTED,
	EHS_MQTT_THREAD_STATE_DISCONNECTING,
	EHS_MQTT_THREAD_STATE_DISCONNECTED,
	EHS_MQTT_THREAD_STATE_INTERRUPTED
} EhsMQTTThreadState;

	struct test_context {
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

static void clean_connection(struct test_context* tester){
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
    void *userdata) {

    (void)connection;
    (void)error_code;
    (void)return_code;
    (void)session_present;

	struct test_context *tester = userdata;
	tester->connections--;
    if(error_code != AWS_ERROR_SUCCESS || return_code != AWS_MQTT_CONNECT_ACCEPTED || session_present != false){
		tester->threadState=EHS_MQTT_THREAD_STATE_DISCONNECTED;
		clean_connection(tester);
		aws_condition_variable_notify_one(&tester->condition_variable);
		return;
	}

    aws_mutex_lock(&tester->lock);
	tester->threadState=EHS_MQTT_THREAD_STATE_CONNECTED;
	EhsMQTTConnectEvent(true);
    aws_mutex_unlock(&tester->lock);
}

static void s_on_connection_interrupted(struct aws_mqtt_client_connection *connection, int error_code, void *userdata) {

    (void)connection;
    struct test_context* tester=userdata;
    //printf("CONNECTION INTERRUPTED error_code=%d\n", error_code);
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
    void *userdata) {

    (void)connection;
    (void)packet_id;
    (void)userdata;

    if(error_code == AWS_ERROR_SUCCESS){
		size_t num_topics = aws_array_list_length(topic_subacks);
		for (size_t i = 0; i < num_topics; ++i) {
			struct aws_mqtt_topic_subscription sub_i;
			aws_array_list_get_at(topic_subacks, &sub_i, i);
			//AWS_FATAL_ASSERT(sub_i.qos != AWS_MQTT_QOS_FAILURE);
		}
	}else{
		// TODO resubscribe failed, what do we do here?
	}
}

static void s_on_connection_resumed(
    struct aws_mqtt_client_connection *connection,
    enum aws_mqtt_connect_return_code return_code,
    bool session_present,
    void *userdata) {

    (void)connection;
	struct test_context *tester = userdata;
	tester->threadState=EHS_MQTT_THREAD_STATE_CONNECTED;
    //printf("CONNECTION RESUMED return_code=%d session_present=%d\n", return_code, session_present);
    if (!session_present) {
        //printf("RESUBSCRIBING...");
        uint16_t packet_id = aws_mqtt_resubscribe_existing_topics(connection, s_on_resubscribed, NULL);
        //AWS_FATAL_ASSERT(packet_id);
    }
}

static void s_mqtt_on_disconnect(struct aws_mqtt_client_connection *connection, void *userdata) {
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

static void on_bootstrap_shutdown_complete(void* userdata){
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
    void *userdata) {
		ehs_char topicStr[EHS_STRING_LENGTH_MAX];
		ehs_char payloadStr[EHS_STRING_LENGTH_MAX];
    (void)connection;
    (void)topic;
    (void)dup;
    (void)qos;
    (void)retain;

    //AWS_FATAL_ASSERT(payload->len == PAYLOAD_LEN);
    //AWS_FATAL_ASSERT(0 == memcmp(payload->ptr, s_payload, PAYLOAD_LEN));

    bool notify = false;
    struct test_context *tester = userdata;
	
	if(topic->len<sizeof(topicStr) && payload->len<sizeof(payloadStr)){
		memcpy(topicStr,topic->ptr,topic->len);
		topicStr[topic->len]='\0';
		memcpy(payloadStr,payload->ptr,payload->len);
		payloadStr[payload->len]='\0';
		aws_mutex_lock(&tester->lock);
		EhsMQTTSubscribeEvent(topicStr,payloadStr);
		aws_mutex_unlock(&tester->lock);
	}
}

struct aws_allocator *allocator = NULL;
struct aws_logger logger={0};

	EHS_FB_THREAD_FUNCTION(mqtt_client_thread) {
		Ehs_FB_ThreadStarted();
		struct aws_byte_cursor topic_cur;
		ehs_bool connect=EHS_FALSE;
		ehs_bool subscribe=EHS_FALSE;
		char* host=NULL;
		uint16_t port=0;
		uint8_t tls=0;
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
		if(allocator==NULL){
			allocator = aws_mem_tracer_new(aws_default_allocator(), NULL, AWS_MEMTRACE_BYTES, 0);
			aws_mqtt_library_init(allocator);
			struct aws_logger_standard_options logger_options = {
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
		struct aws_host_resolver_default_options resolver_options = {
			.el_group = tester.el_group,
			.max_entries = 8,
		};
		tester.resolver = aws_host_resolver_new_default(allocator, &resolver_options);

		struct aws_client_bootstrap_options bootstrap_options = {
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
		while(run){
			EhsMQTTConnectPoll(&connect,&host,&port,&tls,&clientid,&username,
			&password,&clientCertFileName,&clientKeyFileName,&rootCAFileName);
			switch(tester.threadState){
				case EHS_MQTT_THREAD_STATE_CONNECTING:
				case EHS_MQTT_THREAD_STATE_DISCONNECTING:
					//ignore these
					break;
				case EHS_MQTT_THREAD_STATE_INTERRUPTED:
					tester.threadState=EHS_MQTT_THREAD_STATE_DISCONNECTING;
					if(aws_mqtt_client_connection_disconnect(tester.connection, s_mqtt_on_disconnect, &tester)){
						tester.threadState=EHS_MQTT_THREAD_STATE_DISCONNECTED;
						clean_connection(&tester);
					}
					break;
				case EHS_MQTT_THREAD_STATE_INIT:
					if(connect){
						tester.threadState=EHS_MQTT_THREAD_STATE_CONNECTING;
						if(tls==EHS_TRUE){
							EhsHMetagetCurrentAppDir(canonicalAppdataPath);
							canonicalClientCertFileName[0]='\0';
							EhsStrcat(canonicalClientCertFileName,canonicalAppdataPath);
							EhsStrcat(canonicalClientCertFileName,EHS_TD_FILES_SEPARATOR_STR);
							EhsStrcat(canonicalClientCertFileName,clientCertFileName);
							canonicalClientKeyFileName[0]='\0';
							EhsStrcat(canonicalClientKeyFileName,canonicalAppdataPath);
							EhsStrcat(canonicalClientKeyFileName,EHS_TD_FILES_SEPARATOR_STR);
							EhsStrcat(canonicalClientKeyFileName,clientKeyFileName);
							canonicalRootCAFileName[0]='\0';
							EhsStrcat(canonicalRootCAFileName,canonicalAppdataPath);
							EhsStrcat(canonicalRootCAFileName,EHS_TD_FILES_SEPARATOR_STR);
							EhsStrcat(canonicalRootCAFileName,rootCAFileName);
							if(AWS_OP_SUCCESS == aws_tls_ctx_options_init_client_mtls_from_path(&tls_ctx_opt, allocator, canonicalClientCertFileName,canonicalClientKeyFileName)){
								if(AWS_OP_SUCCESS == aws_tls_ctx_options_set_alpn_list(&tls_ctx_opt, "x-amzn-mqtt-ca")){
									if(AWS_OP_SUCCESS == aws_tls_ctx_options_override_default_trust_store_from_path(&tls_ctx_opt,NULL,canonicalRootCAFileName)){
										tester.tls_ctx = aws_tls_client_ctx_new(allocator, &tls_ctx_opt);
										if(tester.tls_ctx != NULL){
											aws_tls_ctx_options_clean_up(&tls_ctx_opt);
											aws_tls_connection_options_init_from_ctx(&tester.tls_connection_options, tester.tls_ctx);
										}else{
											//TODO tls_ctx is NULL
											break;
										}
									}else{
										//TODO failed to init certs
										break;
									}
								}else{
									//TODO failed to init TLS opts
									break;
								}
							}else{
								//TODO failed to init client mtls
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
						struct aws_mqtt_connection_options conn_options = {
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
						if(tls==EHS_FALSE){
							conn_options.tls_options=NULL;
						}
						if(AWS_OP_SUCCESS == aws_mqtt_client_connection_set_connection_interruption_handlers(tester.connection, s_on_connection_interrupted, &tester, s_on_connection_resumed, &tester)){
							if((username && strlen(username)>0) || (password && strlen(password)>0)){
								struct aws_byte_cursor username_cur = aws_byte_cursor_from_c_str(username);
								struct aws_byte_cursor password_cur = aws_byte_cursor_from_c_str(password);
								aws_mqtt_client_connection_set_login(
									tester.connection,
									&username_cur,
									&password_cur);
							}
							if(AWS_OP_SUCCESS == aws_mqtt_client_connection_connect(tester.connection, &conn_options)){
								tester.connections++;
							}else{
								clean_connection(&tester);
							}
						}else{
							//TODO failed to set interruption handlers
						}																
					}
					break;
				case EHS_MQTT_THREAD_STATE_CONNECTED:
					if(connect){
						if(EhsMQTTPublishWritePoll(topic,message,&qos)==EHS_TRUE){
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
						}else if(EhsMQTTSubscribeWritePoll(topic,&subscribe,&qos)==EHS_TRUE){
							topic_cur = aws_byte_cursor_from_c_str(topic);
							if(subscribe==EHS_TRUE){
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
							}else{
								aws_mqtt_client_connection_unsubscribe(
    								tester.connection,
    								&topic_cur,
									NULL,
    								NULL);
							}
						}
					}else{
						tester.threadState=EHS_MQTT_THREAD_STATE_DISCONNECTING;
						if(aws_mqtt_client_connection_disconnect(tester.connection, s_mqtt_on_disconnect, &tester)){
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
					if (*bNewSodlFlagRef == EHS_TRUE) {
						run=EHS_FALSE;
					}
					break;
			}
			EhsSleep(EHS_TIME_s(1));
			if (*bNewSodlFlagRef == EHS_TRUE) {
				switch(tester.threadState){
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
		while(tester.connections>0){
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
		if(tester.client){
			aws_mqtt_client_release(tester.client);
		}
		if(tester.tls_ctx){
			aws_tls_ctx_release(tester.tls_ctx);
		}
		//this is here twice because we end up with two references to the bootstrap
		size_t refs=aws_ref_count_release(&tester.bootstrap->ref_count);
		while(refs>0){
			refs=aws_ref_count_release(&tester.bootstrap->ref_count);
		}
		aws_host_resolver_release(tester.resolver);
		aws_event_loop_group_release(tester.el_group);
		aws_thread_join_all_managed();
		aws_mutex_clean_up(&tester.lock);
		aws_condition_variable_clean_up(&tester.condition_variable);
		Ehs_FB_ThreadComplete();
		EhsHThread_exit();
	}
#endif //else if EHS_LWIP
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_mqtt_client_client_cert client.pem
#define INX_FB_mqtt_client_client_key client.key
#define INX_FB_mqtt_client_server_cert server.crt
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(mqtt_client)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_char* client_cert;
	ehs_char* client_key;
	ehs_char* server_cert;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%s %s %s",&client cert,&client key,&server cert); */
    /*
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_mqtt_client_state_type);
     */
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_mqtt_client_state_type_mine);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(mqtt_client)
{
    /*
	ehs_char* client_cert;
	ehs_char* client_key;
	ehs_char* server_cert;
     */
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
    /*
	inx_mqtt_client_state_type* inx_mqtt_client_state = (inx_mqtt_client_state_type*)EHS_FB_INIT_CONTEXT;
     */
	/* read the initialisation parameters */
	/*
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%s %s %s",&client cert,&client key,&server cert);
*/
	/* Add any further intialisation code here */

	inx_mqtt_client_state_type_mine* inx_mqtt_client_state = (inx_mqtt_client_state_type_mine*)EHS_FB_INIT_CONTEXT;
	inx_mqtt_client_state->pFIdata=NULL;
	inx_mqtt_client_state->connect=0;
	inx_mqtt_client_state->pNext=NULL;
	inx_mqtt_client_state->pPrev=NULL;
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%12s%12s%12s",inx_mqtt_client_state->clientCertFileName,inx_mqtt_client_state->clientKeyFileName,inx_mqtt_client_state->rootCAFileName);
	/* Add any further intialisation code here */
	EhsTPMutex_lock(EhsTPMutex_fbIO);
	inxMQTTSubscribeRegisterWidget(inx_mqtt_client_state);
	EhsTPMutex_unlock(EhsTPMutex_fbIO);
	#if EHS_LWIP
	#else //if EHS_LWIP
		EHS_FB_START_THREAD(mqtt_client_thread,-99);
	#endif //else if EHS_LWIP
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(mqtt_client)
{
    /*
	inx_mqtt_client_state_type *inx_mqtt_client_state = (inx_mqtt_client_state_type*)EHS_FB_DESTROY_CONTEXT;
     */
	//Your code below here
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION connect MACRO START -- DO NOT ALTER
/**
 * Definition of mqtt_publish_publish.
 * Definition of mqtt_client_connect.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(mqtt_client_connect)
{
    /*
	inx_mqtt_client_state_type* inx_mqtt_client_state = (inx_mqtt_client_state_type*)EHS_FB_RUN_CONTEXT;
*/

	// Your code here
    inx_mqtt_client_state_type_mine* inx_mqtt_client_state = (inx_mqtt_client_state_type_mine*)EHS_FB_RUN_CONTEXT;
	//create a pointer to our run data so that we can process events from unity later
	inx_mqtt_client_state->pFIdata = EHS_FB_RUN_CONTEXT_REF;
	inx_mqtt_client_state->connect=1;
    /*
	if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_port))
		EHS_FB_IN_I_API2(INX_mqtt_client_ARG_connect_port) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_clientid))
		EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_clientid) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_username))
		EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_username) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_tls))
		EHS_FB_IN_B_API2(INX_mqtt_client_ARG_connect_tls) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_password))
		EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_password) ;
	if (EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_host))
		EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_host) ;
	EHS_FB_FINISH(INX_mqtt_client_ARG_connect_finishdisconnect);
	EHS_FB_FINISH(INX_mqtt_client_ARG_connect_finishconnect);
     */
}//ICB FUNCTION connect MACRO END -- DO NOT ALTER THIS LINE

#ifdef EHS_MINGW
#define EHS_MQTT_CLIENT_EXPORT __declspec(dllexport)
#else
#define EHS_MQTT_CLIENT_EXPORT // nothing
#endif

EHS_MQTT_CLIENT_EXPORT void EhsMQTTConnectPoll(ehs_bool* connect,char** host,uint16_t* pPort,uint8_t* pTLS,char** clientid,char** username,char** password,char** clientCertFileName,char** clientKeyFileName,char** rootCAFileName){
	inx_mqtt_client_state_type_mine* inx_mqtt_client_state=inxMQTTSubscribeGetLastWidget();
	if(inx_mqtt_client_state==NULL){
		*connect=0;
	}else{
		*connect=inx_mqtt_client_state->connect;
		EhsFunctionInstanceDataType* pFIdata=inx_mqtt_client_state->pFIdata;
		if(pFIdata==NULL){

		}else{
			*clientCertFileName=inx_mqtt_client_state->clientCertFileName;
			*clientKeyFileName=inx_mqtt_client_state->clientKeyFileName;
			*rootCAFileName=inx_mqtt_client_state->rootCAFileName;
			if(EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_host)){
				*host=EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_host);
			}
			if(EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_port)){
				*pPort=EHS_FB_IN_I_API2(INX_mqtt_client_ARG_connect_port);
			}
			if(EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_tls)){
				*pTLS=EHS_FB_IN_B_API2(INX_mqtt_client_ARG_connect_tls);
			}
			if(EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_clientid)){
				*clientid=EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_clientid);
			}
			if(EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_username)){
				*username=EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_username);
			}
			if(EHS_FB_IN_CONNECTED_API2(INX_mqtt_client_ARG_connect_password)){
				*password=EHS_FB_IN_S_API2(INX_mqtt_client_ARG_connect_password);
			}
		}
	}
}

EHS_MQTT_CLIENT_EXPORT void EhsMQTTConnectEvent(ehs_bool connect){
	inx_mqtt_client_state_type_mine* pState=inxMQTTSubscribeGetLastWidget();
	if(pState==NULL){
		
	}else{
		//create pFIData variable so we can use the APIs
		EhsFunctionInstanceDataType* pFIdata=pState->pFIdata;
		if(pFIdata==NULL){

		}else{
			if(connect){
				EHS_FB_FINISH(INX_mqtt_client_ARG_connect_finishconnect);
			}else{
				EHS_FB_FINISH(INX_mqtt_client_ARG_connect_finishdisconnect);
			}
		}
	}
}

//ICB FUNCTION disconnect MACRO START -- DO NOT ALTER
/**
 * Definition of mqtt_client_disconnect.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(mqtt_client_disconnect)
{
	/*
	inx_mqtt_client_state_type* inx_mqtt_client_state = (inx_mqtt_client_state_type*)EHS_FB_RUN_CONTEXT;
*/

	// Your code here
    inx_mqtt_client_state_type_mine* inx_mqtt_client_state = (inx_mqtt_client_state_type_mine*)EHS_FB_RUN_CONTEXT;
    inx_mqtt_client_state->connect=0;
}//ICB FUNCTION disconnect MACRO END -- DO NOT ALTER THIS LINE
