/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 coap 开发包
 *
 *
 *作者:Crow
 *时间:2017-05-22
 *最后修改时间:2017-05-22
 */

#include "x12_coap_kit.h"

static x12_coap_kit_list_t *kitlist = NULL;

x12_coap_route_table_t* x12_coap_route_table_creat(void)
{
	x12_coap_route_table_t *table = (x12_coap_route_table_t *)malloc(sizeof(x12_coap_route_table_t));
	
	memset(table, 0, sizeof(*table));

	return table;
}

char x12_coap_route_add(x12_coap_route_table_t *_table, const char *_url, x12_coap_route_callback_fp _cb)
{
	if(!_table || !_url || !_cb)
	{
		return -1;
	}

	x12_coap_route_t *item = (x12_coap_route_t *)malloc(sizeof(x12_coap_route_t) + strlen(_url) + 1);
	memset(item, 0, sizeof(x12_coap_route_t) + strlen(_url) + 1);

	item->callback = _cb;
	memcpy(item->url, _url, strlen(_url));

	if(_table->route != NULL)
	{
		item->next = _table->route;
	}

	_table->route = item;
	
	_table->count++;

	return 0;
}

void x12_coap_route_free(x12_coap_route_t *_route)
{
	if(_route != NULL)
	{
		x12_coap_route_free(_route->next);

		free(_route);
		_route = NULL;
	}
}

void x12_coap_route_table_free(x12_coap_route_table_t *_table)
{
	if(_table != NULL)
	{
		x12_coap_route_free(_table->route);

		free(_table);
		_table = NULL;
	}
}

x12_coap_kit_item_t* x12_coap_kit_item_get(int _drvid)
{
	if(!kitlist)
	{
		return NULL;
	}

	x12_even_driver_t *drv = x12_even_drivers_search(_drvid);
	
	if(!drv)
	{
		return NULL;
	}

	int connfd = 0;

	if(drv->eventype == 1 || drv->eventype == 2)
	{
		x12_socket_udp_t *udp = (x12_socket_udp_t *)drv->linkdata;

		connfd = udp->fd;
	}
	else if(drv->eventype == 11 || drv->eventype == 12)
	{
		x12_socket_tcp_t *tcp = (x12_socket_tcp_t *)drv->linkdata;
		connfd = tcp->fd;
	}
	else if(drv->eventype == 13)
	{
		x12_socket_tcp_t *tcp = (x12_socket_tcp_t *)drv->linkdata;
		connfd = tcp->accfd;
	}
	
	x12_coap_kit_item_t *item = kitlist->head;
	
	while(item)
	{
		if(item->connfd == connfd)
		{
			break;
		}

		item = item->next;
	}

	return item;
}

