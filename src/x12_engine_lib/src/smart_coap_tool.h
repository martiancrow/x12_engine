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

#ifndef _SMART_COAP_TOOL_H_INCLUDE
#define _SMART_COAP_TOOL_H_INCLUDE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "smart_coap_code.h"

typedef struct smart_coap_option_s smart_coap_option_t;
typedef struct smart_coap_context_s smart_coap_context_t;
typedef struct smart_coap_msg_s smart_coap_msg_t;
typedef struct smart_coap_msg_query_s smart_coap_msg_query_t;

struct smart_coap_context_s
{
	uint8_t ver:2;		//版本号
	uint8_t t:2;		//报文类型
	uint8_t tkl:4;		//标识符长度

	uint8_t code;		//code 编号3.5格式
	
	uint16_t msgid;		//message id
	uint8_t token[15];	//标识符
	
	smart_coap_option_t *option;	//coap操作参数

	uint8_t *payload;		//负载数据
	uint32_t payloadlen;	//负载数据长度

};

struct smart_coap_option_s
{
	uint32_t delta;
	uint32_t length;
	uint8_t *value;
	smart_coap_option_t *next;
};

struct smart_coap_msg_s
{
	char *callurl;						//原始访问url
	smart_coap_msg_query_t *query;
	smart_coap_context_t *ctx;
};

struct smart_coap_msg_query_s
{
	char *name;
	char *value;
	smart_coap_msg_query_t *next;
};

extern void smart_coap_tool_printbyte(unsigned char *_buff, int _bufflen);

/*
 * Function: smart_coap_tool_data_check
 *
 * 检查一段数据格式是否正确
 * 
 * Parameters:
 * 	_buff		-	数据
 * 	_bufflen	-	数据长度
 *
 * Returns:
 * 	-1	-	内部错误
 *	0	-	正常
 *	1	-	数据不完整
 *	2	-	数据错误
 *
 * See Also:
 *
 */
extern int smart_coap_tool_data_check(uint8_t *_buff, uint32_t _bufflen);

/*
 * Function: smart_coap_tool_context_get
 *
 * 通过二进制文件获取上下文
 *
 * Parameters:
 * 	_buff		-	二进制数据
 * 	_bufflen	-	数据长度
 *
 * Returns:
 *	成功	-	上下文
 *	失败	-	null
 *
 * See Also:
 *
 */
extern smart_coap_context_t* smart_coap_tool_context_get(uint8_t *_buff, uint32_t _bufflen);

/*
 * Function: smart_coap_tool_context_encode
 *
 * 序列化上下文
 *
 * Parameters:
 * 	_ctx		-	上下文
 * 	_datalen	-	数据长度
 *
 * Returns:
 *	成功	-	数据
 *	失败	-	null
 *
 * See Also:
 *
 */
extern uint8_t* smart_coap_tool_context_encode(smart_coap_context_t *_ctx, uint32_t *_datalen);

/*
 * Function: smart_coap_tool_context_free
 *
 * 释放上下文
 *
 * Parameters:
 * 	_buff		-	二进制数据
 * 	_bufflen	-	数据长度
 *
 * Returns:
 *	成功	-	上下文
 *	失败	-	null
 *
 * See Also:
 *
 */
extern void smart_coap_tool_context_free(smart_coap_context_t *_ctx);

/*
 * Function: smart_coap_tool_context_creat
 *
 * 创建一个上下文
 * 
 * Parameters:
 *	_t		-	报文类型
 *	_code	-	操作码
 *	_msgid	-	消息id
 *
 * Returns:
 *	成功	-	上下文
 *	失败	-	null
 *
 * See Also:
 *
 */
extern smart_coap_context_t* smart_coap_tool_context_creat(uint8_t _t, uint8_t _code, uint16_t _msgid);

