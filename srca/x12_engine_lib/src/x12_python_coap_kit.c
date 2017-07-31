/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 python coap 包
 *
 *
 *作者:刘晓龙
 *时间:2017-06_03
 *最后修改时间:2017-06-03
 */

#include "x12_python_coap_kit.h"

PyObject* drv_to_pyobj(int _driverid)
{
/*
 * 预留接口,添加ip等
 *
	PyObject *pArgs = PyDict_New();
	
	PyDict_SetItemString(pArgs, "drvid", Py_BuildValue("i", _driverid));

	x12_even_driver_t* driver = x12_even_drivers_search(_driverid);

	if(driver != NULL)
	{
		if(driver->eventype == 2)
		{
			
		}
	}

	return pArgs;
*/

	return Py_BuildValue("i", _driverid);
}

smart_coap_option_t* pyc_pyobj_to_ctx_getopt(PyObject *_obj)
{
	if(!_obj)
	{
		return NULL;
	}

	smart_coap_option_t *opthead = NULL;
	smart_coap_option_t *optlast = NULL;

	int ret = 0;
	
	PyObject *pKeyList = PyDict_Keys(_obj);

	if(pKeyList == NULL)
	{
		return NULL;
	}


	Py_ssize_t count = PyList_GET_SIZE(pKeyList);

	Py_ssize_t i;
	PyObject *pKItem = NULL;


	for(i = 0; i < count; i++)
	{
		pKItem = PyList_GetItem(pKeyList, i);

		if(pKItem)
		{
			char *key = NULL;
			ret = PyArg_Parse(pKItem, "s", &key);

			if(strcmp(key, "code") == 0
			|| strcmp(key, "ver") == 0
			|| strcmp(key, "msgid") == 0
			|| strcmp(key, "tkl") == 0
			|| strcmp(key, "token") == 0
			|| strcmp(key, "t") == 0
			|| strcmp(key, "payload") == 0)
			{
				continue;
			}
			else if(strcmp(key, "sizel") == 0
			|| strcmp(key, "accept") == 0
			|| strcmp(key, "max-age") == 0
			|| strcmp(key, "uri-port") == 0)
			{
				PyObject *pVItem = PyDict_GetItem(_obj, pKItem);
				if(pVItem != NULL)
				{
					uint32_t val = 0;
					ret = PyArg_Parse(pVItem, "i", &val);
					
					val = htons(val);

					uint32_t delta = sct_ctx_getoptdelta(key);
					
					smart_coap_option_t *opt = (smart_coap_option_t *)malloc(sizeof(smart_coap_option_t));
					memset(opt, 0, sizeof(smart_coap_option_t));

					opt->delta = delta;
					opt->length = sizeof(uint32_t);
					opt->value = (uint8_t *)malloc(opt->length);
					memcpy(opt->value, &val, opt->length);
					

					if(!opthead)
					{
						opthead = opt;
						optlast = opt;
					}
					else
					{
						optlast->next = opt;
						optlast = opt;
					}
				}

			}
			else if(strcmp(key, "content-format") == 0)
			{
				PyObject *pVItem = PyDict_GetItem(_obj, pKItem);

				if(pVItem != NULL)
				{
					char *fmtname = NULL;
					ret = PyArg_Parse(pVItem, "s", &fmtname);

					if(ret != 0)
					{
						uint32_t delta = sct_ctx_getoptdelta(key);
						uint32_t fmtno = sct_ctx_getfmtno(fmtname);

						smart_coap_option_t *opt = (smart_coap_option_t *)malloc(sizeof(smart_coap_option_t));
						memset(opt, 0, sizeof(smart_coap_option_t));
						opt->delta = delta;
						opt->length = sizeof(fmtno);
						opt->value = (uint8_t *)malloc(opt->length);
						memset(opt->value, 0, opt->length);
						memcpy(opt->value, &fmtno, sizeof(fmtno));
					}
				}
			}
			else if(strcmp(key, "proxy-scheme") == 0
			|| strcmp(key, "proxy-uri") == 0
			|| strcmp(key, "location-query") == 0
			|| strcmp(key, "uri-query") == 0
			|| strcmp(key, "uri-path") == 0
			|| strcmp(key, "location-path") == 0
			|| strcmp(key, "uri-host") == 0)
			{

				PyObject *pVItem = PyDict_GetItem(_obj, pKItem);
				if(pVItem != NULL)
				{
					char *val = NULL;
					ret = PyArg_Parse(pVItem, "s", &val);

					if(val != NULL)
					{
						uint32_t delta = sct_ctx_getoptdelta(key);
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

						if(!optlast)
						{
							optlast = opt;
						}
						else
						{
							optlast->next = opt;
							optlast = opt;
						}
					}

				}
			}
			else
			{

				PyObject *pVItem = PyDict_GetItem(_obj, pKItem);

				if(pVItem != NULL)
				{
					char *val = NULL;
					uint32_t length = 0;

					ret = PyArg_Parse(pVItem, "s#", &val, &length);

					uint32_t delta = sct_ctx_getoptdelta(key);
					
					smart_coap_option_t *opt = (smart_coap_option_t *)malloc(sizeof(smart_coap_option_t));
					memset(opt, 0, sizeof(smart_coap_option_t));

					opt->delta = delta;
					opt->length = length;
					opt->value = (uint8_t *)malloc(opt->length);
					memcpy(opt->value, &val, opt->length);

					if(!opthead)
					{
						opthead = opt;
					}

					if(!optlast)
					{
						optlast = opt;
					}
					else
					{
						optlast->next = opt;
						optlast = opt;
					}

				}

			}


			pKItem = NULL;

		}
	}

	smart_coap_option_t *item = opthead;


	return opthead;
}

