/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *smart coap 解析工具
 *
 *
 *作者:Crow
 *时间:2017-05-10
 *最后修改时间:2017-05-10
 */

#include <stdio.h>
#include <unistd.h>
#include "x12.h"
#include "x12_even.h"
#include "x12_select.h"
#include "x12_even_drivers.h"
#include "x12_even_queue.h"
#include "x12_socket_tcp.h"
#include "x12_socket_udp.h"
#include "x12_socket_raw.h"
#include "x12_msg.h"
#include "smart_coap_tool.h"

uint32_t sct_ctx_getcodeno(const char *_name)
{
	if(!_name)
	{
		return 0;
	}

	uint32_t codeno = 0;

	if(strcmp(_name, "GET") == 0)
	{
		codeno = SMART_COAP_CODE_REQ_GET;
	}
	else if(strcmp(_name, "POST") == 0)
	{
		codeno = SMART_COAP_CODE_REQ_POST;
	}
	else if(strcmp(_name, "PUT") == 0)
	{
		codeno = SMART_COAP_CODE_REQ_PUT;
	}
	else if(strcmp(_name, "DELETE") == 0)
	{
		codeno = SMART_COAP_CODE_REQ_DELETE;
	}
	else if(strcmp(_name, "Created") == 0)
	{
		codeno = SMART_COAP_CODE_RES_CREATED;
	}
	else if(strcmp(_name, "Deleted") == 0)
	{
		codeno = SMART_COAP_CODE_RES_DELETED;
	}
	else if(strcmp(_name, "Valid") == 0)
	{
		codeno = SMART_COAP_CODE_RES_VALID;
	}
	else if(strcmp(_name, "Changed") == 0)
	{
		codeno = SMART_COAP_CODE_RES_CHANGED;
	}
	else if(strcmp(_name, "Content") == 0)
	{
		codeno = SMART_COAP_CODE_RES_CONTENT;
	}
	else if(strcmp(_name, "Bad-Request") == 0)
	{
		codeno = SMART_COAP_CODE_RES_BAD_REQUEST;
	}
	else if(strcmp(_name, "Unauthorized") == 0)
	{
		codeno = SMART_COAP_CODE_RES_UNAUTHORIZED;
	}
	else if(strcmp(_name, "Bad-Option") == 0)
	{
		codeno = SMART_COAP_CODE_RES_BAD_OPTION;
	}
	else if(strcmp(_name, "Forbidden") == 0)
	{
		codeno = SMART_COAP_CODE_RES_FORBIDDEN;
	}
	else if(strcmp(_name, "Not-Found") == 0)
	{
		codeno = SMART_COAP_CODE_RES_NOT_FOUND;
	}
	else if(strcmp(_name, "Method-Not-Allowed") == 0)
	{
		codeno = SMART_COAP_CODE_RES_METHOD_NOT_ALLOWED;
	}
	else if(strcmp(_name, "Not-Acceptable") == 0)
	{
		codeno = SMART_COAP_CODE_RES_NOT_ACCEPTABLE;
	}
	else if(strcmp(_name, "Precondition-Failed") == 0)
	{
		codeno = SMART_COAP_CODE_RES_PRECONDITION_FAILED;
	}
	else if(strcmp(_name, "Unsuppor-Conten-Type") == 0)
	{
		codeno = SMART_COAP_CODE_RES_UNSUPPOR_CONTEN_TYPE;
	}
	else if(strcmp(_name, "Internal-Server-Error") == 0)
	{
		codeno = SMART_COAP_CODE_RES_INTERNAL_SERVER_ERROR;
	}
	else if(strcmp(_name, "Not-Implemented") == 0)
	{
		codeno = SMART_COAP_CODE_RES_NOT_IMPLEMENTED;
	}
	else if(strcmp(_name, "Bad-Gateway") == 0)
	{
		codeno = SMART_COAP_CODE_RES_BAD_GATEWAY;
	}
	else if(strcmp(_name, "Service-Unavailable") == 0)
	{
		codeno = SMART_COAP_CODE_RES_SERVICE_UNAVAILABLE;
	}
	else if(strcmp(_name, "Gateway-Timeout") == 0)
	{
		codeno = SMART_COAP_CODE_RES_GATEWAY_TIMEOUT;
	}
	else if(strcmp(_name, "Proxying-Not-Supported") == 0)
	{
		codeno = SMART_COAP_CODE_RES_PROXYING_NOT_SUPPORTED;
	}
	else
	{
		codeno = atoi(_name);
	}

	return codeno;
}

uint32_t sct_ctx_getfmtno(const char *_name)
{
	if(!_name)
	{
		return 0;
	}

	uint32_t fmtno = 0;

	if(strcmp(_name, "text/plain") == 0)
	{
		fmtno = SMART_COAP_CODE_CONTEXT_FORMAT_TXT;
	}
	else if(strcmp(_name, "application/link-format") == 0)
	{
		fmtno = SMART_COAP_CODE_CONTEXT_FORMAT_APP_LINK;
	}
	else if(strcmp(_name, "application/xml") == 0)
	{
		fmtno = SMART_COAP_CODE_CONTEXT_FORMAT_APP_XML;
	}
	else if(strcmp(_name, "application/octet-stream") == 0)
	{
		fmtno = SMART_COAP_CODE_CONTEXT_FORMAT_APP_OS;
	}
	else if(strcmp(_name, "application/exi") == 0)
	{
		fmtno = SMART_COAP_CODE_CONTEXT_FORMAT_APP_EXI;
	}
	else if(strcmp(_name, "application/json") == 0)
	{
		fmtno = SMART_COAP_CODE_CONTEXT_FORMAT_APP_JSON;
	}
	else
	{
		fmtno = atoi(_name);
	}


	return fmtno;
}

