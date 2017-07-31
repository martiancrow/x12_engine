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

#ifndef _X12_LUA_API_KIT_H_INCLUDE
#define _X12_LUA_API_KIT_H_INCLUDE

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
#include "x12_coap_kit.h"
#include "x12_msg.h"
#include "x12_lua_coap_kit.h"

/*
 * Function: x12_lua_coap_api_load
 *
 * 载入coap api
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
extern int x12_lua_coap_api_load(lua_State *_sl);


#endif
