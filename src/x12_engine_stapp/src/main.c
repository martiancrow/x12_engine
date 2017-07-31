#include "x12app.h"

//static x12_data_pool_t *pool = NULL;
static x12_data_queue_t *queue = NULL;
static x12_pthread_t wpid_0;
static x12_pthread_t rpid_0;
static x12_pthread_t rpid_1;

x12_socket_udp_t *server;
x12_socket_udp_t *connection;
x12_socket_tcp_t *tcp_server;
x12_socket_tcp_t *tcp_client;

static int lualib_add(lua_State* L)
{

	int op1 = lua_tointeger(L, -1);
	lua_pop(L, 1);


	int op2 = luaL_checkinteger(L, -1);
	lua_pop(L, 1);

	printf("op1 = %d\n", op1);
	int sum = op1 + op2;

	lua_pushinteger(L, sum);

	return 1;
}

static int luaclib_printf(lua_State* L)
{
	printf("this is c lib hello world2!\n");
	return 1;
}

void threadwrite_0(void)
{
	printf("write thread 0 start!\n");
	
	time_t t;  
    
	while(1)
	{
		t = time(NULL);  
		int ii = time(&t);
		
		if(queue == NULL)
		{
			printf("threadwrite_0 : queue is NULL!\n");
		}
		else
		{
			dq_size_t wlen = x12_data_queue_enqueue(queue, (unsigned char *)&ii, sizeof(ii));
			printf("threadwrite_0 : write %d into queue return is %d!\n", ii, wlen);
		}

		sleep(1);
	}
	
	pthread_exit(NULL);
}

void threadread_0(void)
{
	printf("read thread 0 start!\n");
	
	while(1)
	{
		if(queue == NULL)
		{
			printf("threadread_0 : queue is NULL!\n");
		}
		else
		{
			int ii;
			memset(&ii, 0, sizeof(ii));
			dq_size_t rlen = x12_data_queue_dequeue(queue, (unsigned char *)&ii, sizeof(ii));
			printf("threadread_0 : read %d on queue return is %d!\n", ii, rlen);
				
			x12_data_queue_unlock(queue);

		}
		
		
		sleep(2);
	}
	
	pthread_exit(NULL);
}

void threadread_1(void)
{
	printf("read thread 1 start!\n");
	
	while(1)
	{
		if(queue == NULL)
		{
			printf("threadread_1 : queue is NULL!\n");
		}
		else
		{
			int ii;
			memset(&ii, 0, sizeof(ii));

			short rlock = x12_data_queue_lock(queue);

			if(rlock == 0)
			{

				dq_size_t rlen = x12_data_queue_dequeue(queue, (unsigned char *)&ii, sizeof(ii));
		
				printf("threadread_1 : read %d on queue return is %d!\n", ii, rlen);
			}
		
			x12_data_queue_unlock(queue);
		}
		

		sleep(2);
	}

	pthread_exit(NULL);
}

short cleanprefun(unsigned char *_data, dq_size_t _datalen)
{
	printf("this is cleanprefun\n");

	return 0;
}

#ifdef _OS_LINUX_