uint32_t sct_ctx_getoptdelta(const char *_name)
{
	uint32_t delta = 0;

	if(strcmp(_name, "if-match") == 0)
	{
		delta = SMART_COAP_CODE_OPT_IF_MATCH;
	}
	else if(strcmp(_name, "sizel") == 0)
	{
		delta = SMART_COAP_CODE_OPT_SIZEL;
	}
	else if(strcmp(_name, "proxy-scheme") == 0)
	{
		delta = SMART_COAP_CODE_OPT_PROXY_SCHEME;
	}
	else if(strcmp(_name, "proxy-uri") == 0)
	{
		delta = SMART_COAP_CODE_OPT_PROXY_URI;
	}
	else if(strcmp(_name, "location-query") == 0)
	{
		delta = SMART_COAP_CODE_OPT_LOCATION_QUERY;
	}
	else if(strcmp(_name, "accept") == 0)
	{
		delta = SMART_COAP_CODE_OPT_ACCEPT;
	}
	else if(strcmp(_name, "uri-query") == 0)
	{
		delta = SMART_COAP_CODE_OPT_URI_QUERY;
	}
	else if(strcmp(_name, "max-age") == 0)
	{
		delta = SMART_COAP_CODE_OPT_MAX_AGE;
	}
	else if(strcmp(_name, "content-format") == 0)
	{
		delta = SMART_COAP_CODE_OPT_CONTENT_FORMAT;
	}
	else if(strcmp(_name, "uri-path") == 0)
	{
		delta = SMART_COAP_CODE_OPT_URI_PATH;
	}
	else if(strcmp(_name, "location-path") == 0)
	{
		delta = SMART_COAP_CODE_OPT_LOCATION_PATH;
	}
	else if(strcmp(_name, "uri-port") == 0)
	{
		delta = SMART_COAP_CODE_OPT_URI_PORT;
	}
	else if(strcmp(_name, "if-none-match") == 0)
	{
		delta = SMART_COAP_CODE_OPT_IF_NONE_MATCH;
	}
	else if(strcmp(_name, "etag") == 0)
	{
		delta = SMART_COAP_CODE_OPT_ETAG;
	}
	else
	{
		delta = atoi(_name);
	}

	return delta;
}

char* sct_ctx_getoptname(uint32_t _delta)
{
	char *optname = NULL;

	if(_delta == SMART_COAP_CODE_OPT_IF_MATCH)
	{
		optname = "if-match";
	}
	else if(_delta == SMART_COAP_CODE_OPT_SIZEL)
	{
		optname = "sizel";
	}
	else if(_delta == SMART_COAP_CODE_OPT_PROXY_SCHEME)
	{
		optname = "proxy-scheme";
	}
	else if(_delta == SMART_COAP_CODE_OPT_PROXY_URI)
	{
		optname = "proxy-uri";
	}
	else if(_delta == SMART_COAP_CODE_OPT_LOCATION_QUERY)
	{
		optname = "location-query";
	}
	else if(_delta == SMART_COAP_CODE_OPT_ACCEPT)
	{
		optname = "accept";
	}
	else if(_delta == SMART_COAP_CODE_OPT_URI_QUERY)
	{
		optname = "uri-query";
	}
	else if(_delta == SMART_COAP_CODE_OPT_MAX_AGE)
	{
		optname = "max-age";
	}
	else if(_delta == SMART_COAP_CODE_OPT_CONTENT_FORMAT)
	{
		optname = "content-format";
	}
	else if(_delta == SMART_COAP_CODE_OPT_URI_PATH)
	{
		optname = "uri-path";
	}
	else if(_delta == SMART_COAP_CODE_OPT_LOCATION_PATH)
	{
		optname = "location-path";
	}
	else if(_delta == SMART_COAP_CODE_OPT_URI_PORT)
	{
		optname = "uri-port";
	}
	else if(_delta == SMART_COAP_CODE_OPT_IF_NONE_MATCH)
	{
		optname = "if-none-match";
	}
	else if(_delta == SMART_COAP_CODE_OPT_ETAG)
	{
		optname = "etag";
	}
	else if(_delta == SMART_COAP_CODE_OPT_URI_HOST)
	{
		optname = "uri-host";
	}
	else
	{
		char optnumber[16] = {0};
		sprintf(optnumber, "%d", _delta);
		optname = optnumber;
	}

	return optname;
}

char* sct_ctx_getfmtname(uint32_t _fmt)
{
	char *fmtname = NULL;

	if(_fmt == SMART_COAP_CODE_CONTEXT_FORMAT_TXT)
	{
		fmtname = "text/plain";
	}
	else if(_fmt == SMART_COAP_CODE_CONTEXT_FORMAT_APP_LINK)
	{
		fmtname = "application/link-format";
	}
	else if(_fmt == SMART_COAP_CODE_CONTEXT_FORMAT_APP_XML)
	{
		fmtname = "application/xml";
	}
	else if(_fmt == SMART_COAP_CODE_CONTEXT_FORMAT_APP_OS)
	{
		fmtname = "application/octet-stream";
	}
	else if(_fmt == SMART_COAP_CODE_CONTEXT_FORMAT_APP_EXI)
	{
		fmtname = "application/exi";
	}
	else if(_fmt == SMART_COAP_CODE_CONTEXT_FORMAT_APP_JSON)
	{
		fmtname = "application/json";
	}
	else
	{
		char fmtnumber[16] = {0};
		sprintf(fmtnumber, "%d", _fmt);
		fmtname = fmtnumber;
	}

	return fmtname;
}

char* sct_ctx_getcodename(uint32_t _codeno)
{
	char *codename = NULL;

	if(_codeno == SMART_COAP_CODE_REQ_GET)
	{
		codename = "GET";
	}
	else if(_codeno == SMART_COAP_CODE_REQ_DELETE)
	{
		codename = "DELETE";
	}
	else if(_codeno == SMART_COAP_CODE_REQ_PUT)
	{
		codename = "PUT";
	}
	else if(_codeno == SMART_COAP_CODE_REQ_POST)
	{
		codename = "POST";
	}
	else if(_codeno == SMART_COAP_CODE_RES_CREATED)
	{
		codename = "Created";
	}
	else if(_codeno == SMART_COAP_CODE_RES_CONTENT)
	{
		codename = "Content";
	}
	else if(_codeno == SMART_COAP_CODE_RES_PROXYING_NOT_SUPPORTED)
	{
		codename = "Proxying-Not-Supported";
	}
	else if(_codeno == SMART_COAP_CODE_RES_GATEWAY_TIMEOUT)
	{
		codename = "Gateway-Timeout";
	}
	else if(_codeno == SMART_COAP_CODE_RES_SERVICE_UNAVAILABLE)
	{
		codename = "Service-Unavailable";
	}
	else if(_codeno == SMART_COAP_CODE_RES_BAD_GATEWAY)
	{
		codename = "Bad-Gateway";
	}
	else if(_codeno == SMART_COAP_CODE_RES_NOT_IMPLEMENTED)
	{
		codename = "Not-Implemented";
	}
	else if(_codeno == SMART_COAP_CODE_RES_INTERNAL_SERVER_ERROR)
	{
		codename = "Internal-Server-Error";
	}
	else if(_codeno == SMART_COAP_CODE_RES_UNSUPPOR_CONTEN_TYPE)
	{
		codename = "Unsuppor-Conten-Type";
	}
	else if(_codeno == SMART_COAP_CODE_RES_PRECONDITION_FAILED)
	{
		codename = "Precondition-Failed";
	}
	else if(_codeno == SMART_COAP_CODE_RES_NOT_ACCEPTABLE)
	{
		codename = "Not-Acceptable";
	}
	else if(_codeno == SMART_COAP_CODE_RES_METHOD_NOT_ALLOWED)
	{
		codename = "Method-Not-Allowed";
	}
	else if(_codeno == SMART_COAP_CODE_RES_NOT_FOUND)
	{
		codename = "Not-Found";
	}
	else if(_codeno == SMART_COAP_CODE_RES_FORBIDDEN)
	{
		codename = "Forbidden";
	}
	else if(_codeno == SMART_COAP_CODE_RES_BAD_OPTION)
	{
		codename = "Bad-Option";
	}
	else if(_codeno == SMART_COAP_CODE_RES_UNAUTHORIZED)
	{
		codename = "Unauthorized";
	}
	else if(_codeno == SMART_COAP_CODE_RES_BAD_REQUEST)
	{
		codename = "Bad Request";
	}
	else if(_codeno == SMART_COAP_CODE_RES_CHANGED)
	{
		codename = "Changed";
	}
	else if(_codeno == SMART_COAP_CODE_RES_VALID)
	{
		codename = "Valid";
	}
	else if(_codeno == SMART_COAP_CODE_RES_DELETED)
	{
		codename = "Deleted";
	}
	else
	{
		char codenumber[16] = {0};
		sprintf(codenumber, "%d", _codeno);
		codename = codenumber;
	}

	return codename;
}

