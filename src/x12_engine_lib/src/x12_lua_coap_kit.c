/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 lua coap 包
 *
 *
 *作者:刘晓龙
 *时间:2017-07-11
 *最后修改时间:2017-07-11
 */


#include "x12_lua_coap_kit.h"

int x12_lua_coap_kit_sct_req_pushstate(lua_State *_sl, smart_coap_msg_t *_msg)
{
	if(!_sl || !_msg)
	{
		return -1;
	}

	smart_coap_context_t *ctx = _msg->ctx;
	
	uint16_t msgid = ctx->msgid;
	char *type = "ACK";
	char *code = "Content";
	uint8_t ver = 1;

	int t_idx = 0;

	lua_newtable(_sl);
	t_idx = lua_gettop(_sl);
	
	lua_pushstring(_sl, "msgid");
	lua_pushinteger(_sl, msgid);
	lua_settable(_sl, t_idx);

	lua_pushstring(_sl, "type");
	lua_pushstring(_sl, type);
	lua_settable(_sl, t_idx);

	lua_pushstring(_sl, "code");
	lua_pushstring(_sl, code);
	lua_settable(_sl, t_idx);

	lua_pushstring(_sl, "ver");
	lua_pushinteger(_sl, ver);
	lua_settable(_sl, t_idx);
	
	lua_setglobal(_sl, "RES_HEAD");
	
	lua_newtable(_sl);
	t_idx = lua_gettop(_sl);
	
	smart_coap_option_t *optitem = ctx->option;
	
	while(optitem != NULL)
	{
		char *optname = sct_ctx_getoptname(optitem->delta);
		
		if(optname == NULL)
		{
			optitem = optitem->next;
			continue;
		}

		if(optitem->delta == SMART_COAP_CODE_OPT_URI_PATH)
		{	
			char *uri_path =  smart_coap_tool_get_uri(ctx);
			lua_pushstring(_sl, optname);
			lua_pushstring(_sl, uri_path);
			lua_settable(_sl, t_idx);
		}
		else if(optitem->delta == SMART_COAP_CODE_OPT_SIZEL
		|| optitem->delta == SMART_COAP_CODE_OPT_CONTENT_FORMAT
		|| optitem->delta == SMART_COAP_CODE_OPT_ACCEPT
		|| optitem->delta == SMART_COAP_CODE_OPT_URI_QUERY
		|| optitem->delta == SMART_COAP_CODE_OPT_LOCATION_PATH
		|| optitem->delta == SMART_COAP_CODE_OPT_LOCATION_QUERY
		|| optitem->delta == SMART_COAP_CODE_OPT_PROXY_URI
		|| optitem->delta == SMART_COAP_CODE_OPT_PROXY_SCHEME
		|| optitem->delta == SMART_COAP_CODE_OPT_MAX_AGE)
		{
			optitem = optitem->next;
			continue;
		}
		else if(optitem->delta == SMART_COAP_CODE_OPT_URI_PORT)
		{
			uint32_t value = 0;
			memcpy(&value, optitem->value, optitem->length);
			value = ntohs(value);
			
			lua_pushstring(_sl, optname);
			lua_pushinteger(_sl, value);
			lua_settable(_sl, t_idx);
		}
		else if(optitem->delta == SMART_COAP_CODE_OPT_URI_HOST)
		{
			lua_pushstring(_sl, optname);
			lua_pushstring(_sl, (char *)optitem->value);
			lua_settable(_sl, t_idx);
		}
		else
		{
			lua_pushstring(_sl, optname);
			lua_pushlstring(_sl, (char *)optitem->value, optitem->length);
			lua_settable(_sl, t_idx);
		}

		optitem = optitem->next;

	}

	lua_setglobal(_sl, "RES_OPTION");

	
	return 0;
}