char wifimonitor_callback(void* _queue)
{
	struct x12_evens_queue_item_s even;
	
	char ret = x12_evens_queue_denter(_queue, &even);

	if(ret == 0)
	{
		unsigned char *buff;
		int bufflen;

		buff = even.data;
		bufflen = even.datalen;

		unsigned char sa[6];
		ret = ieee80211_air_tool_scan_getsa(buff, bufflen, sa);

		if(ret == 0)
		{

			int cctaglen;
			unsigned char *cctag = ieee80211_air_tool_scan_gettag_merge(buff, bufflen, 0xcc, &cctaglen);
			
			if(cctag)
			{
				printf("---------------------------\n");
				printf("sa:%02x:%02x:%02x:%02x:%02x:%02x\n", sa[0], sa[1], sa[2], sa[3], sa[4], sa[5]);
	
				unsigned char da[6];
				ret = ieee80211_air_tool_scan_getda(buff, bufflen, da);

				if(ret == 0)
				{
					printf("da:%02x:%02x:%02x:%02x:%02x:%02x\n", da[0], da[1], da[2], da[3], da[4], da[5]);
				}

				signed char signal = 0;
				signal = ieee80211_air_tool_scan_getsignal(buff, bufflen);

				if(signal != 1)
				{
					printf("signal:%ddBm\n", signal);
				}

				int ssidlen;
				unsigned char *ssid = ieee80211_air_tool_scan_gettag_index(buff, bufflen, 0x00, &ssidlen, 0);

				if(ssidlen == 0)
				{
					printf("ssid:null\n");
				}
				else
				{
					printf("ssid:%s\n", ssid);
				}

				if(cctag)
				{
					printf("cctaglen:%d\n", cctaglen);
					printf("cctag:%s\n", cctag);
				}
				else
				{
					printf("cctag:null\n");
				}
				

				printf("---------------------------\n");
			
				if(ssid != NULL)
				{
					free(ssid);
					ssid = NULL;
				}
		
			}

			if(cctag != NULL)
			{
				free(cctag);
				cctag = NULL;
			}

		}	
	}
	else
	{
		printf("get monitor buff is fail!\n");
	}

	return 0;
}

char wifimonitorkb_callback(void* _queue)
{
	struct x12_evens_queue_item_s even;
	char ret = x12_evens_queue_denter(_queue, &even);

	if(ret == 0)
	{
		
		char *ssid = "scanmsg";
		//ieee80211_air_tool_scan_senddata("wlan0", NULL, 0, 0xcc, even.data, even.datalen);
		ieee80211_air_tool_scan_senddata("wlan0", (unsigned char *)ssid, strlen(ssid) + 1, 0xcc, even.data, even.datalen);
		
		/*
		unsigned char ies[1024 + 256];
		memset(ies, 0, sizeof(ies));
		memcpy(ies, "hello world", 11);

		ieee80211_air_tool_scan_senddata("wlan0", (unsigned char *)ssid, strlen(ssid), 0xcc, ies, sizeof(ies));
		*/
	}
	else
	{
		printf("evens get fail\n");
	}

	return 0;
}

#endif


char tcp_servercallback(void* _queue)
{
	struct x12_evens_queue_item_s even;

	char ret = x12_evens_queue_denter(_queue, &even);

	if(ret == 0)
	{
		printf("driverid:%d - %s\n", even.evendriverid, even.data);
		x12_even_driver_t *driver = x12_even_drivers_search(even.evendriverid);
		
		x12_socket_tcp_send(driver->linkdata, even.data, even.datalen);
	}
	else
	{
		printf("evens get fail!\n");
	}

	return 0;
}

char clientcallback(void* _queue)
{
	struct x12_evens_queue_item_s even;
	char ret = x12_evens_queue_denter(_queue, &even);

	if(ret == 0)
	{
		printf("driverid:%d - %s\n", even.evendriverid, even.data);

	}
	else
	{
		printf("evens get fail!\n");	
	}
	
	return 0;
}

char clientkbcallback(void* _queue)
{
	struct x12_evens_queue_item_s even;
	char ret = x12_evens_queue_denter(_queue, &even);
	
	if(ret == 0)
	{
		x12_even_driver_t *client =  x12_even_drivers_search(0);

		if(client != NULL)
		{
			x12_socket_udp_send(client->linkdata, even.data, even.datalen);
		}
	}
	else
	{
		printf("evens get fail\n");
	}

	return 0;

}


char tcp_clientcallback(void* _queue)
{
	struct x12_evens_queue_item_s even;

	char ret = x12_evens_queue_denter(_queue, &even);

	if(ret == 0)
	{
		printf("driverid:%d - %s\n", even.evendriverid, even.data);
	}
	else
	{
		printf("evens get fail\n");
	}

	return 0;
}

char tcp_clientkbcallback(void *_queue)
{
	struct x12_evens_queue_item_s even;

	char ret = x12_evens_queue_denter(_queue, &even);

	if(ret == 0)
	{

		x12_even_driver_t *client =  x12_even_drivers_search(0);

		if(client != NULL)
		{
			x12_socket_tcp_send(client->linkdata, even.data, even.datalen);
		}
	}
	else
	{
		printf("kb get msg fail\n");
	}

	return 0;
}