smart_coap_context_t* pyc_pyobj_to_ctx(PyObject *_obj)
{
	if(!_obj)
	{
		return NULL;
	}

	int ret = 0;

	smart_coap_context_t *ctx = (smart_coap_context_t *)malloc(sizeof(smart_coap_context_t));
	memset(ctx, 0, sizeof(smart_coap_context_t));
	
	PyObject *pVer = PyDict_GetItemString(_obj, "ver");
	if(pVer != NULL)
	{
		uint8_t ver = 0;
		ret = PyArg_Parse(pVer, "i", &ver);
		ctx->ver = ver;
	}

	PyObject *pT = PyDict_GetItemString(_obj, "t");
	if(pT != NULL)
	{
		uint8_t t = 0;

		char *tname = NULL;

		ret = PyArg_Parse(pT, "s", &tname);

		if(strcmp(tname, "CON") == 0)
		{
			t = 0;
		}
		
		if(strcmp(tname, "NON") == 0)
		{
			t = 1;
		}
		
		if(strcmp(tname, "ACK") == 0)
		{
			t = 2;
		}
		
		if(strcmp(tname, "RST") == 0)
		{
			t = 3;
		}

		ctx->t = t;
	}
	
	PyObject *pTKL = PyDict_GetItemString(_obj, "tkl");
	if(pTKL != NULL)
	{
		uint8_t tkl = 0;
		ret = PyArg_Parse(pTKL, "i", &tkl);
		if(tkl > 15)
		{
			tkl = 0;
		}
		ctx->tkl = tkl;
	}

	PyObject *pCode = PyDict_GetItemString(_obj, "code");
	if(pCode != NULL)
	{
		uint8_t code = 0;
		char *codename = NULL;

		ret = PyArg_Parse(pCode, "s", &codename);

		code = sct_ctx_getcodeno(codename);
		
		ctx->code = code;
	}
	
	PyObject *pMsgId = PyDict_GetItemString(_obj, "msgid");
	if(pMsgId != NULL)
	{
		uint16_t msgid = 0;
		ret = PyArg_Parse(pMsgId, "i", &msgid);
		ctx->msgid = msgid;
	}
	
	PyObject *pToken = PyDict_GetItemString(_obj, "pToken");
	if(pToken != NULL)
	{
		uint8_t *tokenp = NULL;
		ret = PyArg_Parse(pToken, "s", &tokenp);
		memcpy(ctx->token, tokenp, ctx->tkl);
	}
	
	PyObject *pPayload = PyDict_GetItemString(_obj, "payload");
	if(pPayload != NULL)
	{
		uint8_t *payload = NULL;
		uint32_t length = 0;
		
		ret = PyArg_Parse(pPayload, "s#", &payload, &length);

		ctx->payloadlen = length;

		if(payload != NULL)
		{
			ctx->payload = (uint8_t *)malloc(length);
			memcpy(ctx->payload, payload, length);
		}
	}
	
	ctx->option = pyc_pyobj_to_ctx_getopt(_obj);
	

	return ctx;

}