int x12_lua_coap_kit_sct_msg_pushstate(lua_State *_sl, smart_coap_msg_t *_msg)
{
	if(!_sl || !_msg)
	{
		return -1;
	}

	int t_idx = 0;

	lua_newtable(_sl);
	t_idx = lua_gettop(_sl);
	
	smart_coap_msg_query_t *item = _msg->query;

	while(item != NULL)
	{
		lua_pushstring(_sl, item->name);
		lua_pushstring(_sl, item->value);

		lua_settable(_sl, t_idx);

		item = item->next;
	}

	lua_setglobal(_sl, "GET");


	smart_coap_context_t *ctx = _msg->ctx;
	lua_pushlstring(_sl, (const char *)ctx->payload, ctx->payloadlen);

	lua_setglobal(_sl, "POST");
	
	lua_newtable(_sl);
	t_idx = lua_gettop(_sl);

	lua_pushstring(_sl, "ver");
	lua_pushinteger(_sl, ctx->ver);
	lua_settable(_sl, t_idx);

	lua_pushstring(_sl, "type");
	
	if(ctx->t == 1)
	{
		lua_pushstring(_sl, "NON");
	}
	else if(ctx->t == 2)
	{
		lua_pushstring(_sl, "ACK");
	}
	else if(ctx->t == 3)
	{
		lua_pushstring(_sl, "RST");
	}
	else
	{
		lua_pushstring(_sl, "CON");
	}

	lua_settable(_sl, t_idx);

	lua_pushstring(_sl, "token");
	lua_pushlstring(_sl, (const char *)ctx->token, ctx->tkl);
	lua_settable(_sl, t_idx);
	
	lua_pushstring(_sl, "msgid");
	lua_pushinteger(_sl, ctx->msgid);
	lua_settable(_sl, t_idx);
	
	char *codename = sct_ctx_getcodename(ctx->code);
	
	lua_pushstring(_sl, "code");
	lua_pushstring(_sl, codename);
	lua_settable(_sl, t_idx);

	lua_setglobal(_sl, "HEAD");


	lua_newtable(_sl);
	t_idx = lua_gettop(_sl);

	smart_coap_option_t *optitem = ctx->option;
	
	while(optitem != NULL)
	{
		char *optname = sct_ctx_getoptname(optitem->delta);
		
		if(optname == NULL)
		{
			optitem = optitem->next;
			continue;
		}

		if(optitem->delta == SMART_COAP_CODE_OPT_URI_PATH)
		{	
			char *uri_path =  smart_coap_tool_get_uri(ctx);
			lua_pushstring(_sl, optname);
			lua_pushstring(_sl, uri_path);
			lua_settable(_sl, t_idx);
		}
		else if(optitem->delta == SMART_COAP_CODE_OPT_CONTENT_FORMAT)
		{
			uint32_t fmt = 0;

			if(optitem->length <= sizeof(fmt))
			{
				memcpy(&fmt, optitem->value, optitem->length);
				fmt = ntohs(fmt);
			}

			char *format = sct_ctx_getfmtname(fmt);
			
			lua_pushstring(_sl, optname);
			lua_pushstring(_sl, format);
			lua_settable(_sl, t_idx);

		}
		else if(optitem->delta == SMART_COAP_CODE_OPT_SIZEL
		|| optitem->delta == SMART_COAP_CODE_OPT_URI_PORT
		|| optitem->delta == SMART_COAP_CODE_OPT_MAX_AGE
		|| optitem->delta == SMART_COAP_CODE_OPT_ACCEPT)
		{
			uint32_t value = 0;
			memcpy(&value, optitem->value, optitem->length);
			value = ntohs(value);
			
			lua_pushstring(_sl, optname);
			lua_pushinteger(_sl, value);
			lua_settable(_sl, t_idx);
		}
		else if(optitem->delta == SMART_COAP_CODE_OPT_URI_HOST
		|| optitem->delta == SMART_COAP_CODE_OPT_LOCATION_PATH
		|| optitem->delta == SMART_COAP_CODE_OPT_URI_QUERY
		|| optitem->delta == SMART_COAP_CODE_OPT_LOCATION_QUERY
		|| optitem->delta == SMART_COAP_CODE_OPT_PROXY_URI
		|| optitem->delta == SMART_COAP_CODE_OPT_PROXY_SCHEME)
		{
			lua_pushstring(_sl, optname);
			lua_pushstring(_sl, (char *)optitem->value);
			lua_settable(_sl, t_idx);
		}
		else
		{
			lua_pushstring(_sl, optname);
			lua_pushlstring(_sl, (char *)optitem->value, optitem->length);
			lua_settable(_sl, t_idx);
		}

		optitem = optitem->next;

	}

	lua_setglobal(_sl, "OPTION");
	
	return 0;
}