int x12_coap_login(int _senderid, smart_coap_msg_t *_msg)
{
/*
	char *rootpath = "/Volumes/DATA/virtualboxHost/ubuntu16.04_64/x12_engine/bin";
	x12_python_call_init(rootpath);

	PyObject *msg = sct_msg_to_pyobj(_msg);
	PyObject *send = drv_to_pyobj(_senderid);

	x12_python_call_eval(rootpath, "hello", "hello", NULL, "(O, O)", send, msg);

	x12_python_call_reslease();

	return 0;
*/
	char *id = smart_coap_tool_msg_get_query(_msg, "id", 0);
	char *pwd = smart_coap_tool_msg_get_query(_msg, "pwd", 0);


	printf("id = %s, pwd = %s\n", id, pwd);

	//printf("id = %lu, pwd = %lu\n", strlen(id), strlen(pwd));
	
	if(id != NULL)
	{
		free(id);
		id = NULL;
	}

	if(pwd != NULL)
	{
		free(pwd);
		pwd = NULL;
	}

	return 0;
}

int x12_coap_test(int _senderid, smart_coap_msg_t *_msg)
{
	printf("this is test accept!\n");
	
	uint16_t msgid = _msg->ctx->msgid;

	smart_coap_context_t *ctx = smart_coap_tool_context_creat(SMART_COAP_CODE_MSG_TYPE_ACK, SMART_COAP_CODE_RES_CONTENT, msgid);

	uint16_t port = 9095;
	smart_coap_tool_context_add_opt(ctx, SMART_COAP_CODE_OPT_URI_PORT, 2, (uint8_t *)&port);

	char payload[50];
	memset(payload, 0, sizeof(payload));

	char *no = smart_coap_tool_msg_get_query(_msg, "no", 0);

	memcpy(payload, no, strlen(no));

	smart_coap_tool_context_set_payload(ctx, (uint8_t *)payload, strlen(payload) + 1);

	uint32_t sizel = strlen(payload) + 1;
	smart_coap_tool_context_add_opt(ctx, SMART_COAP_CODE_OPT_SIZEL, 4, (uint8_t *)&sizel);
	
	uint32_t msglen;
	uint8_t *msg = smart_coap_tool_context_encode(ctx, &msglen);
	
	int ret = x12_msg_send(_senderid, msg, msglen);
	
	printf("test accept driverid = %d, no = %s, msglen = %d, send ret = %d\n", _senderid, no, msglen, ret);

	return 0;
}

int x12_coap_time(int _senderid, smart_coap_msg_t *_msg)
{
	printf("this is time accept!\n");
	
	uint16_t msgid = _msg->ctx->msgid;

	smart_coap_context_t *ctx = smart_coap_tool_context_creat(SMART_COAP_CODE_MSG_TYPE_ACK, SMART_COAP_CODE_RES_CONTENT, msgid);

	uint16_t port = 9095;
	smart_coap_tool_context_add_opt(ctx, SMART_COAP_CODE_OPT_URI_PORT, 2, (uint8_t *)&port);

	char payload[50];
	memset(payload, 0, sizeof(payload));

	time_t timep;
	time(&timep);
	strcpy(payload, ctime(&timep));


	smart_coap_tool_context_set_payload(ctx, (uint8_t *)payload, strlen(payload) + 1);

	uint32_t sizel = strlen(payload) + 1;
	smart_coap_tool_context_add_opt(ctx, SMART_COAP_CODE_OPT_SIZEL, 4, (uint8_t *)&sizel);
	
	uint32_t msglen;
	uint8_t *msg = smart_coap_tool_context_encode(ctx, &msglen);
	
		
	int ret = x12_msg_send(_senderid, msg, msglen);
	
	uint32_t len = 0;
	uint8_t *gport = smart_coap_tool_context_get_opt(_msg->ctx, SMART_COAP_CODE_OPT_URI_PORT, 0, &len);



	printf("time accept port = %d, driverid = %d, msglen = %d, send ret = %d\n", *(uint32_t *)gport, _senderid, msglen, ret);

	return 0;
}

