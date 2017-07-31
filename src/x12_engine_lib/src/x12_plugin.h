/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 插件相关操作
 *
 *
 *作者:Crow
 *时间:2017-06-06
 *最后修改时间:2017-06-06
 */

#ifndef _X12_PLUGIN_H_INCLUDE
#define _X12_PLUGIN_H_INCLUDE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <dlfcn.h>

/*
 * Function: plugin_load_func
 *
 * 插件载入函数原型
 *
 * Parameters:
 * 	_data	-	传入数据
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
typedef uint16_t (*plugin_load_func)(void* _data);

/*
 * Function: x12_plugin_init
 *
 * 初始化插件
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
extern void x12_plugin_init(void);

extern int x12_plugin_load(const char *_path, const char *_loadname, void* _data);

#endif
