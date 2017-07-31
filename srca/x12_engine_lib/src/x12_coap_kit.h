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

#ifndef _X12_COAP_KIT_H_INCLUDE
#define _X12_COAP_KIT_H_INCLUDE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "smart_coap_code.h"
#include "smart_coap_tool.h"
#include "x12_even_queue.h"
#include "x12_even_drivers.h"
#include "x12_socket_udp.h"
#include "x12_socket_tcp.h"

/*
 * Function: x12_coap_route_callback_fp
 *
 * 路由回调函数
 *
 * Parameters:
 * 	_senderid		-	发送者在x12中的设备id
 * 	_msg			-	发送的数据
 *
 * Returns:
 *	目前暂不做判断
 *
 * See Also:
 *
 */
typedef int (*x12_coap_route_callback_fp)(int _senderid, smart_coap_msg_t *_msg);

/*
 * Function: x12_coap_kitud_free
 *
 * 路由回调函数
 *
 * Parameters:
 * 	_ud		-	用户数据
 *
 * See Also:
 *
 */
typedef void (*x12_coap_kitud_free)(void* _ud);

typedef struct x12_coap_route_s x12_coap_route_t;
typedef struct x12_coap_route_table_s x12_coap_route_table_t;
typedef struct x12_coap_kit_list_s x12_coap_kit_list_t;
typedef struct x12_coap_kit_item_s x12_coap_kit_item_t;

struct x12_coap_route_s
{
	x12_coap_route_callback_fp callback;
	x12_coap_route_t *next;
	char url[];
};

struct x12_coap_route_table_s
{
	uint32_t count;
	x12_coap_route_t *route;
	x12_coap_route_callback_fp msgcallback;
};

struct x12_coap_kit_item_s
{
	int connfd;
	x12_coap_route_table_t *routetable;
	x12_coap_kit_item_t *next;
	void* udata;
	x12_coap_kitud_free freefunc;
};

struct x12_coap_kit_list_s
{
	x12_coap_kit_item_t* head;
};

/*
 * Function: x12_coap_kit_init
 *
 * 初始化一个coap工具链
 *
 * Returns:
 *	成功	-	工具包
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern void x12_coap_kit_init(void);

/*
 * Function: x12_coap_kit_creat
 *
 * 添加一个coap工具包
 *
 * Parameters:
 * 	_drvid	-	设备id
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_coap_kit_add(int _drvid);

/*
 * Function: x12_coap_kit_route_add
 *
 * 添加一个路由项
 *
 * Parameters:
 *  _drvid		-	设备id
 * 	_url		-	地址
 * 	_callback	-	回调
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_coap_kit_route_add(int _drvid, const char *_url, void* _callback);

/*
 * Function: x12_coap_kit_route_set_msgcallback
 *
 * 添加一个路由项
 *
 * Parameters:
 * 	_drvid		-	设备id
 * 	_callback	-	回调
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_coap_kit_set_msgcallback(int _drvid, void* _callback);

/*
 * Function: x12_coap_kit_even_callback
 *
 * 底层事件回调函数
 *
 * Parameters:
 * 	_queue		-	x12消息队列
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern char x12_coap_kit_even_callback(void* _queue);

/*
 * Function: x12_coap_kit_destroy
 *
 * 销毁coap工具链
 *
 * See Also:
 *
 */
extern void x12_coap_kit_destroy(void);

/*
 * Function: x12_coap_kit_even_callback
 *
 * 底层事件回调函数
 *
 * Parameters:
 * 	_drvid		-	设备编号
 *
 * Returns:
 *	成功	-	工具包
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern x12_coap_kit_item_t* x12_coap_kit_item_get(int _drvid);

/*
 * Function: x12_coap_kit_udata_add
 *
 * 底层事件回调函数
 *
 * Parameters:
 * 	_drvid		-	设备id
 * 	_udata		-	用户数据
 * 	_func		-	释放函数
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_coap_kit_udata_add(int _drvid, void* _udata, x12_coap_kitud_free _func);

#endif