int x12_coap_kit_url_cmp(const char *_senduri, const char *_routeurl)
{
	if(!_senduri || !_routeurl)
	{
		return -1;
	}

	int sendurisize = strlen(_senduri) + 1;
	int routeurlsize = strlen(_routeurl) + 1;

	char *srccp = (char *)malloc(sendurisize);
	memset(srccp, 0, sendurisize);
	memcpy(srccp, _senduri, strlen(_senduri));
	char *uri = srccp;

	char *dstcp = (char *)malloc(routeurlsize);
	memset(dstcp, 0, routeurlsize);
	memcpy(dstcp, _routeurl, strlen(_routeurl));
	char *url = dstcp;
	
	if(uri != NULL && url != NULL)
	{
		
		int i;
		
		for(i = 0; i < strlen(uri); i++)
		{
			if(uri[i] == '\\')
			{
				uri[i] = '/';
			}
		}

		while(uri[0] != '\0' && uri[0] == '/')
		{
			uri++;
		}

		while(strlen(uri) > 0 && uri[strlen(uri) - 1] == '/')
		{
			uri[strlen(uri) - 1] = '\0';
		}

		for(i = 0; i < strlen(url); i++)
		{
			if(url[i] == '\\')
			{
				url[i] = '/';
			}
		}

		while(url[0] == '/' && url[0] != '\0')
		{
			url++;
		}

		while(strlen(url) > 0 && (url[strlen(url) - 1] == '/' || url[strlen(url) - 1] == ':'))
		{
			url[strlen(url) - 1] = '\0';
		}

		int urilen = strlen(uri);
		int urllen = strlen(url);
		
		int uripart = 1;
		int urlpart = 1;

		for(i = 0; i < urilen; i++)
		{
			if(uri[i] == '/')
			{
				uripart++;
			}
		}

		for(i = 0; i < urllen; i++)
		{
			if(url[i] == '/')
			{
				urlpart++;
			}
		}


		if(uripart == urlpart)
		{
			char* uris[uripart];
			char* urls[urlpart];

			int offset = 0;

			uris[offset++] = uri;

			for(i = 0; i < urilen; i++)
			{
				if(uri[i] == '/')
				{
					uri[i] = '\0';
				}

				if(i != 0 && uri[i - 1] == '\0')
				{
					if(offset < uripart)
					{
						uris[offset++] = uri + i;
					}
					else
					{
						break;
					}
				}

			}

			offset = 0;

			urls[offset++] = url;
	
			for(i = 0; i < urllen; i++)
			{
				if(url[i] == '/')
				{
					url[i] = '\0';
				}
			
				if(i != 0 && url[i - 1] == '\0')
				{
					if(offset < urlpart)
					{
						urls[offset++] = url + i;
					}
					else
					{
						break;
					}
				}
			}

			for(i = 0; i < urlpart; i++)
			{
				if(strcmp(uris[i], urls[i]) != 0)
				{
					if(urls[i][0] != ':')
					{
						return -1;
					}

				}

			}


		}
		else
		{
			return -1;
		}

		free(srccp);
		srccp = NULL;

		free(dstcp);
		dstcp = NULL;

	}
	else
	{
		return -1;
	}

	return 0;
}

void x12_coap_kit_init(void)
{
	if(!kitlist)
	{
		kitlist = (x12_coap_kit_list_t *)malloc(sizeof(x12_coap_kit_list_t));
		memset(kitlist, 0, sizeof(x12_coap_kit_list_t));
	}
}

int x12_coap_kit_add(int _drvid)
{
	int ret = 0;
	
	x12_even_driver_t *drv = x12_even_drivers_search(_drvid);

	if(!drv || !kitlist)
	{
		return -1;
	}

	x12_coap_kit_item_t *item = (x12_coap_kit_item_t *)malloc(sizeof(x12_coap_kit_item_t));
	memset(item, 0, sizeof(x12_coap_kit_item_t));

	if(drv->eventype == 1 || drv->eventype == 2)
	{
		x12_socket_udp_t *udp = (x12_socket_udp_t *)drv->linkdata;

		item->connfd = udp->fd;
	}
	else if(drv->eventype == 11 || drv->eventype == 12 || drv->eventype == 13)
	{
		x12_socket_tcp_t *tcp = (x12_socket_tcp_t *)drv->linkdata;

		item->connfd = tcp->fd;
	}

	item->routetable = x12_coap_route_table_creat();

	if(kitlist->head)
	{
		item->next = kitlist->head;
		kitlist->head = item;
	}
	else
	{
		kitlist->head = item;
	}
	
	return ret;
}

int x12_coap_kit_route_add(int _drvid, const char *_url, void* _callback)
{
	x12_coap_kit_item_t *item = x12_coap_kit_item_get(_drvid);

	if(!item)
	{
		return -1;
	}

	int ret = x12_coap_route_add(item->routetable, _url, _callback);

	return ret;
}

int x12_coap_kit_set_msgcallback(int _drvid, void* _callback)
{
	x12_coap_kit_item_t *item = x12_coap_kit_item_get(_drvid);

	if(!item)
	{
		return -1;
	}
	
	if(item->routetable)
	{
		item->routetable->msgcallback = _callback;
	}

	return 0;
}

void x12_coap_kit_free(x12_coap_kit_item_t *_item)
{
	if(_item)
	{
		if(_item->udata && _item->freefunc)
		{
			_item->freefunc(_item->udata);
		}

		x12_coap_kit_free(_item->next);
		x12_coap_route_table_free(_item->routetable);
	}
}

void x12_coap_kit_destroy()
{
	if(kitlist)
	{
		x12_coap_kit_free(kitlist->head);

		free(kitlist);
		kitlist = NULL;
	}
}

