/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 lua  配置模块
 *
 *
 *作者:刘晓龙
 *时间:2017-07-03
 *最后修改时间:2017-07-03
 */

#ifndef _X12_CFG_LUA_H_INCLUDE
#define _X12_CFG_LUA_H_INCLUDE

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
#include "x12_lua_coap_kit.h"
#include "x12_python_coap_kit.h"
#include "x12_python_call.h"

/*
 * Function: x12_cfg_lua_init
 *
 * 初始化配置文件
 *
 * Parameters:
 * 	_file		-	配置文件
 * 	_cfgname	-	入口变量地址
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_cfg_lua_init(const char *_file, const char *_cfgname);

#endif