void x12_lua_coap_kit_ri_free(x12_lua_coap_route_item_t *_ri)
{
	if(_ri)
	{
		x12_lua_coap_kit_ri_free(_ri->next);

		free(_ri);
		_ri = NULL;
	}
}

void x12_lua_coap_kit_rl_free(x12_lua_coap_route_list_t *_rl)
{
	if(_rl)
	{
		x12_lua_coap_kit_ri_free(_rl->head);
	}
}

void x12_lua_ckd_free(void* _ud)
{
	if(_ud)
	{
		x12_lua_coap_route_list_t *list = (x12_lua_coap_route_list_t *)_ud;
		x12_lua_coap_kit_rl_free(list);
	}
}

int x12_lua_coap_kit_rl_creat(int _drvid)
{

	x12_lua_coap_route_list_t *list = (x12_lua_coap_route_list_t *)malloc(sizeof(x12_lua_coap_route_list_t));
	memset(list, 0, sizeof(*list));

	int ret = x12_coap_kit_udata_add(_drvid, list, x12_lua_ckd_free);

	return ret;
}

x12_lua_coap_route_item_t* x12_lua_coap_kit_ri_get(int _senderid, char *_uri)
{
	x12_coap_kit_item_t *kit = x12_coap_kit_item_get(_senderid);

	if(!kit)
	{
		return NULL;
	}

	x12_lua_coap_route_list_t *list = (x12_lua_coap_route_list_t *)(kit->udata);

	if(!list)
	{
		return NULL;
	}

	x12_lua_coap_route_item_t *item = list->head;

	while(item)
	{
		if(strcmp(_uri, item->uri) == 0)
		{
			break;
		}

		item = item->next;
	}

	return item;

}

int x12_lua_coap_kit_uri_cb(int _senderid, smart_coap_msg_t *_msg)
{

	x12_lua_coap_route_item_t *item = x12_lua_coap_kit_ri_get(_senderid, _msg->callurl);
	
	if(item)
	{
		lua_State *L = luaL_newstate(); 	
		luaL_openlibs(L); 					
		x12_lua_coap_api_load(L);

		lua_pushinteger(L, _senderid);
		lua_setglobal(L, "senderid");

		x12_lua_coap_kit_sct_msg_pushstate(L, _msg);
		x12_lua_coap_kit_sct_req_pushstate(L, _msg);

		luaL_dofile(L, item->path);
		lua_getglobal(L, item->func);
		
		lua_pcall(L, 0, 1, 0); //调用函数，2个参数，1个返回值
	
		lua_close(L);
	}

	return 0;
}

int x12_lua_coap_kit_msg_cb(int _senderid, void* _msg)
{

	x12_coap_kit_item_t *kit = x12_coap_kit_item_get(_senderid);

	if(!kit)
	{
		return -1;
	}

	x12_lua_coap_route_list_t *list = (x12_lua_coap_route_list_t *)(kit->udata);

	if(!list)
	{
		return -1;
	}

	lua_State *L = luaL_newstate(); 	
	luaL_openlibs(L); 					
	x12_lua_coap_api_load(L);
		
	lua_pushinteger(L, _senderid);
	lua_setglobal(L, "senderid");
	
	x12_lua_coap_kit_sct_msg_pushstate(L, _msg);
	x12_lua_coap_kit_sct_req_pushstate(L, _msg);
	
	luaL_dofile(L, list->mcbpath);
	lua_getglobal(L, list->mcbfunc);
		
	lua_pcall(L, 0, 0, 0); //调用函数，2个参数，1个返回值
	
	return 0;
}

int x12_lua_coap_kit_set_msgcb(int _drvid, char *_path, char *_func)
{
	if(!_path || !_func)
	{
		return -1;
	}

	x12_coap_kit_item_t *kit = x12_coap_kit_item_get(_drvid);

	if(!kit)
	{
		return -1;
	}

	x12_lua_coap_route_list_t *list = (x12_lua_coap_route_list_t *)(kit->udata);

	if(!list)
	{
		return -1;
	}

	memset(list->mcbpath, 0, sizeof(list->mcbpath));
	memset(list->mcbfunc, 0, sizeof(list->mcbfunc));

	memcpy(list->mcbpath, _path, strlen(_path));
	memcpy(list->mcbfunc, _func, strlen(_func));

	x12_coap_kit_set_msgcallback(_drvid, x12_lua_coap_kit_msg_cb);

	return 0;
}