PyObject* sct_ctx_to_pyobj(smart_coap_context_t *_ctx)
{
	if(!_ctx)
	{
		return NULL;
	}

	PyObject *pArgs = PyDict_New();

	char *tname = NULL;
	
	if(_ctx->t == 0)
	{
		tname = "CON";
	}
	else if(_ctx->t == 1)
	{
		tname = "NON";
	}
	else if(_ctx->t == 2)
	{
		tname = "ACK";
	}
	else if(_ctx->t == 3)
	{
		tname = "RST";
	}

	PyDict_SetItemString(pArgs, "ver", Py_BuildValue("i", _ctx->ver));
	PyDict_SetItemString(pArgs, "t", Py_BuildValue("s", tname));
	PyDict_SetItemString(pArgs, "tkl", Py_BuildValue("i", _ctx->tkl));
	
	char *codename = sct_ctx_getcodename(_ctx->code);

	PyDict_SetItemString(pArgs, "code", Py_BuildValue("s", codename));
	PyDict_SetItemString(pArgs, "msgid", Py_BuildValue("i", _ctx->msgid));
	PyDict_SetItemString(pArgs, "token", Py_BuildValue("s", _ctx->token));
	
	if(_ctx->payload != NULL)
	{
		PyDict_SetItemString(pArgs, "payload", Py_BuildValue("s#", _ctx->payload, _ctx->payloadlen));
	}

	smart_coap_option_t *item = _ctx->option;
	
	while(item != NULL)
	{
		char *optname = sct_ctx_getoptname(item->delta);
		
		if(optname == NULL)
		{
			item = item->next;
			continue;
		}

		if(item->delta == SMART_COAP_CODE_OPT_URI_PATH)
		{	
			item = item->next;
			continue;
		}
		else if(item->delta == SMART_COAP_CODE_OPT_CONTENT_FORMAT)
		{
			uint32_t fmt = 0;

			if(item->length <= sizeof(fmt))
			{
				memcpy(&fmt, item->value, item->length);
				fmt = ntohs(fmt);
			}

			char *format = sct_ctx_getfmtname(fmt);

			PyDict_SetItemString(pArgs, "content-format", Py_BuildValue("s", format));
		}
		else if(item->delta == SMART_COAP_CODE_OPT_SIZEL
		|| item->delta == SMART_COAP_CODE_OPT_URI_PORT
		|| item->delta == SMART_COAP_CODE_OPT_MAX_AGE
		|| item->delta == SMART_COAP_CODE_OPT_ACCEPT)
		{
			uint32_t value = 0;
			memcpy(&value, item->value, item->length);
			value = ntohs(value);
			PyDict_SetItemString(pArgs, optname, Py_BuildValue("i", value));
		}
		else if(item->delta == SMART_COAP_CODE_OPT_URI_HOST
		|| item->delta == SMART_COAP_CODE_OPT_LOCATION_PATH
		|| item->delta == SMART_COAP_CODE_OPT_URI_QUERY
		|| item->delta == SMART_COAP_CODE_OPT_LOCATION_QUERY
		|| item->delta == SMART_COAP_CODE_OPT_PROXY_URI
		|| item->delta == SMART_COAP_CODE_OPT_PROXY_SCHEME)
		{
			PyDict_SetItemString(pArgs, optname, Py_BuildValue("s", item->value));
		}
		else
		{
			PyDict_SetItemString(pArgs, optname, Py_BuildValue("{s:i,s:s}", "length", item->length, "value", item->value));
		}

		item = item->next;

	}

	char *uri_path =  smart_coap_tool_get_uri(_ctx);

	PyDict_SetItemString(pArgs, "uri-path", Py_BuildValue("s", uri_path));

	
	return pArgs;
}

