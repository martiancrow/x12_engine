/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 lua coap 接口
 *
 *
 *作者:刘晓龙
 *时间:2017-07-11
 *最后修改时间:2017-07-11
 */

#include "x12_lua_coap_api.h"

smart_coap_option_t* x12_lua_coap_api_getoption(lua_State *_sl)
{
	if(!_sl)
	{
		return NULL;
	}

	if(!lua_istable(_sl, -1))
	{
		return NULL;
	}
	
	smart_coap_option_t *opthead = NULL;
	smart_coap_option_t *optlast = NULL;

	int opt_t_idx = lua_gettop(_sl);
	lua_pushnil(_sl);

	while(lua_next(_sl, opt_t_idx))
	{
		const char *key = lua_tostring(_sl, -2);
		uint32_t delta = sct_ctx_getoptdelta(key);

		if(delta == SMART_COAP_CODE_OPT_CONTENT_FORMAT)
		{
			if(!lua_isstring(_sl, -1))
			{
				goto getopt_fail;
			}

			const char *fmtname = lua_tostring(_sl, -1);
			uint32_t fmtno = sct_ctx_getfmtno(fmtname);

			smart_coap_option_t *opt = (smart_coap_option_t *)malloc(sizeof(smart_coap_option_t));
			memset(opt, 0, sizeof(smart_coap_option_t));
				
			opt->delta = delta;
			opt->length = sizeof(fmtno);
			opt->value = (uint8_t *)malloc(opt->length);
			memcpy(opt->value, &fmtno, sizeof(fmtno));
			
			if(!opthead)
			{
				opthead = opt;
			}
			else
			{
				optlast->next = opt;
			}
						
			optlast = opt;
		}
		else if(delta == SMART_COAP_CODE_OPT_SIZEL
		|| delta == SMART_COAP_CODE_OPT_URI_PORT
		|| delta == SMART_COAP_CODE_OPT_MAX_AGE
		|| delta == SMART_COAP_CODE_OPT_ACCEPT)
		{
			if(!lua_isnumber(_sl, -1))
			{
				goto getopt_fail;
			}

			uint32_t val = lua_tointeger(_sl, -1);
			val = htons(val);
					
			smart_coap_option_t *opt = (smart_coap_option_t *)malloc(sizeof(smart_coap_option_t));
			memset(opt, 0, sizeof(smart_coap_option_t));

			opt->delta = delta;
			opt->length = sizeof(val);
			opt->value = (uint8_t *)malloc(opt->length);
			memcpy(opt->value, &val, opt->length);
					
			if(!opthead)
			{
				opthead = opt;
			}
			else
			{
				optlast->next = opt;
			}
						
			optlast = opt;
		}
		else if(delta == SMART_COAP_CODE_OPT_URI_HOST
		|| delta == SMART_COAP_CODE_OPT_URI_PATH
		|| delta == SMART_COAP_CODE_OPT_LOCATION_PATH
		|| delta == SMART_COAP_CODE_OPT_URI_QUERY
		|| delta == SMART_COAP_CODE_OPT_LOCATION_QUERY
		|| delta == SMART_COAP_CODE_OPT_PROXY_URI
		|| delta == SMART_COAP_CODE_OPT_PROXY_SCHEME)
		{
			if(!lua_isstring(_sl, -1))
			{
				goto getopt_fail;
			}

			const char *val = lua_tostring(_sl, -1);
			
			smart_coap_option_t *opt = (smart_coap_option_t *)malloc(sizeof(smart_coap_option_t));
			memset(opt, 0, sizeof(smart_coap_option_t));

			opt->delta = delta;
			opt->length = strlen(val) + 1;
			opt->value = (uint8_t *)malloc(opt->length);
			memset(opt->value, 0, opt->length);
			memcpy(opt->value, val, strlen(val));


			if(!opthead)
			{
				opthead = opt;
			}
			else
			{
				optlast->next = opt;
			}
						
			optlast = opt;
		}
		else
		{
			if(lua_isstring(_sl, -1))
			{
				goto getopt_fail;
			}

			size_t len = 0;
			const char *val = lua_tolstring(_sl, -1, &len);

			smart_coap_option_t *opt = (smart_coap_option_t *)malloc(sizeof(smart_coap_option_t));
			memset(opt, 0, sizeof(smart_coap_option_t));

			opt->delta = delta;
			opt->length = len;
			opt->value = (uint8_t *)malloc(opt->length);
			memcpy(opt->value, &val, opt->length);
			
			if(!opthead)
			{
				opthead = opt;
			}
			else
			{
				optlast->next = opt;
			}
						
			optlast = opt;
		}

		lua_pop(_sl, 1);
	}

	lua_settop(_sl, opt_t_idx);

	return opthead;

getopt_fail:

	smart_coap_tool_option_free(opthead);
	return NULL;

}

