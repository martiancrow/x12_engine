/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 python 回调工具
 *
 *
 *作者:Crow
 *时间:2017-06_03
 *最后修改时间:2017-06-03
 */

#ifndef _X12_PYTHON_CALL_H_INCLUDE
#define _X12_PYTHON_CALL_H_INCLUDE

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

typedef struct x12_py_plugin_item_s x12_py_plugin_item_t;
typedef struct x12_py_plugin_list_s x12_py_plugin_list_t;

struct x12_py_plugin_item_s
{
	char path[256];
	char module[128];
	char func[128];
	x12_py_plugin_item_t *next;
};

struct x12_py_plugin_list_s
{
	x12_py_plugin_item_t *head;
};

/*
 * Function: pyeval_ret
 *
 * 执行函数返回
 *
 * Parameters:
 * 	_evalret	-	返回值
 *
 *
 * See Also:
 *
 */
typedef void (*pyeval_ret)(PyObject *_evalret);

/*
 * Function: x12_python_call_init
 *
 * 初始化python环境变量
 *
 * Parameters:
 * 	_rootpath	-	脚本路径
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_python_call_init(char *_rootpath);

/*
 * Function: x12_python_call_reslease
 *
 * 释放python环境
 *
 * See Also:
 *
 */
extern void x12_python_call_reslease(void);

/*
 * Function: x12_python_call_eval
 *
 * 回调一个python函数
 *
 * Parameters:
 *  _rootpath	-	文件路径
 * 	_module		-	模块名称
 * 	_function	-	函数名称
 * 	_func		-	返回回调函数
 * 	_format		-	格式化参数
 *
 * Returns:
 *	成功	-	返回执行结果
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int x12_python_call_eval(const char *_rootpath, const char *_module, const char *_function, pyeval_ret _func, const char *_format, ...);

/*
 * Function: x12_python_call_plugin_luacfg_do
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
extern int x12_python_call_plugin_luacfg_do(lua_State *_sl);

#endif