void smart_coap_tool_printbyte(unsigned char *_buff, int _bufflen)
{
	int i;

	printf("\n");

	for(i = 0; i < _bufflen; i++)
	{
		printf("0x%02x ", _buff[i]);

		if((i + 1) % 8 == 0)
		{
			printf("\n");
		}
	}

	printf("\n\n");

	smart_coap_context_t *ctx = smart_coap_tool_context_get(_buff, _bufflen);

	if(ctx != NULL)
	{
		printf("v:%d t:%d tkl:%d c:%d id:%d\n", ctx->ver, ctx->t, ctx->tkl, ctx->code, ctx->msgid);

		if(ctx->option != NULL)
		{

			smart_coap_option_t *opt = ctx->option;

			while(opt != NULL)
			{
				printf("delta:%d, length:%d, value:%s\n", opt->delta, opt->length, opt->value);

				opt = opt->next;
			}
		}

		char *uri = smart_coap_tool_get_uri(ctx);

		printf("\nuri:%s\n", uri);
		
		char *url = "/test/:id/:pwd";
		printf("url:%s\n", url);

		//smart_coap_msg_t *msg = smart_coap_tool_msg_restful_get(ctx, url);


		ctx->code = SMART_COAP_CODE_RES_CONTENT;

		uint8_t val[32];
		memset(val, 0, sizeof(val));
		memcpy(val, "hello world", 11);

		smart_coap_tool_context_set_payload(ctx, val, sizeof(val));
	
		uint8_t tk[12];
		memset(tk, 0, sizeof(tk));
		smart_coap_tool_context_set_token(ctx, tk, sizeof(tk));
		
		uint8_t sizel = 32;
		smart_coap_tool_context_add_opt(ctx, SMART_COAP_CODE_OPT_SIZEL, 1, &sizel);

	}
/*
	uint32_t datalen;
	uint8_t *data = smart_coap_tool_context_encode(ctx, &datalen);

	printf("\n");
	printf("encode datalen : %d\n", datalen);

	for(i = 0; i < datalen; i++)
	{
		printf("0x%02x ", data[i]);

		if((i + 1) % 8 == 0)
		{
			printf("\n");
		}
	}


	printf("\n\n");

	printf("\n");

	smart_coap_tool_context_free(ctx);

	if(data != NULL)
	{
		free(data);
		data = NULL;
	}

*/

}

void smart_coap_tool_option_free(smart_coap_option_t *_item)
{
	if(_item != NULL)
	{
		smart_coap_tool_option_free(_item->next);

		if(_item->value != NULL)
		{
			free(_item->value);
			_item->value = NULL;
		}
		
		free(_item);
		_item = NULL;
	}
}

uint32_t smart_coap_tool_option_count(smart_coap_context_t *_ctx)
{
	if(!_ctx)
	{
		return 0;
	}

	uint32_t count = 0;

	smart_coap_option_t *item = _ctx->option;

	while(item != NULL)
	{
		count++;
		item = item->next;

	}

	return count;
}

smart_coap_option_t* smart_coap_tool_option_get(uint8_t *_buff, uint32_t _bufflen, uint32_t *_offset, uint32_t *_delta, uint8_t *_errno)
{

	uint32_t offset = *_offset;
	uint8_t err_no = 0;

	uint32_t opt_delta = *_delta;
	uint32_t opt_len = 0;

	uint8_t opt_delta_item = 0;
	uint8_t opt_len_item = 0;
	memcpy(&opt_delta_item, _buff + offset, sizeof(opt_delta_item));
	memcpy(&opt_len_item, _buff + offset, sizeof(opt_len_item));
	opt_delta_item = (opt_delta_item >> 4) & 0x0F;
	opt_len_item = opt_len_item & 0x0F;
	offset += sizeof(uint8_t);

	if(opt_delta_item == 15 || opt_len_item == 15)
	{
		err_no = 1;
		goto get_opt_fail;
	}

	opt_delta += opt_delta_item;
	opt_len += opt_len_item;

	if(opt_delta_item == 13)
	{
		
		if(_bufflen < offset + 1)
		{
			err_no = 1;
			goto get_opt_fail;
		}

		uint8_t opt_delta_exp = 0;
		memcpy(&opt_delta_exp, _buff + offset, sizeof(opt_delta_exp));
		opt_delta += opt_delta_exp;
		offset += sizeof(opt_delta_exp);
	}
	else if(opt_delta_item == 14)
	{
		if(_bufflen < offset + 2)
		{
			err_no = 1;
			goto get_opt_fail;
		}

		uint16_t opt_delta_exp = 0;
		memcpy(&opt_delta_exp, _buff + offset, sizeof(opt_delta_exp));
		opt_delta_exp = ntohs(opt_delta_exp);
		opt_delta += 255 + opt_delta_exp;
		offset += sizeof(opt_delta_exp);
	}

	if(opt_len_item == 13)
	{
		if(_bufflen < offset + 1)
		{
			err_no = 1;
			goto get_opt_fail;
		}

		uint8_t opt_len_exp = 0;
		memcpy(&opt_len_exp, _buff + offset, sizeof(opt_len_exp));
		opt_len = 13 + opt_len_exp;
		offset += sizeof(opt_len_exp);


	}
	else if(opt_len_item == 14)
	{
		
		if(_bufflen < offset + 2)
		{
			err_no = 1;
			goto get_opt_fail;
		}

		uint16_t opt_len_exp = 0;
		memcpy(&opt_len_exp, _buff + offset, sizeof(opt_len_exp));
		opt_len_exp = ntohs(opt_len_exp);
		opt_len = 14 + 255 + opt_len_exp;
		offset += sizeof(opt_len_exp);
	}

	if(_bufflen < offset + opt_len)
	{
		err_no = 1;
		goto get_opt_fail;
	}

	uint32_t alloc_len = opt_len;

	if(opt_delta == SMART_COAP_CODE_OPT_URI_HOST 
	|| opt_delta == SMART_COAP_CODE_OPT_LOCATION_PATH
	|| opt_delta == SMART_COAP_CODE_OPT_URI_PATH 
	|| opt_delta == SMART_COAP_CODE_OPT_URI_QUERY
	|| opt_delta == SMART_COAP_CODE_OPT_LOCATION_QUERY
	|| opt_delta == SMART_COAP_CODE_OPT_PROXY_URI
	|| opt_delta == SMART_COAP_CODE_OPT_PROXY_SCHEME)
	{
		alloc_len++;
	}

	uint8_t *opt_value = (uint8_t *)malloc(alloc_len);
	memset(opt_value, 0, alloc_len);
	memcpy(opt_value, _buff + offset, opt_len);

	offset += opt_len;

	smart_coap_option_t *opt = (smart_coap_option_t *)malloc(sizeof(smart_coap_option_t));
	memset(opt, 0, sizeof(smart_coap_option_t));
	
	opt->delta = opt_delta;
	opt->length = alloc_len;
	opt->value = opt_value;

	*_errno = err_no;
	*_offset = offset;
	*_delta = opt_delta;

	return opt;

get_opt_fail:
	
	*_errno = err_no;
	*_offset = offset;
	
	return NULL;
}