smart_coap_context_t* x12_lua_coap_api_getctx(lua_State *_sl)
{
	if(!_sl)
	{
		return NULL;
	}

	int ret = 0;

	smart_coap_context_t *ctx = (smart_coap_context_t *)malloc(sizeof(smart_coap_context_t));
	memset(ctx, 0, sizeof(smart_coap_context_t));
	
	int ctx_t_idx = lua_gettop(_sl);
	lua_pushnil(_sl);

	while(lua_next(_sl, ctx_t_idx))
	{
		
		const char *key = lua_tostring(_sl, -2);
	
		if(strcmp(key, "ver") == 0)
		{
			if(!lua_isnumber(_sl, -1))
			{
				goto getctx_fail;
			}

			ctx->ver = lua_tointeger(_sl, -1);

		}

		if(strcmp(key, "type") == 0)
		{

			if(!lua_isstring(_sl, -1))
			{
				goto getctx_fail;
			}

			const char *type = lua_tostring(_sl, -1);


			if(strcmp(type, "CON") == 0)
			{
				ctx->t = 0;
			}
			else if(strcmp(type, "NON") == 0)
			{
				ctx->t = 1;
			}
			else if(strcmp(type, "ACK") == 0)
			{
				ctx->t = 2;
			}
			else if(strcmp(type, "RST") == 0)
			{
				ctx->t = 3;
			}
		
		}

		if(strcmp(key, "token") == 0)
		{
			if(!lua_isstring(_sl, -1))
			{
				goto getctx_fail;
			}

			size_t len = 0;
			const char *token = lua_tolstring(_sl, -1, &len);

			if(token && len <= 15)
			{
				ctx->tkl = len;
				memcpy(ctx->token, token, len);
			}
		}
		
		if(strcmp(key, "msgid") == 0)
		{
			if(!lua_isnumber(_sl, -1))
			{
				goto getctx_fail;
			}

			ctx->msgid = lua_tointeger(_sl, -1);
		}
		
		if(strcmp(key, "payload") == 0)
		{
			if(!lua_isstring(_sl, -1))
			{
				goto getctx_fail;
			}

			size_t len = 0;
			const char *payload = lua_tolstring(_sl, -1, &len);

			if(payload)
			{
				ctx->payloadlen = len;
				ctx->payload = (uint8_t *)malloc(ctx->payloadlen);
				memset(ctx->payload, 0, ctx->payloadlen);
				memcpy(ctx->payload, payload, len);
			}
		}
		
		if(strcmp(key, "code") == 0)
		{
			if(!lua_isstring(_sl, -1))
			{
				goto getctx_fail;
			}

			const char *codename = lua_tostring(_sl, -1);

			ctx->code = sct_ctx_getcodeno(codename);
		}

		if(strcmp(key, "option") == 0)
		{
			if(!lua_istable(_sl, -1))
			{
				goto getctx_fail;
			}

			ctx->option = x12_lua_coap_api_getoption(_sl);

		}
		
		lua_pop(_sl, 1);
	}

	return ctx;

getctx_fail:
	
	smart_coap_tool_context_free(ctx);
	return NULL;

}