int x12_coap_msgcallback(int _senderid, smart_coap_msg_t *_msg)
{
/*
	char *rootpath = "/Volumes/DATA/virtualboxHost/ubuntu16.04_64/x12_coap_engine/bin";
	x12_python_call_init(rootpath);

	PyObject *args = sct_msg_to_pyobj(_msg);

	x12_python_call_eval(rootpath, "hello", "hello", NULL, "(O)", args);

	return 0;
*/
	printf("this is msgcallback!\n");


	if(_msg->ctx->payload != NULL)
	{
		printf("payload:\n%s\n", _msg->ctx->payload);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	/*
	pool = x12_data_pool_creat();
	
	if(pool == NULL)
	{
		printf("creat data pool fail!\n");
		return 0;
	}
	
	pthread_create(&wpid_0, NULL, (void *)threadwrite_0, NULL);
	pthread_create(&rpid_0, NULL, (void *)threadread_0, NULL);
	pthread_create(&rpid_1, NULL, (void *)threadread_1, NULL);
	
	pthread_join(wpid_0, NULL);
	pthread_join(rpid_0, NULL);
	pthread_join(rpid_1, NULL);
	
	x12_data_pool_destroy(pool);
	*/

	if(argc > 1 && strcmp(argv[1], "-s") == 0)
	{
		printf("start udp server!\n");

		server = x12_socket_udp_creat_listen(NULL, 9095);
		
		if(server == NULL)
		{
			printf("server creat is fail!\n");
			return 0;
		}
		
		/*
		x12_select_init(0);
		x12_select_addlisten(server->fd);
		x12_select_loopforever();
		*/

		x12_listen_evens_init();
		x12_even_drivers_init();
		x12_coap_kit_init();

		x12_evens_queue_t *queue = x12_evens_queue_creat();
		x12_evens_queue_pthread_creat(queue, x12_coap_kit_even_callback);

		int id;
		int dret = x12_even_drivers_add(&id, server, sizeof(*server), 1);	
		
		x12_coap_kit_add(id);
		
		x12_coap_kit_route_add(id, "/login/:id/:pwd", x12_coap_login);
		x12_coap_kit_route_add(id, "/time", x12_coap_time);
		x12_coap_kit_route_add(id, "/test/:no", x12_coap_test);
		
		x12_listen_evens_add(server->fd, server, sizeof(x12_socket_udp_t), queue, 1);
	
		x12_listen_evens_forever();
	}
	else if(argc > 1 && strcmp(argv[1], "-ts") == 0)
	{
		printf("start tcp server!\n");
		
		tcp_server = x12_socket_tcp_creat_listen(NULL, 9096);

		if(tcp_server == NULL)
		{
			printf("creat listen server is fail!\n");
			return 0;
		}

		x12_listen_evens_init();
		x12_even_drivers_init();
	 	x12_evens_queue_t *queue = x12_evens_queue_creat();
		
		x12_evens_queue_pthread_creat(queue, tcp_servercallback);

		x12_listen_evens_add(tcp_server->fd, (unsigned char *)tcp_server, sizeof(x12_socket_tcp_t), queue, 11);

		x12_listen_evens_forever();

	}
	else if(argc > 1 && strcmp(argv[1], "-c") == 0)
	{
		printf("connection udp server...\n");

		//connection = x12_socket_udp_creat_connection("192.168.1.242", 9095);
		connection = x12_socket_udp_creat_connection("127.0.0.1", 9095);
		
		if(connection == NULL)
		{
			printf("conn creat is fail!\n");
			return 0;
		}

		/*
		x12_select_init(0);
		x12_select_addlisten(connection->fd);
		x12_select_addlisten(0);
		*/

		x12_listen_evens_init();
		x12_even_drivers_init();
		//x12_evens_queue_init();
	 	x12_evens_queue_t *queue = x12_evens_queue_creat();
		int id;
		int dret = x12_even_drivers_add(&id, connection, sizeof(*connection), 2);	
		
		if(dret < 0)
		{
			printf("add driver is fail!\n");
		}
		
		x12_evens_queue_pthread_creat(queue, clientcallback);
		x12_listen_evens_add(connection->fd, (unsigned char *)connection, sizeof(x12_socket_udp_t), queue, 2);
		
		x12_evens_queue_t *kbqueue = x12_evens_queue_creat();
		x12_evens_queue_pthread_creat(kbqueue, clientkbcallback);
		x12_listen_evens_add(0, NULL, 0, kbqueue, 0);

		x12_listen_evens_forever();
	}
	else if(argc > 1 && strcmp(argv[1], "-coap") == 0)
	{
		printf("coap test...\n");

		x12_listen_evens_init();
		x12_even_drivers_init();
		x12_coap_kit_init();
		
		int i;

		for(i = 0; i < 1; i++)
		{

			//x12_socket_udp_t *coap = x12_socket_udp_creat_connection("192.168.1.242", 9095);
			x12_socket_udp_t *coap = x12_socket_udp_creat_connection("127.0.0.1", 9095);
			//x12_socket_udp_t *coap = x12_socket_udp_creat_connection("127.0.0.1", 5683);
		
			if(coap == NULL)
			{
				printf("coap client creat fail\n");
				return 0;
			}


			x12_evens_queue_t *queue = x12_evens_queue_creat();

			int id;
			int dret = x12_even_drivers_add(&id, coap, sizeof(*coap), 2);

			if(dret < 0)
			{
				printf("add driver is fail!\n");
				return 0;
			}
		
			x12_coap_kit_add(id);
			x12_coap_kit_set_msgcallback(id, x12_coap_msgcallback);
		
			x12_evens_queue_pthread_creat(queue, x12_coap_kit_even_callback);
			
			
			x12_listen_evens_add(coap->fd, (unsigned char *)coap, sizeof(x12_socket_udp_t), queue, 2);

			printf("i = %d\n", i);
			

			
			uint16_t msgid = 23695;
			smart_coap_context_t *ctx = smart_coap_tool_context_creat(SMART_COAP_CODE_MSG_TYPE_CON, SMART_COAP_CODE_REQ_GET, msgid);

			uint16_t port = 9095;
			smart_coap_tool_context_add_opt(ctx, SMART_COAP_CODE_OPT_URI_PORT, 2, (uint8_t *)&port);

			char uri[50] = {0};
			sprintf(uri, "/login/username/password");
			//char *uri = "/time";
			smart_coap_tool_context_add_opt(ctx, SMART_COAP_CODE_OPT_URI_PATH, strlen(uri) + 1, (uint8_t *)uri);
		
			//char *query = "no=107&type=3&sort=down";
			//char *query = "&&no=107&&type=3&sort=down";
			//smart_coap_tool_context_add_opt(ctx, SMART_COAP_CODE_OPT_URI_QUERY, strlen(query) + 1, (uint8_t *)query);

			uint32_t sendtime = 0;
			uint32_t msglen;
			uint8_t *msg = smart_coap_tool_context_encode(ctx, &msglen);
	
			while(1)
			{
				x12_socket_udp_send(coap, msg, msglen);
			
				//sleep(1);

				printf("send %d time\n", sendtime++);
			
				break;
			}

			struct sockaddr_in out_addr;
			uint32_t addrlen = sizeof(out_addr);
			memset(&out_addr, 0, sizeof(out_addr));
			int ret = getsockname(coap->fd, (struct sockaddr *)&out_addr, &addrlen);
			printf("ret is %d, out port is %d\n", ret, ntohs(out_addr.sin_port));
		}

		x12_listen_evens_forever();
	}
	else if(argc > 1 && strcmp(argv[1], "-tc") == 0)
	{
		printf("connection tcp server...\n");

		//tcp_client = x12_socket_tcp_creat_connection("192.168.1.242", 9096);
		tcp_client = x12_socket_tcp_creat_connection("127.0.0.1", 9096);
		
		if(tcp_client == NULL)
		{
			printf("connection tcp server fail\n");
		}

		x12_listen_evens_init();
		x12_even_drivers_init();
		x12_evens_queue_t *queue = x12_evens_queue_creat();
		int id;
		x12_even_drivers_add(&id, tcp_client, sizeof(*tcp_client), 12);
		x12_evens_queue_pthread_creat(queue, tcp_clientcallback);
		x12_listen_evens_add(tcp_client->fd, (unsigned char *)tcp_client, sizeof(x12_socket_tcp_t), queue, 12);
		
		x12_evens_queue_t *queuekb = x12_evens_queue_creat();
		x12_evens_queue_pthread_creat(queuekb, tcp_clientkbcallback);
		x12_listen_evens_add(0, NULL, 0, queuekb, 0);
		x12_listen_evens_forever();
	}
	else if(argc > 1 && strcmp(argv[1], "-q") == 0)
	{
		queue = x12_data_queue_creat(32, 32, &cleanprefun);

		if(queue == NULL)
		{
			printf("queue creat is fail errno = %d!\n", dq_errno);
			return 0;
		}

		pthread_create(&wpid_0, NULL, (void *)threadwrite_0, NULL);
		pthread_create(&rpid_0, NULL, (void *)threadread_0, NULL);
		pthread_create(&rpid_1, NULL, (void *)threadread_1, NULL);
	
		pthread_join(wpid_0, NULL);
		pthread_join(rpid_0, NULL);
		pthread_join(rpid_1, NULL);
		
		printf("main is end\n");
		
		x12_data_queue_destroy(queue);

	}
	else if(argc > 1 && strcmp(argv[1], "-bb") == 0)
	{
		char msg[128] = "I am broadCast message from server!"; 

	    int brdcFd;  
	    if((brdcFd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	    {  
	        printf("socket fail\n");  
	        return -1;  
	    } 

	    int optval = 1;//这个值一定要设置，否则可能导致sendto()失败  
	    setsockopt(brdcFd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(int)); 

	    struct sockaddr_in theirAddr;  
	    memset(&theirAddr, 0, sizeof(struct sockaddr_in));  
	    theirAddr.sin_family = AF_INET;  
	    theirAddr.sin_addr.s_addr = inet_addr("255.255.255.255");  
	    theirAddr.sin_port = htons(4001);

	    int sendBytes;  
	    if((sendBytes = sendto(brdcFd, msg, strlen(msg), 0, (struct sockaddr *)&theirAddr, sizeof(theirAddr))) == -1)
	    {  
	        printf("sendto fail, errno=%d\n", errno);  
	        return -1;  
	    }

	    //printf("msg=%s, msgLen=%lu, sendBytes=%d\n", msg, strlen(msg), sendBytes);  
	    close(brdcFd);  

	    return 0;  
	}
	else if(argc > 1 && strcmp(argv[1], "-b") == 0)
	{
		//printf("EDEADLK = %d\n", EDEADLK);


		x12_socket_udp_t *broadcast = x12_socket_udp_creat_listen(NULL, 9095);
		unsigned char msg[256 + 128];
		memset(msg, 0, sizeof(msg));
		memcpy(msg, "hello world!", 12);
		int ret = x12_socket_udp_sendbroadcast(broadcast, (unsigned char *)msg, sizeof(msg));
		
		printf("ret = %d\n", ret);
	}
	else if(argc > 1 && strcmp(argv[1], "-wm") == 0)
	{

#ifdef _OS_LINUX_
		
		x12_socket_raw_t *wifi_mon = x12_socket_raw_creat_monitor("wlan0");
		
		if(wifi_mon == NULL)
		{
			printf("creat wifi monitor is fail!\n");
			return 0;
		}

		x12_listen_evens_init();
		x12_even_drivers_init();
	 	x12_evens_queue_t *queue = x12_evens_queue_creat();
		x12_evens_queue_pthread_creat(queue, wifimonitor_callback);
		x12_listen_evens_add(wifi_mon->fd, wifi_mon, sizeof(x12_socket_raw_t), queue, 21);
		
		
		x12_evens_queue_t *queuekb = x12_evens_queue_creat();
		x12_evens_queue_pthread_creat(queuekb, wifimonitorkb_callback);
		x12_listen_evens_add(0, NULL, 0, queuekb, 0);
		
		x12_listen_evens_forever();

#else
		
		printf("this func only work on linux!\n");

#endif

	}
	else if(argc > 1 && strcmp(argv[1], "-qt") == 0)
	{
		printf("this is test queue!\n");

		int a1 = 1;
		int a2 = 2;
		int a3 = 3;
		int a4 = 4;
		int a5 = 5;

		int *arr;

		arr = malloc(sizeof(int) * 5);

		//arr = realloc(arr, sizeof(int) * 5);
		
		memset(arr, 0, sizeof(int) * 5);

		memcpy(arr, &a1, sizeof(a1));
		memcpy(&arr[1], &a2, sizeof(a2));
		memcpy(&arr[2], &a3, sizeof(a3));
		memcpy(&arr[3], &a4, sizeof(a4));
		memcpy(&arr[4], &a5, sizeof(a5));

		//printf("arr    add %lu\n", arr);
		//printf("arr[1] add %lu\n", &arr[1]);
		//printf("arr[1] add %lu\n", arr + 1);

		//printf("sizeof int is %lu\n", sizeof(int));

		int i;

		for(i = 0; i < 5; i++)
		{
			printf("a[%d]=%d\n", i, arr[i]);
		}
	}
	else if(argc > 1 && strcmp(argv[1], "-python") == 0)
	{
		typedef struct test_s {
			uint32_t length;
			uint8_t *str;
		}test_t;

		test_t str;
		str.length = 6;
		str.str = (uint8_t *)malloc(str.length);

		memcpy(str.str, "hello\0", 6);

		char *rootpath = "./bin";

		x12_python_call_init(rootpath);

		x12_python_call_eval(rootpath, "hello", "hello", NULL, "(i)", 1);
	}
	else if(argc > 1 && strcmp(argv[1], "-plugin") == 0)
	{
		x12_plugin_load("./bin/x12_python.so", "load", NULL);
	}
	else if(argc > 1 && strcmp(argv[1], "-cfg") == 0)
	{

		char *file = "./bin/config.lua";
		char *name = "x12cfg";

		if(argc > 2)
		{
			file = argv[2];
		}

		if(argc > 3)
		{
			name = argv[3];
		}

		x12_cfg_lua_init(file, name);

		x12_listen_evens_forever();

	}
	else if(argc > 1 && strcmp(argv[1], "-lua") == 0)
	{
		lua_State *L = luaL_newstate(); 	//新建lua解释器
		luaL_openlibs(L); 					//载入lua所有函数库
		lua_register(L, "chelloworld", luaclib_printf);	
		lua_register(L, "add1", lualib_add);	
		
		lua_pushstring(L, "hello world this is argv"); 	//第一个操作数入栈
		lua_setglobal(L, "cmsg");			 //获取add函数
		
		luaL_dofile(L, "./bin/test.lua"); 	//执行"Test.lua"文件中的代码
		/*
		int x = 2;
		int y = 4;

		lua_getglobal(L, "add");			 //获取add函数
		lua_pushnumber(L, x); 	//第一个操作数入栈
		lua_pushnumber(L, y); 	//第一个操作数入栈
		
		lua_pcall(L, 2, 1, 0); //调用函数，2个参数，1个返回值

		int sum = (int)lua_tonumber(L, -1);

		lua_pop(L, 1);

		printf("lua add %d + %d = %d\n", x, y, sum);

		lua_getglobal(L, "msg");			 
		const char *msg = lua_tostring(L, -1);
		printf("lua msg is %s\n", msg);
		lua_pop(L, 1);
		*/
		
		lua_close(L); //释放
	}
	else
	{
		int ti;
		uint16_t tu = 65535;

		ti = tu;


		printf("this is enum test %d\n", ti);

		enum test_e {
			value,
			value2 = 11,
			value3,
		};


		printf("value1 = %d\n", value);
		
		printf("value3 = %d\n", value3);

		
		int i = 10;

		if(i > 5)
		{
			printf("this is 5\n");
		}
		else if(i > 3)
		{
			printf("this is 3\n");
		}


	}

	return 0;
}