int smart_coap_tool_data_check(uint8_t *_buff, uint32_t _bufflen)
{
	if(!_buff)
	{
		return -1;
	}

	if(_bufflen < 4)
	{
		return 2;
	}

	uint32_t offset = 0;

	uint8_t vtl;	//ver t tkl
	memcpy(&vtl, _buff + offset, sizeof(vtl));

	uint8_t ver = (vtl >> 6) & 0x03;
	uint8_t tkl = vtl & 0x0F;

	offset += sizeof(vtl);

	if(ver != 1)
	{
		return 2;
	}

	if(_bufflen < 4 + tkl)
	{
		return 2;
	}

	uint8_t code;
	memcpy(&code, _buff + offset, sizeof(code));

	offset += sizeof(code);

	if(code == 0 || code == 0xFF)
	{
		return 2;
	}

	//msgid 不检查
	offset += sizeof(uint16_t);

	//token 不检查
	offset += tkl;
	
	uint32_t sizel = 0;
	uint32_t payloadlen = 0;

	if(_buff[offset] == 0xFF)
	{
		return 0;
	}

	smart_coap_option_t *opthead;
	smart_coap_option_t *optlast;
	
	uint8_t err_no;
	uint32_t opt_delta = 0;
	opthead = smart_coap_tool_option_get(_buff, _bufflen, &offset, &opt_delta, &err_no);
	optlast = opthead;

	if(opthead == NULL)
	{
		return 2;
	}

	while(_bufflen > offset && _buff[offset] != 0xFF)
	{
		smart_coap_option_t *optitem = smart_coap_tool_option_get(_buff, _bufflen, &offset, &opt_delta, &err_no);
		
		if(optitem != NULL)
		{
			optlast->next = optitem;
			optlast = optitem;
		}
		else
		{
			smart_coap_tool_option_free(opthead);
			return 2;
		}
	}

	smart_coap_option_t *item = opthead;
	
	while(item != NULL)
	{
		if(item->delta == SMART_COAP_CODE_OPT_SIZEL)
		{
			int sizeloffset = sizeof(sizel) - item->length;

			if(sizeloffset < 0)
			{
				smart_coap_tool_option_free(opthead);
				return 2;
			}
			else if(sizeloffset == sizeof(sizel))
			{
				sizel = 0;
			}
			else
			{
				memcpy(&sizel + sizeloffset, item->value, item->length);
			}

			sizel = ntohs(sizel);

			break;
		}

		item = item->next;
	}

	smart_coap_tool_option_free(opthead);
	
	if(_bufflen > offset)
	{
		if(_buff[offset] == 0xFF)
		{
			offset++;
			payloadlen = _bufflen - offset;
		
			if(payloadlen < sizel)
			{
				return 1;
			}
		
		}
		else
		{
			return 2;
		}
	}

	return 0;
}

smart_coap_context_t* smart_coap_tool_context_get(uint8_t *_buff, uint32_t _bufflen)
{
	if(!_buff)
	{
		return NULL;
	}

	if(_bufflen < 4)
	{
		return NULL;
	}

	uint32_t offset = 0;

	uint8_t vtl;	//ver t tkl
	memcpy(&vtl, _buff + offset, sizeof(vtl));

	uint8_t ver = (vtl >> 6) & 0x03;
	uint8_t t = (vtl >> 4) & 0x03;
	uint8_t tkl = vtl & 0x0F;

	offset += sizeof(vtl);

	uint8_t code;
	memcpy(&code, _buff + offset, sizeof(code));

	offset += sizeof(code);

	uint16_t msgid;
	memcpy(&msgid, _buff + offset, sizeof(msgid));
	msgid = ntohs(msgid);
	
	offset += sizeof(msgid);

	if(_bufflen < offset + tkl)
	{
		return NULL;
	}

	uint8_t token[15];
	memset(token, 0, sizeof(token));
	memcpy(token, _buff + offset, tkl);

	offset += tkl;

	smart_coap_context_t *ctx = (smart_coap_context_t *)malloc(sizeof(smart_coap_context_t));
	memset(ctx, 0, sizeof(smart_coap_context_t));

	ctx->ver = ver;
	ctx->t = t;
	ctx->tkl = tkl;
	ctx->code = code;
	ctx->msgid = msgid;
	memcpy(ctx->token, token, sizeof(token));

	if(_bufflen <= offset)
	{
		return ctx;
	}

	uint8_t *payload = NULL;
	uint32_t payloadlen = 0;

	if(_buff[offset] == 0xFF)
	{
		offset++;
		payloadlen = _bufflen - offset;
		payload = (uint8_t *)malloc(payloadlen);
		memcpy(payload, _buff + offset, payloadlen);

		goto get_ctx_success;
	}

	smart_coap_option_t *opthead;
	smart_coap_option_t *optlast;
	
	uint8_t err_no;
	uint32_t opt_delta = 0;
	opthead = smart_coap_tool_option_get(_buff, _bufflen, &offset, &opt_delta, &err_no);
	optlast = opthead;

	if(opthead == NULL)
	{
		goto get_ctx_fail;
	}

	while(_bufflen > offset && _buff[offset] != 0xFF)
	{
		smart_coap_option_t *optitem = smart_coap_tool_option_get(_buff, _bufflen, &offset, &opt_delta, &err_no);
		
		if(optitem != NULL)
		{
			optlast->next = optitem;
			optlast = optitem;
		}
		else
		{
			goto get_ctx_fail;
		}
	}

	ctx->option = opthead;

	if(_bufflen > offset)
	{
		if(_buff[offset] == 0xFF)
		{
			offset++;
			payloadlen = _bufflen - offset;
			payload = (uint8_t *)malloc(payloadlen);
			memcpy(payload, _buff + offset, payloadlen);
		}
		else
		{
			goto get_ctx_fail;
		}
	}

get_ctx_success:
	
	ctx->payload = payload;
	ctx->payloadlen = payloadlen;

	return ctx;

get_ctx_fail:

	if(ctx != NULL)
	{
		free(ctx);
		ctx = NULL;
	}

	if(opthead != NULL)
	{
		smart_coap_tool_option_free(opthead);
	}

	return NULL;

}