int x12_lua_coap_kit_ri_add(int _drvid, char *_uri, char *_path, char *_func)
{


	if(!_uri || !_path || !_func)
	{
		return -1;
	}

	x12_coap_kit_item_t *kit = x12_coap_kit_item_get(_drvid);

	if(!kit)
	{
		return -1;
	}

	x12_lua_coap_route_list_t *list = (x12_lua_coap_route_list_t *)(kit->udata);

	if(!list)
	{
		return -1;
	}

	x12_lua_coap_route_item_t *item = (x12_lua_coap_route_item_t *)malloc(sizeof(x12_lua_coap_route_item_t));
	memset(item, 0, sizeof(*item));
	
	memcpy(item->uri, _uri, strlen(_uri));
	memcpy(item->path, _path, strlen(_path));
	memcpy(item->func, _func, strlen(_func));

	if(list->head)
	{
		item->next = list->head;
	}
	
	list->head = item;

	x12_coap_kit_route_add(_drvid, _uri, x12_lua_coap_kit_uri_cb);

	return 0;
}

int x12_lua_coap_kit_creatcfg(char *_action, char *_addr, int _port, char *_name, x12_lua_coap_cfg_route_t *_route, int _route_len, char *_msgcbpath, char *_msgcbfunc)
{
	x12_socket_udp_t *udp = NULL;
	int etype = 1;

	if(strcmp(_action, "listen") == 0)
	{
		udp = x12_socket_udp_creat_listen(_addr, _port);
		etype = 1;
	}
	else if(strcmp(_action, "connection") == 0)
	{
		udp = x12_socket_udp_creat_connection(_addr, _port);
		etype = 2;
	}
	else
	{
		return -1;
	}
		
	if(udp == NULL)
	{
		return -1;
	}

	int i;

	x12_listen_evens_init();
	x12_even_drivers_init();
	x12_coap_kit_init();

	x12_evens_queue_t *queue = x12_evens_queue_creat();
	x12_evens_queue_pthread_creat(queue, x12_coap_kit_even_callback);

	int id;
	int dret = x12_even_drivers_add(&id, udp, sizeof(*udp), etype);	
	x12_coap_kit_add(id);

	
	x12_lua_coap_kit_rl_creat(id);
	
	x12_lua_coap_kit_set_msgcb(id, _msgcbpath, _msgcbfunc);

	for(i = 0; i < _route_len; i++)
	{
		x12_lua_coap_kit_ri_add(id, _route[i].uri, _route[i].path, _route[i].func);
	}
		
	x12_listen_evens_add(udp->fd, udp, sizeof(x12_socket_udp_t), queue, etype);
	
	return id;
}