static int x12_lua_coap_api_response(lua_State* L)
{
	if(!L)
	{
		lua_pushinteger(L, -1);
		return 1;
	}

	if(!lua_isstring(L, -1))
	{
		lua_pushinteger(L, -1);
		return 1;
	}

	size_t len = 0;
	const char *payload = lua_tolstring(L, -1, &len);
	
	lua_getglobal(L, "senderid");
	
	if(!lua_isnumber(L, -1))
	{
		lua_pushinteger(L, -1);
		return 1;
	}

	int senderid = lua_tointeger(L, -1);

	lua_getglobal(L, "RES_HEAD");

	if(!lua_istable(L, -1))
	{
		lua_pushinteger(L, -1);
		return 1;
	}

	smart_coap_context_t *ctx = x12_lua_coap_api_getctx(L);

	if(!ctx)
	{
		lua_pushinteger(L, -1);
		return 1;
	}

	ctx->payloadlen = len;
	ctx->payload = (uint8_t *)malloc(ctx->payloadlen + 1);
	memset(ctx->payload, 0, ctx->payloadlen + 1);
	memcpy(ctx->payload, payload, ctx->payloadlen);
	
	lua_getglobal(L, "RES_OPTION");

	if(!lua_istable(L, -1))
	{
		lua_pushinteger(L, -1);
		return 1;
	}

	ctx->option = x12_lua_coap_api_getoption(L);
	
	uint32_t msglen = 0; 
	uint8_t *msg = smart_coap_tool_context_encode(ctx, &msglen);

	int ret = x12_msg_send(senderid, msg, msglen);
	
	smart_coap_tool_context_free(ctx);
	
	lua_pushinteger(L, ret);
	
	return 1;
}

static int x12_lua_coap_api_sleep(lua_State* L)
{
	if(!L)
	{
		return 1;
	}

	if(!lua_isnumber(L, -1))
	{
		return 1;
	}

	unsigned int sec = lua_tointeger(L, -1);
	lua_pop(L, 1);
	
	sleep(sec);
	
	return 1;
}

static int x12_lua_coap_api_msg_send(lua_State* L)
{
	if(!L)
	{
		lua_pushinteger(L, -1);
		return 1;
	}
	

	if(!lua_istable(L, -1))
	{
		lua_pushinteger(L, -1);
		return 1;
	}

	smart_coap_context_t *ctx = x12_lua_coap_api_getctx(L);
	lua_pop(L, 1);
	
	if(!lua_isnumber(L, -1))
	{
		lua_pushinteger(L, -1);
		return 1;
	}

	int drvid = lua_tointeger(L, -1);
	lua_pop(L, 1);
	
	if(!ctx)
	{
		lua_pushinteger(L, -1);
		return 1;
	}
	
	uint32_t msglen = 0; 
	uint8_t *msg = smart_coap_tool_context_encode(ctx, &msglen);

	int ret = x12_msg_send(drvid, msg, msglen);
	
	smart_coap_tool_context_free(ctx);
	
	
	lua_pushinteger(L, ret);
	
	return 1;
}

static int x12_lua_coap_api_udp_listen(lua_State* L)
{
	if(!L)
	{
		lua_pushinteger(L, -1);
		return 1;
	}

	x12_socket_udp_t *udp = NULL;

	x12_listen_evens_init();
	x12_even_drivers_init();
	x12_coap_kit_init();

	x12_evens_queue_t *queue = x12_evens_queue_creat();
	x12_evens_queue_pthread_creat(queue, x12_coap_kit_even_callback);

	int id;
	int dret = x12_even_drivers_add(&id, udp, sizeof(*udp), 1);	
	x12_coap_kit_add(id);
	
	x12_lua_coap_kit_rl_creat(id);
	
	lua_pushinteger(L, id);
	return 1;
}

int x12_lua_coap_api_load(lua_State *_sl)
{
	if(!_sl)
	{
		return -1;
	}

	lua_register(_sl, "x12_coap_msg_send", x12_lua_coap_api_msg_send);	
	lua_register(_sl, "x12_coap_udp_listen", x12_lua_coap_api_udp_listen);	
	lua_register(_sl, "x12_coap_response", x12_lua_coap_api_response);	
	lua_register(_sl, "x12_coap_sleep", x12_lua_coap_api_sleep);	
	
	return 0;
}