uint8_t* smart_coap_tool_option_encode(uint32_t _delta, uint32_t _length, uint8_t *_value, uint32_t *_datalen)
{
	uint32_t opt_delta_item = _delta;
	uint32_t opt_len_item = _length;

	uint32_t datalen = 1;
	uint8_t optdl = 0;

	if(opt_delta_item > 13 + 255)
	{
		optdl |= 0xE0;
		datalen += 2;
	}
	else if(opt_delta_item > 12)
	{
		optdl |= 0xD0;
		datalen += 1;
	}
	else
	{
		uint8_t temp = opt_delta_item;
		optdl |= (temp << 4 & 0xF0);
	}

	if(opt_len_item > 13 + 255)
	{
		optdl |= 0x0E;
		datalen += 2;
	}
	else if(opt_len_item > 12)
	{
		optdl |= 0x0D;
		datalen += 1;
	}
	else
	{
		uint8_t temp = opt_len_item;
		optdl |= temp & 0x0F;
	}

	datalen += _length;

	uint8_t *data = (uint8_t *)malloc(datalen);
	memset(data, 0, datalen);

	uint32_t offset = 0;

	data[offset++] = optdl;
	
	if(opt_delta_item > 13 + 255)
	{
		uint16_t delta_exp = opt_delta_item - (14 + 255);
		delta_exp = htons(delta_exp);
		memcpy(data + offset, &delta_exp, sizeof(delta_exp));
		offset += sizeof(delta_exp);
	}
	else if(opt_delta_item > 12)
	{
		uint8_t delta_exp = opt_delta_item - 13;
		memcpy(data + offset, &delta_exp, sizeof(delta_exp));
		offset += sizeof(delta_exp);
	}

	if(opt_len_item > 13 + 255)
	{
		uint16_t length_exp = opt_len_item - (14 + 255);
		length_exp = htons(length_exp);
		memcpy(data + offset, &length_exp, sizeof(length_exp));
		offset += sizeof(length_exp);
	}
	else if(opt_len_item > 12)
	{
		uint8_t length_exp = opt_len_item - 13;
		memcpy(data + offset, &length_exp, sizeof(length_exp));
		offset += sizeof(length_exp);
	}

	memcpy(data + offset, _value, opt_len_item);

	*_datalen = datalen;

	return data;

}

uint8_t* smart_coap_tool_context_encode(smart_coap_context_t *_ctx, uint32_t *_datalen)
{
	if(!_ctx)
	{
		*_datalen = 0;
		return NULL;
	}

	uint8_t *data;
	uint32_t offset = 0;
	uint32_t datasize = 0;
	uint32_t blocksize = 128;

	datasize += blocksize;
	data = (uint8_t *)malloc(datasize);

	uint8_t vtl;
	
	uint8_t ver = _ctx->ver;
	ver = (ver << 6) & 0xc0;

	uint8_t t = _ctx->t;
	t = (t << 4) & 0x30;

	uint8_t tkl = _ctx->tkl;
	tkl = tkl & 0x0F;

	vtl = ver | t | tkl;

	data[offset++] = vtl;
	data[offset++] = _ctx->code;

	uint16_t nmsgid = htons(_ctx->msgid);
	memcpy(data + offset, &nmsgid, sizeof(nmsgid));
	offset += sizeof(nmsgid);

	if(_ctx->tkl > 0)
	{
		memcpy(data + offset, _ctx->token, _ctx->tkl);
		offset += _ctx->tkl;
	}

	if(_ctx->option != 	NULL)
	{
		int i, j;

		uint32_t optcount = smart_coap_tool_option_count(_ctx);
		
		smart_coap_option_t* opts[optcount];
		smart_coap_option_t *item = _ctx->option;

		i = 0;
		while(item != NULL && i < optcount)
		{
			opts[i++] = item;
			item = item->next;
		}

		//按delta从小到大排序
		for(i = 0; i < optcount - 1; i++)
		{
			for(j = 0; j < optcount - 1 - i; j++)
			{
				if(opts[j]->delta > opts[j + 1]->delta)
				{
					item = opts[j];
					opts[j] = opts[j + 1];
					opts[j + 1] = item;
				}
			}
		}

		uint32_t opt_delta = 0;
		uint32_t opt_delta_item = 0;
		
		for(i = 0; i < optcount; i++)
		{
			opt_delta_item = 0;
			
			uint32_t bufflen;
			uint8_t *buff = NULL;

			if(opts[i]->delta < opt_delta)
			{
				opt_delta_item = opt_delta - opts[i]->delta;
				buff = smart_coap_tool_option_encode(opt_delta_item, 0, NULL, &bufflen);
				i--;
				opt_delta += opt_delta_item;
			}
			else
			{
				opt_delta_item = opts[i]->delta - opt_delta;
				buff = smart_coap_tool_option_encode(opt_delta_item, opts[i]->length, opts[i]->value, &bufflen);
				opt_delta = opts[i]->delta;
			}


			while(datasize < offset + bufflen)
			{
				datasize += blocksize;
				data = (uint8_t *)realloc(data, datasize);
			}

			memcpy(data + offset, buff, bufflen);
			offset += bufflen;

			if(buff != NULL)
			{
				free(buff);
				buff = NULL;
			}


		}

		/*
		if(_ctx->payload != NULL)
		{
			while(datasize < offset + _ctx->payloadlen + 1)
			{
				datasize += blocksize;
				data = (uint8_t *)realloc(data, datasize);
			}

			data[offset++] = 0xFF;

			memcpy(data + offset, _ctx->payload, _ctx->payloadlen);
			offset += _ctx->payloadlen;
		}
		*/
	
	}
	
	if(_ctx->payload != NULL)
	{
		while(datasize < offset + _ctx->payloadlen + 1)
		{
			datasize += blocksize;
			data = (uint8_t *)realloc(data, datasize);
		}

		data[offset++] = 0xFF;

		memcpy(data + offset, _ctx->payload, _ctx->payloadlen);
		offset += _ctx->payloadlen;
	}
	
	//smart_coap_tool_printbyte(data, offset);
	
	*_datalen = offset;
	return data;
}