int x12_lua_coap_kit_do(lua_State *_sl)
{
	if(!_sl)
	{
		return -1;
	}

	if(!lua_istable(_sl, -1))
	{
		return -1;
	}

	int t_idx = lua_gettop(_sl);
	lua_pushnil(_sl);

	printf("\t\tthis is coap server:\n");

	int i;

	char *action = NULL;
	char *address = NULL;
	char *name = NULL;
	char *msgcbpath = NULL;
	char *msgcbfunc = NULL;
	int port = 0;

	int inc_len = 0;
	char** inc = NULL;

	int done_len = 0;
	char** done = NULL;

	x12_lua_coap_cfg_route_t *route = NULL;
	int route_len = 0;

	while(lua_next(_sl, t_idx))
	{
		const char *key = lua_tostring(_sl, -2);
	
		if(strcmp(key, "family") == 0)
		{
			if(!lua_isstring(_sl, -1))
			{
				goto param_err;
			}

			const char *family = lua_tostring(_sl, -1);

			if(strcmp(family, "x12_coap_udp") != 0)
			{
				goto param_err;
			}

		}
		else if(strcmp(key, "address") == 0)
		{
			if(!lua_isstring(_sl, -1))
			{
				goto param_err;
			}

			const char *val = lua_tostring(_sl, -1);
			address = (char *)malloc(strlen(val) + 1);
			memset(address, 0, strlen(val) + 1);
			memcpy(address, val, strlen(val));
		}
		else if(strcmp(key, "action") == 0)
		{
			if(!lua_isstring(_sl, -1))
			{
				goto param_err;
			}

			const char *val = lua_tostring(_sl, -1);
			action = (char *)malloc(strlen(val) + 1);
			memset(action, 0, strlen(val) + 1);
			memcpy(action, val, strlen(val));
		}
		else if(strcmp(key, "name") == 0)
		{
			if(!lua_isstring(_sl, -1))
			{
				goto param_err;
			}

			const char *val = lua_tostring(_sl, -1);
			name = (char *)malloc(strlen(val) + 1);
			memset(name, 0, strlen(val) + 1);
			memcpy(name, val, strlen(val));
		}
		else if(strcmp(key, "port") == 0)
		{
			if(!lua_isnumber(_sl, -1))
			{
				goto param_err;
			}

			port = lua_tonumber(_sl, -1);
		}
		else if(strcmp(key, "msgcb") == 0)
		{
			if(!lua_istable(_sl, -1))
			{
				goto param_err;
			}
			
			int msgcbc_len = luaL_len(_sl, -1);

			if(msgcbc_len != 2)
			{
				goto param_err;
			}

			for(i = 1; i <= msgcbc_len; i++)
			{
				lua_pushnumber(_sl, i);
				lua_gettable(_sl, -2);
				
				if(!lua_isstring(_sl, -1))
				{
					goto param_err;
				}

				const char *val = lua_tostring(_sl, -1);
				
				if(i == 1)
				{
					msgcbpath = (char *)malloc(strlen(val) + 1);
					memset(msgcbpath, 0, strlen(val) + 1);
					memcpy(msgcbpath, val, strlen(val));
				}
				else if(i == 2)
				{
					msgcbfunc = (char *)malloc(strlen(val) + 1);
					memset(msgcbfunc, 0, strlen(val) + 1);
					memcpy(msgcbfunc, val, strlen(val));
				}


				lua_pop(_sl, 1);
			}

		}
		else if(strcmp(key, "inc") == 0)
		{
			if(!lua_istable(_sl, -1))
			{
				goto param_err;
			}

			inc_len = luaL_len(_sl, -1);

			inc = (char **)malloc(sizeof(char *) * inc_len);
			memset(inc, 0, sizeof(char *) * inc_len);

			for(i = 1; i <= inc_len; i++)
			{
				lua_pushnumber(_sl, i);
				lua_gettable(_sl, -2);
				const char *val = lua_tostring(_sl, -1);

				if(val)
				{
					inc[i - 1] = (char *)malloc(strlen(val) + 1);
					memset(inc[i - 1], 0, strlen(val) + 1);
					memcpy(inc[i - 1], val, strlen(val));
				}

				lua_pop(_sl, 1);
			}
		}
		else if(strcmp(key, "done") == 0)
		{
			if(!lua_istable(_sl, -1))
			{
				goto param_err;
			}

			done_len = luaL_len(_sl, -1);

			done = (char **)malloc(sizeof(char *) * done_len);
			memset(done, 0, sizeof(char *) * done_len);

			for(i = 1; i <= done_len; i++)
			{
				lua_pushnumber(_sl, i);
				lua_gettable(_sl, -2);
				const char *val = lua_tostring(_sl, -1);

				if(val)
				{
					done[i - 1] = (char *)malloc(strlen(val) + 1);
					memset(done[i - 1], 0, strlen(val) + 1);
					memcpy(done[i - 1], val, strlen(val));
				}

				lua_pop(_sl, 1);
			}
		}
		else if(strcmp(key, "route") == 0)
		{
			if(!lua_istable(_sl, -1))
			{
				goto param_err;
			}

			route_len = luaL_len(_sl, -1);

			route = (x12_lua_coap_cfg_route_t *)malloc(sizeof(x12_lua_coap_cfg_route_t) * route_len);
			memset(route, 0, sizeof(x12_lua_coap_cfg_route_t) * route_len);

			int j;

			for(i = 1; i <= route_len; i++)
			{
				lua_pushnumber(_sl, i);
				lua_gettable(_sl, -2);

				if(!lua_istable(_sl, -1))
				{
					goto param_err;
				}

				int keylen = luaL_len(_sl, -1);

				if(keylen < 3)
				{
					goto param_err;
				}

				for(j = 1; j <= keylen; j++)
				{
					lua_pushnumber(_sl, j);
					lua_gettable(_sl, -2);
					
					const char *val = lua_tostring(_sl, -1);
				
					if(val)
					{
						if(j == 1)
						{
							memcpy(route[i - 1].uri, val, strlen(val));
						}
						else if(j == 2)
						{
							memcpy(route[i - 1].path, val, strlen(val));
						}
						else if(j == 3)
						{
							memcpy(route[i - 1].func, val, strlen(val));
						}
					}

					lua_pop(_sl, 1);
				
				}

				lua_pop(_sl, 1);
			}
		}
		

		lua_pop(_sl, 1);
	}

	for(i = 0; i < inc_len; i++)
	{
		printf("\t\tinc:%s\n", inc[i]);
		
		lua_State *L = luaL_newstate(); 	
		luaL_openlibs(L); 					
		x12_lua_coap_api_load(L);

		lua_pushstring(L, name);
		lua_setglobal(L, "NAME");
		
		luaL_dofile(L, inc[i]);
		lua_close(L);
	}

	printf("\t\t%s %s:%d do %s\n", name, address, port, action);
	int drvid = x12_lua_coap_kit_creatcfg(action, address, port, name, route, route_len, msgcbpath, msgcbfunc);
	
	for(i = 0; i < done_len; i++)
	{
		printf("\t\tdone:%s\n", done[i]);

		lua_State *L = luaL_newstate(); 	
		luaL_openlibs(L); 					
		x12_lua_coap_api_load(L);

		lua_pushinteger(L, drvid);
		lua_setglobal(L, "DRVID");

		lua_pushstring(L, name);
		lua_setglobal(L, "NAME");
		
		luaL_dofile(L, done[i]);
		lua_close(L);
		
	}

	for(i = 0; i < route_len; i++)
	{
		printf("\t\troute:%s %s %s\n", route[i].uri, route[i].path, route[i].func);
	}
	
	if(address)
	{
		free(address);
		address = NULL;
	}

	if(name)
	{
		free(name);
		name = NULL;
	}

	if(action)
	{
		free(action);
		action = NULL;
	}

	if(msgcbpath)
	{
		free(msgcbpath);
		msgcbpath = NULL;
	}

	if(msgcbfunc)
	{
		free(msgcbfunc);
		msgcbfunc = NULL;
	}

	if(inc)
	{
		for(i = 0; i < inc_len; i++)
		{
			if(inc[i])
			{
				free(inc[i]);
				inc[i] = NULL;
			}
		}

		free(inc);
		inc = NULL;
	}
	
	if(done)
	{
		for(i = 0; i < done_len; i++)
		{
			if(done[i])
			{
				free(done[i]);
				done[i] = NULL;
			}
		}

		free(done);
		done = NULL;
	}

	if(route)
	{
		free(route);
		route = NULL;
	}

	lua_settop(_sl, t_idx);
	
	return 0;

param_err:
	//参数错误
	if(address)
	{
		free(address);
		address = NULL;
	}

	if(name)
	{
		free(name);
		name = NULL;
	}

	if(action)
	{
		free(action);
		action = NULL;
	}

	if(msgcbpath)
	{
		free(msgcbpath);
		msgcbpath = NULL;
	}

	if(msgcbfunc)
	{
		free(msgcbfunc);
		msgcbfunc = NULL;
	}

	if(inc)
	{
		for(i = 0; i < inc_len; i++)
		{
			if(inc[i])
			{
				free(inc[i]);
				inc[i] = NULL;
			}
		}

		free(inc);
		inc = NULL;
	}

	if(done)
	{
		for(i = 0; i < done_len; i++)
		{
			if(done[i])
			{
				free(done[i]);
				done[i] = NULL;
			}
		}

		free(done);
		done = NULL;
	}

	if(route)
	{
		free(route);
		route = NULL;
	}

	lua_settop(_sl, t_idx);
	
	return -1;

}

