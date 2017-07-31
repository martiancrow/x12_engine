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

#ifndef _X12_PYTHON_COAP_KIT_H_INCLUDE
#define _X12_PYTHON_COAP_KIT_H_INCLUDE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <python2.7/Python.h>
#include <dirent.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "smart_coap_tool.h"
#include "x12_even_queue.h"
#include "x12_even_drivers.h"
#include "x12_socket_udp.h"
#include "x12_select.h"
#include "x12_coap_kit.h"
#include "x12_python_call.h"

typedef struct x12_py_coap_route_item_s x12_py_coap_route_item_t;
typedef struct x12_py_coap_route_list_s x12_py_coap_route_list_t;

struct x12_py_coap_route_item_s
{
	char path[256];
	char module[64];
	char func[64];
	char uri[256];
	x12_py_coap_route_item_t *next;
};

struct x12_py_coap_route_list_s
{
	char mcbpath[256];
	char mcbmodule[64];
	char mcbfunc[64];
	x12_py_coap_route_item_t *head;
};

/*
 * Function: x12_py_coap_kit_rl_creat
 *
 * 创建一个路由链表
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
extern int x12_py_coap_kit_rl_creat(int _drvid);

/*
 * Function: x12_py_coap_kit_ri_add
 *
 * 添加路由
 *
 * Parameters:
 * 	_drvid	-	设备id
 * 	_uri	-	路由地址
 * 	_path	-	脚本路径
 * 	_mod	-	模块名称
 * 	_func	-	函数名称
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_py_coap_kit_ri_add(int _drvid, char *_uri, char *_path, char *_mod, char *_func);

/*
 * Function: x12_py_coap_kit_set_msgcb
 *
 * 设置消息回调函数
 *
 * Parameters:
 * 	_drvid	-	设备id
 * 	_path	-	脚本路径
 * 	_mod	-	模块名称
 * 	_func	-	函数名称
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_py_coap_kit_set_msgcb(int _drvid, char *_path, char *_mod, char *_func);

/*
 * Function: x12_py_coap_kit_udp_listen
 *
 * 监听一个地址
 *
 * Parameters:
 * 	_addr	-	ip地址
 * 	_port	-	端口
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_py_coap_kit_udp_listen(const char *_addr, int _port);

/*
 * Function: x12_py_coap_kit_udp_conn
 *
 * 链接一个地址
 *
 * Parameters:
 * 	_addr	-	ip地址
 * 	_port	-	端口
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_py_coap_kit_udp_conn(const char *_addr, int _port);

/*
 * Function: x12_py_coap_kit_rl_free
 *
 * 释放路由链表
 *
 * Parameters:
 * 	_rl	-	路由链表
 *
 * See Also:
 *
 */
extern void x12_py_coap_kit_rl_free(x12_py_coap_route_list_t *_rl);


#endif