void smart_coap_tool_context_free(smart_coap_context_t *_ctx)
{
	if(_ctx != NULL)
	{
		if(_ctx->option)
		{
			smart_coap_tool_option_free(_ctx->option);
			_ctx->option = NULL;
		}

		if(_ctx->payload)
		{
			free(_ctx->payload);
			_ctx->payload = NULL;
		}

		free(_ctx);
		_ctx = NULL;

	}

}

smart_coap_context_t* smart_coap_tool_context_creat(uint8_t _t, uint8_t _code, uint16_t _msgid)
{

	if(_t > 3)
	{
		return NULL;
	}

	smart_coap_context_t *ctx = (smart_coap_context_t *)malloc(sizeof(smart_coap_context_t));

	memset(ctx, 0, sizeof(smart_coap_context_t));

	ctx->ver = 1;
	ctx->t = _t;
	ctx->code = _code;
	ctx->msgid = _msgid;

	return ctx;
}

char smart_coap_tool_context_set_token(smart_coap_context_t *_ctx, uint8_t *_token, uint8_t _tokenlen)
{
	if(!_token || !_ctx)
	{
		return -1;
	}

	if(_tokenlen > 15)
	{
		return -1;
	}

	_ctx->tkl = _tokenlen;
	memcpy(_ctx->token, _token, _tokenlen);

	return 0;

}

char smart_coap_tool_context_add_opt(smart_coap_context_t *_ctx, uint32_t _delta, uint32_t _length, uint8_t *_value)
{
	if(!_ctx)
	{
		return -1;
	}

	if(_delta == SMART_COAP_CODE_OPT_URI_HOST 
	|| _delta == SMART_COAP_CODE_OPT_LOCATION_PATH
	|| _delta == SMART_COAP_CODE_OPT_URI_PATH 
	|| _delta == SMART_COAP_CODE_OPT_URI_QUERY
	|| _delta == SMART_COAP_CODE_OPT_LOCATION_QUERY
	|| _delta == SMART_COAP_CODE_OPT_PROXY_URI
	|| _delta == SMART_COAP_CODE_OPT_PROXY_SCHEME)
	{
		_length = strlen((char *)_value);
	}

	if(_delta == SMART_COAP_CODE_OPT_URI_PATH
	|| _delta == SMART_COAP_CODE_OPT_LOCATION_PATH)
	{
		while(_value[0] != '\0' && _value[0] == '/')
		{
			_value++;
		}

		_length = strlen((char *)_value);
	}

	if(_delta == SMART_COAP_CODE_OPT_URI_PORT
	|| _delta == SMART_COAP_CODE_OPT_CONTENT_FORMAT
	|| _delta == SMART_COAP_CODE_OPT_MAX_AGE
	|| _delta == SMART_COAP_CODE_OPT_ACCEPT
	|| _delta == SMART_COAP_CODE_OPT_SIZEL)
	{
		if(_length == 2)
		{
			uint16_t *value = (uint16_t *)_value;
			*value = htons(*value);
		}
		else if(_length == 4)
		{
			uint32_t *value = (uint32_t *)_value;
			*value = htons(*value);
			
		}
		
	}

	smart_coap_option_t *opt = (smart_coap_option_t *)malloc(sizeof(smart_coap_option_t));
	memset(opt, 0, sizeof(smart_coap_option_t));

	opt->delta = _delta;
	opt->length = _length;
	if(opt->length > 0)
	{
		opt->value = (uint8_t *)malloc(opt->length);
		memcpy(opt->value, _value, opt->length);
	}

	if(_ctx->option == NULL)
	{
		_ctx->option = opt;
	}
	else
	{
		smart_coap_option_t *last = _ctx->option;
		
		while(last->next != NULL)
		{
			last = last->next;
		}

		last->next = opt;
	
	}

	return 0;

}

char smart_coap_tool_context_del_opt(smart_coap_context_t *_ctx, uint32_t _delta, uint8_t _index)
{
	if(!_ctx)
	{
		return -1;
	}

	smart_coap_option_t *last = NULL;
	smart_coap_option_t *item = _ctx->option;

	uint8_t index = 0;

	while(item != NULL)
	{
		if(item->delta == _delta)
		{
			if(index == _index)
			{
				if(last == NULL)
				{
					_ctx->option = item->next;
				}
				else
				{
					last->next = item->next;
				}

				smart_coap_tool_option_free(item);

				break;
			}
		}

		last = item;
		item = item->next;
	}

	return 0;
}

void* smart_coap_tool_msg_get_opt(smart_coap_msg_t *_msg, uint32_t _delta, uint8_t _index, uint32_t *_len)
{
	if(!_msg)
	{
		return NULL;
	}

	if(_msg->ctx == NULL)
	{
		return NULL;
	}

	uint32_t length = 0;
	uint8_t *value = smart_coap_tool_context_get_opt(_msg->ctx, _delta, _index, &length);

	if(value == NULL)
	{
		*_len = 0;
		return NULL;
	}

	if(_delta == SMART_COAP_CODE_OPT_URI_HOST 
	|| _delta == SMART_COAP_CODE_OPT_LOCATION_PATH
	|| _delta == SMART_COAP_CODE_OPT_URI_PATH 
	|| _delta == SMART_COAP_CODE_OPT_URI_QUERY
	|| _delta == SMART_COAP_CODE_OPT_LOCATION_QUERY
	|| _delta == SMART_COAP_CODE_OPT_PROXY_URI
	|| _delta == SMART_COAP_CODE_OPT_PROXY_SCHEME)
	{
		*_len = length;
		return (char *)value;
	}

	if(_delta == SMART_COAP_CODE_OPT_URI_PORT
	|| _delta == SMART_COAP_CODE_OPT_CONTENT_FORMAT
	|| _delta == SMART_COAP_CODE_OPT_MAX_AGE
	|| _delta == SMART_COAP_CODE_OPT_ACCEPT
	|| _delta == SMART_COAP_CODE_OPT_SIZEL)
	{
		uint32_t *numvalue = (uint32_t *)malloc(sizeof(uint32_t));
		memset(numvalue, 0, sizeof(uint32_t));
		int numoffset = sizeof(*numvalue) - length;

		if(numoffset < 0)
		{
			*_len = 0;
		}
		else if(numoffset == sizeof(numvalue))
		{
			*_len = 0;
		}
		else
		{
			*_len = sizeof(numvalue);
			memcpy(numvalue + numoffset, value, length);
		}

		*numvalue = ntohs(*numvalue);
		return numvalue;
	}

	*_len = length;
	return value;
}