PyObject* sct_msg_to_pyobj(smart_coap_msg_t *_msg)
{
	if(!_msg || !_msg->ctx)
	{
		return NULL;
	}
	
	PyObject *pArgs = PyDict_New();

	PyObject *pHead = sct_ctx_to_pyobj(_msg->ctx);	

	PyDict_SetItemString(pArgs, "HEAD", Py_BuildValue("O", pHead));

	//偷懒单元测试
	//pyc_pyobj_to_ctx(pHead);

	PyObject* pQuery = PyDict_New();

	smart_coap_msg_query_t *item = _msg->query;

	while(item != NULL)
	{
		PyDict_SetItemString(pQuery, item->name, Py_BuildValue("s", item->value));
		item = item->next;
	}

	PyDict_SetItemString(pArgs, "GET", Py_BuildValue("O", pQuery));

	if(_msg->ctx->payload != NULL)
	{
		PyDict_SetItemString(pArgs, "POST", Py_BuildValue("s#", _msg->ctx->payload, _msg->ctx->payloadlen));
	}

	PyDict_SetItemString(pArgs, "CALLURL", Py_BuildValue("s", _msg->callurl));
	
	return pArgs;
}

x12_py_coap_route_item_t* x12_py_coap_kit_ri_get(int _senderid, char *_uri)
{
	x12_coap_kit_item_t *kit = x12_coap_kit_item_get(_senderid);

	if(!kit)
	{
		return NULL;
	}

	x12_py_coap_route_list_t *list = (x12_py_coap_route_list_t *)(kit->udata);

	if(!list)
	{
		return NULL;
	}

	x12_py_coap_route_item_t *item = list->head;

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

void x12_py_coap_kit_ri_free(x12_py_coap_route_item_t *_ri)
{
	if(_ri)
	{
		x12_py_coap_kit_ri_free(_ri->next);

		free(_ri);
		_ri = NULL;
	}
}

void x12_py_ckd_free(void* _ud)
{
	if(_ud)
	{
		x12_py_coap_route_list_t *list = (x12_py_coap_route_list_t *)_ud;
		x12_py_coap_kit_rl_free(list);
	}
}

int x12_py_coap_kit_uri_cb(int _senderid, smart_coap_msg_t *_msg)
{

	x12_py_coap_route_item_t *item = x12_py_coap_kit_ri_get(_senderid, _msg->callurl);


	if(item)
	{
		void* msg = sct_msg_to_pyobj(_msg);
		void* send = drv_to_pyobj(_senderid);
	
		//printf("run to this path = %s\nmod = %s\nfunc = %s\n", item->path, item->module, item->func);

		x12_python_call_eval(item->path, item->module, item->func, NULL, "(O, O)", send, msg);
	
	}

	return 0;
}

int x12_py_coap_kit_msg_cb(int _senderid, void* _msg)
{

	x12_coap_kit_item_t *kit = x12_coap_kit_item_get(_senderid);

	if(!kit)
	{
		return -1;
	}

	x12_py_coap_route_list_t *list = (x12_py_coap_route_list_t *)(kit->udata);

	if(!list)
	{
		return -1;
	}

	void* msg = sct_msg_to_pyobj(_msg);
	void* send = drv_to_pyobj(_senderid);
	
	x12_python_call_eval(list->mcbpath, list->mcbmodule, list->mcbfunc, NULL, "(O, O)", send, msg);
	

	return 0;
}

int x12_py_coap_kit_rl_creat(int _drvid)
{

	x12_py_coap_route_list_t *list = (x12_py_coap_route_list_t *)malloc(sizeof(x12_py_coap_route_list_t));
	memset(list, 0, sizeof(*list));

	int ret = x12_coap_kit_udata_add(_drvid, list, x12_py_ckd_free);

	return ret;
}

int x12_py_coap_kit_ri_add(int _drvid, char *_uri, char *_path, char *_mod, char *_func)
{
	if(!_uri || !_path || !_mod || !_func)
	{
		return -1;
	}

	x12_coap_kit_item_t *kit = x12_coap_kit_item_get(_drvid);

	if(!kit)
	{
		return -1;
	}

	x12_py_coap_route_list_t *list = (x12_py_coap_route_list_t *)(kit->udata);

	if(!list)
	{
		return -1;
	}

	x12_py_coap_route_item_t *item = (x12_py_coap_route_item_t *)malloc(sizeof(x12_py_coap_route_item_t));
	memset(item, 0, sizeof(*item));
	
	memcpy(item->uri, _uri, strlen(_uri));
	memcpy(item->path, _path, strlen(_path));
	memcpy(item->module, _mod, strlen(_mod));
	memcpy(item->func, _func, strlen(_func));

	if(list->head)
	{
		item->next = list->head;
	}
	
	list->head = item;

	x12_coap_kit_route_add(_drvid, _uri, x12_py_coap_kit_uri_cb);

	return 0;
}

int x12_py_coap_kit_set_msgcb(int _drvid, char *_path, char *_mod, char *_func)
{
	if(!_path || !_mod || !_func)
	{
		return -1;
	}

	x12_coap_kit_item_t *kit = x12_coap_kit_item_get(_drvid);

	if(!kit)
	{
		return -1;
	}

	x12_py_coap_route_list_t *list = (x12_py_coap_route_list_t *)(kit->udata);

	if(!list)
	{
		return -1;
	}

	memset(list->mcbpath, 0, sizeof(list->mcbpath));
	memset(list->mcbmodule, 0, sizeof(list->mcbmodule));
	memset(list->mcbfunc, 0, sizeof(list->mcbfunc));

	memcpy(list->mcbpath, _path, strlen(_path));
	memcpy(list->mcbmodule, _mod, strlen(_mod));
	memcpy(list->mcbfunc, _func, strlen(_func));

	x12_coap_kit_set_msgcallback(_drvid, x12_py_coap_kit_msg_cb);

	return 0;
}

void x12_py_coap_kit_rl_free(x12_py_coap_route_list_t *_rl)
{
	if(_rl)
	{
		x12_py_coap_kit_ri_free(_rl->head);
	}
}


int x12_py_coap_kit_udp_listen(const char *_addr, int _port)
{
	void* server = x12_socket_udp_creat_listen(_addr, _port);

	if(!server)
	{
		return -1;
	}
	
	void* queue = x12_evens_queue_creat();
	x12_evens_queue_pthread_creat(queue, x12_coap_kit_even_callback);
	x12_socket_udp_even_add(server, queue);

	int driverid = x12_socket_udp_driver_add(server);

	if(driverid < 0)
	{
		return -1;
	}

	x12_coap_kit_add(driverid);
	x12_py_coap_kit_rl_creat(driverid);
	
	return driverid;
}

int x12_py_coap_kit_udp_conn(const char *_addr, int _port)
{
	void* conn = x12_socket_udp_creat_connection(_addr, _port);

	if(!conn)
	{
		return -1;
	}

	void* queue = x12_evens_queue_creat();
	x12_evens_queue_pthread_creat(queue, x12_coap_kit_even_callback);
	x12_socket_udp_even_add(conn, queue);

	int drvid = x12_socket_udp_driver_add(conn);

	if(drvid < 0)
	{
		return -1;
	}

	x12_coap_kit_add(drvid);
	x12_py_coap_kit_rl_creat(drvid);

	return drvid;

}