x12_coap_route_t* x12_coap_kit_route_search(void* _kit, const char *_uri)
{
	if(!_uri || !_kit)
	{
		return NULL;
	}

	x12_coap_route_table_t *route_table = (x12_coap_route_table_t *)_kit;

	if(route_table == NULL)
	{
		return NULL;
	}

	x12_coap_route_t *item = route_table->route;

	while(item != NULL)
	{
		if(x12_coap_kit_url_cmp(_uri, item->url) == 0)
		{
			break;
		}

		item = item->next;
	}

	return item;
}

int x12_coap_kit_callback(int _senderid, uint8_t *_buff, uint32_t _bufflen)
{
	x12_coap_kit_item_t *item = x12_coap_kit_item_get(_senderid);

	if(!item)
	{
		return -1;
	}

	x12_coap_route_table_t *route_table = item->routetable;
	
	int ret = 0;
	smart_coap_context_t *ctx = smart_coap_tool_context_get(_buff, _bufflen);
	char* uri = smart_coap_tool_get_uri(ctx);
	
	if(uri == NULL || strlen(uri) == 0)
	{
		if(route_table != NULL && route_table->msgcallback)
		{
			smart_coap_msg_t *msg = smart_coap_tool_msg_restful_get(ctx, NULL);
			ret = route_table->msgcallback(_senderid, msg);

			return ret;
		}
	}

	x12_coap_route_t *route = x12_coap_kit_route_search(route_table, uri);

	if(route == NULL)
	{
		if(route_table != NULL && route_table->msgcallback)
		{
			smart_coap_msg_t *msg = smart_coap_tool_msg_restful_get(ctx, NULL);
			ret = route_table->msgcallback(_senderid, msg);

			return ret;
		}
		else
		{
			return ret;
		}
	}
	
	smart_coap_msg_t *msg = smart_coap_tool_msg_restful_get(ctx, route->url);

	if(msg != NULL)
	{
		ret = route->callback(_senderid, msg);
		smart_coap_tool_msg_free(msg);
		msg = NULL;
	}
	
	if(uri != NULL)
	{
		free(uri);
		uri = NULL;
	}

	return ret;
}

char x12_coap_kit_even_callback(void* _queue)
{
	int count = 10;
	int index = 0;

	struct x12_evens_queue_item_s evens[count];
	memset(&evens, 0, sizeof(x12_evens_queue_item_t) * count);

	while(index < count)
	{
		int isempty = x12_evens_queue_isempty(_queue);
		if(isempty == 1)
		{
			x12_evens_queue_giveupdenter(_queue, index);
			break;
		}
		else if(isempty == -1)
		{
			break;
		}
		else
		{
			char ret = x12_evens_queue_denter(_queue, &evens[index++]);
					
			if(ret == 0)
			{
				int i;
				i = index - 1;
				int checkret = 	smart_coap_tool_data_check(evens[i].data, evens[i].datalen);
				
				if(checkret == 0)
				{
					x12_coap_kit_callback(evens[i].evendriverid, evens[i].data, evens[i].datalen);
					break;
				}

				uint32_t bufflen = 0;	
				uint32_t offset = 0;
					
				for(i = 0; i < index; i++)
				{
					bufflen += evens[i].datalen;
				}

				uint8_t buff[bufflen];
				memset(buff, 0, bufflen);
				
				for(i = 0; i < index; i++)
				{
					memcpy(buff + offset, evens[i].data, evens[i].datalen);
					offset += evens[i].datalen;
				}
				
				checkret = 	smart_coap_tool_data_check(buff, bufflen);

				if(checkret == 0)
				{
					i = index - 1;
					x12_coap_kit_callback(evens[i].evendriverid, buff, bufflen);
				}
				else if(checkret == 2)
				{
					break;
				}
				else if(checkret == 1)
				{
					continue;
				}

			}

		}
	}

	return 0;
}

int x12_coap_kit_udata_add(int _drvid, void* _udata, x12_coap_kitud_free _func)
{
	if(!_udata || !_func)
	{
		return -1;
	}

	x12_coap_kit_item_t *kit = x12_coap_kit_item_get(_drvid);

	if(!kit)
	{
		return -1;
	}

	kit->udata = _udata;
	kit->freefunc = _func;

	return 0;

}