uint8_t* smart_coap_tool_context_get_opt(smart_coap_context_t *_ctx, uint32_t _delta, uint8_t _index, uint32_t *_length)
{
	if(!_ctx)
	{
		return NULL;
	}

	uint8_t *value = NULL;
	smart_coap_option_t *item = _ctx->option;

	uint8_t index = 0;

	while(item != NULL)
	{
		if(item->delta == _delta)
		{
			if(index == _index)
			{
				if(item->value != NULL)
				{

					if(item->delta == SMART_COAP_CODE_OPT_SIZEL
					|| item->delta == SMART_COAP_CODE_OPT_URI_PORT
					|| item->delta == SMART_COAP_CODE_OPT_MAX_AGE
					|| item->delta == SMART_COAP_CODE_OPT_ACCEPT
					|| item->delta == SMART_COAP_CODE_OPT_CONTENT_FORMAT)
					{
						uint32_t number = 0;
						memcpy(&number, item->value, item->length);
						number = ntohs(number);
						
						value = (uint8_t *)malloc(sizeof(uint32_t));
						memcpy(value, &number, sizeof(uint32_t));
						*_length = sizeof(uint32_t);
					}
					else if(item->delta == SMART_COAP_CODE_OPT_URI_HOST
					|| item->delta == SMART_COAP_CODE_OPT_URI_PATH
					|| item->delta == SMART_COAP_CODE_OPT_LOCATION_PATH
					|| item->delta == SMART_COAP_CODE_OPT_URI_QUERY
					|| item->delta == SMART_COAP_CODE_OPT_LOCATION_QUERY
					|| item->delta == SMART_COAP_CODE_OPT_PROXY_URI
					|| item->delta == SMART_COAP_CODE_OPT_PROXY_SCHEME)
					{
						value = (uint8_t *)malloc(item->length + 1);
						memset(value, 0, item->length + 1);
						memcpy(value, item->value, item->length);
						*_length = item->length + 1;
					}
					else
					{
						value = (uint8_t *)malloc(item->length);
						memcpy(value, item->value, item->length);
						*_length = item->length;
					}
				}


				break;
			}
		}

		item = item->next;
	}

	return value;
}

char smart_coap_tool_context_set_payload(smart_coap_context_t *_ctx, uint8_t *_value, uint32_t _len)
{
	if(!_ctx)
	{
		return -1;
	}

	if(_ctx->payload != NULL)
	{
		free(_ctx->payload);
		_ctx->payload = NULL;
	}

	_ctx->payloadlen = _len;
	_ctx->payload = (uint8_t *)malloc(_ctx->payloadlen);
	memcpy(_ctx->payload, _value, _ctx->payloadlen);

	return 0;
}

char* smart_coap_tool_get_uri(smart_coap_context_t *_ctx)
{
	if(!_ctx)
	{
		return NULL;
	}

	char *uri = NULL;
	uint32_t blocksize = 64;
	uint32_t offset = 0;
	uint32_t urilen = 0;
	
	urilen += blocksize;
	uri = (char *)malloc(urilen);
	memset(uri, 0, urilen);

	smart_coap_option_t *item = _ctx->option;

	while(item != NULL)
	{
		if(item->delta == SMART_COAP_CODE_OPT_URI_PATH)
		{
			if(item->length != 0)
			{
				if((item->value)[0] != '/')
				{
					while(urilen - offset < 1)
					{
						urilen += blocksize;
						uri = (char *)realloc(uri, urilen);
						memset(uri + offset, 0, urilen - offset);
					}

					uri[offset++] = '/';
				}

				while(urilen - offset < strlen((char *)item->value))
				{
					urilen += blocksize;
					uri = (char *)realloc(uri, urilen);
					memset(uri + offset, 0, urilen - offset);
				}

				memcpy(uri + offset, item->value, strlen((char *)item->value));
				offset += strlen((char *)item->value);
			}
		}
		
		item = item->next;

	}

	int i;

	for(i = 0; i < strlen(uri); i++)
	{
		if(uri[i] == '\\')
		{
			uri[i] = '/';
		}
	}

	return uri;

}

char* smart_coap_tool_get_query(smart_coap_context_t *_ctx)
{
	if(!_ctx)
	{
		return NULL;
	}

	char *query;
	int offset = 0;
	int datablock = 32;
	int datasize = 0;

	datasize += datablock;
	query = (char *)malloc(datasize);
	memset(query + offset, 0, datasize - offset);

	
	smart_coap_option_t *item = _ctx->option;

	while(item != NULL)
	{
		if(item->delta == SMART_COAP_CODE_OPT_URI_QUERY)
		{
			if(item->length != 0)
			{
				char *val = NULL;

				if(offset > 0 && (item->value)[0] != '&' && query[offset - 1] != '&')
				{
					while(datasize - offset < 1)
					{
						datasize += datablock;
						query = (char *)realloc(query, datasize);
						memset(query + offset, 0, datasize - offset);
					}

					query[offset++] = '&';
				}

				if(offset == 0 && (item->value)[0] == '&')
				{
					val = (char *)item->value + 1;
				}
				else
				{
					val = (char *)item->value;
				}

				while(datasize - offset < strlen(val))
				{
					datasize += datablock;
					query = (char *)realloc(query, datasize);
					memset(query + offset, 0, datasize - offset);
				}

				memcpy(query + offset, item->value, strlen(val));
				offset += strlen(val);
			}
		}
		
		item = item->next;

	}

	return query;
}