/*
 * Function: smart_coap_tool_context_set_token
 *
 * 设置上下文token
 * 
 * Parameters:
 * 	_ctx		-	上下文
 *	_token		-	token
 *	_tokenlen	-	token长度
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern char smart_coap_tool_context_set_token(smart_coap_context_t *_ctx, uint8_t *_token, uint8_t _tokenlen);

/*
 * Function: smart_coap_tool_context_add_opt
 *
 * 添加上下文option
 * 
 * Parameters:
 * 	_ctx	-	上下文
 *	_delta	-	编号
 *	_length	-	长度
 *	_value	-	值
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern char smart_coap_tool_context_add_opt(smart_coap_context_t *_ctx, uint32_t _delta, uint32_t _length, uint8_t *_value);

/*
 * Function: smart_coap_tool_context_del_opt
 *
 * 添加上下文option
 * 
 * Parameters:
 * 	_ctx	-	上下文
 *	_delta	-	编号
 *	_index	-	值
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern char smart_coap_tool_context_del_opt(smart_coap_context_t *_ctx, uint32_t _delta, uint8_t _index);

/*
 * Function: smart_coap_tool_context_get_opt
 *
 * 查询option
 * 
 * Parameters:
 * 	_ctx	-	上下文
 *	_delta	-	编号
 *	_index	-	排序
 *	_length	-	值长度
 *
 * Returns:
 *	成功	-	值内容
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern uint8_t* smart_coap_tool_context_get_opt(smart_coap_context_t *_ctx, uint32_t _delta, uint8_t _index, uint32_t *_length);

/*
 * Function: smart_coap_tool_context_set_payload
 *
 * 设置上下文payload
 * 
 * Parameters:
 * 	_ctx	-	上下文
 *	_value	-	值
 *	_len	-	长度
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern char smart_coap_tool_context_set_payload(smart_coap_context_t *_ctx, uint8_t *_value, uint32_t _len);

/*
 * Function: smart_coap_tool_get_uri
 *
 * 获取上下文中的uri
 * 
 * Parameters:
 * 	_ctx	-	上下文
 *
 * Returns:
 *	成功	-	uri字符串
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern char* smart_coap_tool_get_uri(smart_coap_context_t *_ctx);

/*
 * Function: smart_coap_tool_get_query
 *
 * 获取上下文中的query
 * 
 * Parameters:
 * 	_ctx	-	上下文
 *
 * Returns:
 *	成功	-	query字符串
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern char* smart_coap_tool_get_query(smart_coap_context_t *_ctx);

/*
 * Function: smart_coap_tool_msg_restful_get
 *
 * 获取一个消息请求
 * 
 * Parameters:
 * 	_ctx	-	上下文
 * 	_url	-	restful风格url
 *
 * Returns:
 *	成功	-	msg
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern smart_coap_msg_t* smart_coap_tool_msg_restful_get(smart_coap_context_t *_ctx, char *_url);

/*
 * Function: smart_coap_tool_msg_get_query
 *
 * 释放参数链
 * 
 * Parameters:
 * 	_msg	-	消息
 * 	_name	-	消息键
 * 	_index	-	序列
 *
 * See Also:
 *
 */
extern char* smart_coap_tool_msg_get_query(smart_coap_msg_t *_msg, char *_name, int _index);

/*
 * Function: smart_coap_tool_msg_get_payload
 *
 * 获取payload
 * 
 * Parameters:
 * 	_msg	-	消息
 * 	_length	-	负载长度
 *
 * Returns:
 *	成功	-	负载拷贝
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern uint8_t* smart_coap_tool_msg_get_payload(smart_coap_msg_t *_msg, uint8_t *_length);

/*
 * Function: smart_coap_tool_msg_query_free
 *
 * 释放参数链
 * 
 * Parameters:
 * 	_query	-	参数链
 *
 * See Also:
 *
 */
extern void smart_coap_tool_msg_query_free(smart_coap_msg_query_t *_query);

/*
 * Function: smart_coap_tool_msg_free
 *
 * 释放参数链
 * 
 * Parameters:
 * 	_msg	-	消息
 *
 * See Also:
 *
 */
extern void smart_coap_tool_msg_free(smart_coap_msg_t *_msg);

extern char* sct_ctx_getcodename(uint32_t _codeno);

extern char* sct_ctx_getfmtname(uint32_t _fmt);

extern char* sct_ctx_getoptname(uint32_t _delta);

extern uint32_t sct_ctx_getcodeno(const char *_name);

extern uint32_t sct_ctx_getfmtno(const char *_name);

extern uint32_t sct_ctx_getoptdelta(const char *_name);

extern void smart_coap_tool_option_free(smart_coap_option_t *_item);

#endif
