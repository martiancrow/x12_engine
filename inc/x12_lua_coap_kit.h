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

#ifndef _X12_LUA_COAP_KIT_H_INCLUDE
#define _X12_LUA_COAP_KIT_H_INCLUDE

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
#include "x12_lua_coap_api.h"

typedef struct x12_lua_coap_route_item_s x12_lua_coap_route_item_t;
typedef struct x12_lua_coap_route_list_s x12_lua_coap_route_list_t;

struct x12_lua_coap_route_item_s
{
	char path[256];
	char func[64];
	char uri[256];
	x12_lua_coap_route_item_t *next;
};

struct x12_lua_coap_route_list_s
{
	char mcbpath[256];
	char mcbfunc[64];
	x12_lua_coap_route_item_t *head;
};

typedef struct x12_lua_coap_cfg_route_s
{
	char uri[256];
	char path[256];
	char func[64];
} x12_lua_coap_cfg_route_t;

/*
 * Function: x12_lua_coap_kit_do
 *
 * 安配置创建一个服务
 *
 * Parameters:
 * 	_sl		-	服务堆栈	
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_lua_coap_kit_do(lua_State *_sl);

extern int x12_lua_coap_kit_rl_creat(int _drvid);

extern int x12_lua_coap_kit_rl_creat(int _drvid);
#endif