smart_coap_msg_t* smart_coap_tool_msg_restful_get(smart_coap_context_t *_ctx, char *_url)
{
	if(!_ctx)
	{
		return NULL;
	}
	
	smart_coap_msg_query_t *queryhead = NULL;

	char *_uri = smart_coap_tool_get_uri(_ctx);
	char *uri = _uri;

	if(uri != NULL && _url != NULL)
	{

		char *urlcp = (char *)malloc(strlen(_url) + 1);
		memset(urlcp, 0, strlen(_url) + 1);
		memcpy(urlcp, _url, strlen(_url));

		char *url = urlcp;
		
		while(uri[0] != '\0' && uri[0] == '/')
		{
			uri++;
		}

		while(strlen(uri) > 0 && uri[strlen(uri) - 1] == '/')
		{
			uri[strlen(uri) - 1] = '\0';
		}

		int i;

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
					if(urls[i][0] == ':')
					{

						if(queryhead == NULL)
						{
							queryhead = (smart_coap_msg_query_t *)malloc(sizeof(smart_coap_msg_query_t));
							memset(queryhead, 0, sizeof(smart_coap_msg_query_t));
							queryhead->name = (char *)malloc(strlen(urls[i] + 1) + 1);
							memset(queryhead->name, 0, strlen(urls[i] + 1) + 1);
							memcpy(queryhead->name, urls[i] + 1, strlen(urls[i] + 1));

							queryhead->value = (char *)malloc(strlen(uris[i]) + 1);
							memset(queryhead->value, 0, strlen(uris[i]) + 1);
							memcpy(queryhead->value, uris[i], strlen(uris[i]));
						}
						else
						{

							smart_coap_msg_query_t *item = (smart_coap_msg_query_t *)malloc(sizeof(smart_coap_msg_query_t));
							memset(item, 0, sizeof(smart_coap_msg_query_t));
							item->name = (char *)malloc(strlen(urls[i] + 1) + 1);
							memset(item->name, 0, strlen(urls[i] + 1) + 1);
							memcpy(item->name, urls[i] + 1, strlen(urls[i] + 1));

							item->value = (char *)malloc(strlen(uris[i]) + 1);
							memset(item->value, 0, strlen(uris[i]) + 1);
							memcpy(item->value, uris[i], strlen(uris[i]));

							item->next = queryhead;
							queryhead = item;
						}
					}
					else
					{
						smart_coap_tool_msg_query_free(queryhead);
						break;
					}

				}



			}

		}

		if(urlcp != NULL)
		{
			free(urlcp);
			urlcp = NULL;
		}
		
		if(_uri != NULL)
		{
			free(_uri);
			_uri = NULL;
		}
	}


	//获取标准参数

	char *querycp = smart_coap_tool_get_query(_ctx);

	if(querycp != NULL)
	{
		char *query = querycp;

		while(query[0] != '\0' && (query[0] == '?' || query[0] == '&' || query[0] == '='))
		{
			query++;
		}


		while(strlen(query) > 0 && (query[strlen(query) - 1] == '&' || query[strlen(query) - 1] == '?' || query[strlen(query) - 1] == '='))
		{
			query[strlen(query) - 1] = '\0';
		}

		char *name = NULL;
		char *value = NULL;

		int i;
		int querylen = strlen(query);

		for(i = 0; i <= querylen; i++)
		{

			if(i == 0)
			{
				name = query + i;
			}
			else if(query[i] == '=' || query[i] == '&')
			{
				query[i] = '\0';
			}
			else
			{
				if(query[i - 1] == '\0')
				{
					if(name == NULL)
					{
						name = query + i;
					}
					else
					{
						value = query + i;
					}
				}
			}

			if(name != NULL && value != NULL && query[i] == '\0')
			{
				
				if(queryhead == NULL)
				{
					queryhead = (smart_coap_msg_query_t *)malloc(sizeof(smart_coap_msg_query_t));
					memset(queryhead, 0, sizeof(smart_coap_msg_query_t));
					queryhead->name = (char *)malloc(strlen(name) + 1);
					memset(queryhead->name, 0, strlen(name) + 1);
					memcpy(queryhead->name, name, strlen(name));

					queryhead->value = (char *)malloc(strlen(value) + 1);
					memset(queryhead->value, 0, strlen(value) + 1);
					memcpy(queryhead->value, value, strlen(value));
						
				}
				else
				{

					smart_coap_msg_query_t *item = (smart_coap_msg_query_t *)malloc(sizeof(smart_coap_msg_query_t));
					memset(item, 0, sizeof(smart_coap_msg_query_t));
					item->name = (char *)malloc(strlen(name) + 1);
					memset(item->name, 0, strlen(name) + 1);
					memcpy(item->name, name, strlen(name));

					item->value = (char *)malloc(strlen(value) + 1);
					memset(item->value, 0, strlen(value) + 1);
					memcpy(item->value, value, strlen(value));

					item->next = queryhead;
					queryhead = item;
				}
				
				name = NULL;
				value = NULL;
			}
		}

		free(querycp);
		querycp = NULL;
	}

/*
	smart_coap_msg_query_t *item = queryhead;

	while(item != NULL)
	{
		printf("queryhead name:%s, value:%s\n", item->name, item->value);
		item = item->next;
	}
*/

	smart_coap_msg_t *msg = (smart_coap_msg_t *)malloc(sizeof(smart_coap_msg_t));
	memset(msg, 0, sizeof(smart_coap_msg_t));

	msg->ctx = _ctx;
	msg->query = queryhead;
	
	if(_url)
	{
		msg->callurl = (char *)malloc(strlen(_url) + 1);
		memset(msg->callurl, 0, strlen(_url) + 1);
		memcpy(msg->callurl, _url, strlen(_url));
	}	
	return msg;
}

char* smart_coap_tool_msg_get_query(smart_coap_msg_t *_msg, char *_name, int _index)
{
	if(!_msg)
	{
		return NULL;
	}

	char *value = NULL;
	int index = 0;
	smart_coap_msg_query_t *item = _msg->query;

	while(item != NULL)
	{

		if(strcmp(item->name, _name) == 0)
		{
			if(index == _index)
			{
				break;
			}
			else
			{
				index++;
			}
		}

		item = item->next;

	}

	if(item != NULL)
	{
		value = (char *)malloc(strlen(item->value) + 1);
		memset(value, 0, strlen(item->value) + 1);
		memcpy(value, item->value, strlen(item->value));
	}

	return value;
}

uint8_t* smart_coap_tool_msg_get_payload(smart_coap_msg_t *_msg, uint8_t *_length)
{
	if(!_msg)
	{
		return NULL;
	}

	if(_msg->ctx == NULL)
	{
		return NULL;
	}

	uint8_t *payload = NULL;
	uint32_t length = 0;

	if(_msg->ctx->payload != NULL)
	{
		length = _msg->ctx->payloadlen;
		payload = (uint8_t *)malloc(length);
		memset(payload, 0, length);
		memcpy(payload, _msg->ctx->payload, length);
	}
	
	*_length = length;
	return payload;
}

void smart_coap_tool_msg_query_free(smart_coap_msg_query_t *_query)
{

	if(_query != NULL)
	{
		smart_coap_tool_msg_query_free(_query->next);
		
		if(_query->value != NULL)
		{
			free(_query->value);
			_query->value = NULL;
		}

		if(_query->name != NULL)
		{
			free(_query->name);
			_query->name = NULL;
		}

	}
}

void smart_coap_tool_msg_free(smart_coap_msg_t *_msg)
{

	if(_msg != NULL)
	{
		if(_msg->query != NULL)
		{
			smart_coap_tool_msg_query_free(_msg->query);
			_msg->query = NULL;
		}

		if(_msg->ctx != NULL)
		{
			smart_coap_tool_context_free(_msg->ctx);
			_msg->ctx = NULL;
		}

		if(_msg->callurl != NULL)
		{
			free(_msg->callurl);
			_msg->callurl = NULL;
		}

		free(_msg);
		_msg = NULL;

	}
}

