/*
 *
 * Copyright (C) 刘晓龙
 * Copyright (C) 昆明黑海科技有限公司
 *
 */


/*
 * 
 * 原始套接字操作
 *
 * 作者:Crow
 * 时间:2017-03-22
 * 最后修改时间:2017-03-22
 *      
 */

#ifdef _OS_LINUX_

#ifndef _X12_SOCKET_RAW_H_INCLUDE_
#define _X12_SOCKET_RAW_H_INCLUDE_

#include <fcntl.h>

#include "driver_nl80211.h"

typedef int x12_socket_raw_size_t;
typedef struct x12_socket_raw_s x12_socket_raw_t;

struct x12_socket_raw_s
{
	int fd;
	struct sockaddr_ll addr;	//监听设备地址
	unsigned int addrlen;		//地址长度
	unsigned char type;			//类型
};

/*
 * Function: x12_socket_raw_creat
 *
 * 创建一个空的原始套接字
 *
 * Returns:
 *	成功	-	返回地址族
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern x12_socket_raw_t* x12_socket_raw_creat(void);

/*
 * Function: x12_socket_raw_creat_monitor
 *
 * 创建一个监听模式的原始套接字
 *
 * Parameters:
 *	_devname 	-	原始设备名称
 *
 * Returns:
 *	成功	-	返回地址族
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern x12_socket_raw_t* x12_socket_raw_creat_monitor(const char *_devname);

/*
 * Function: x12_socket_raw_read
 *
 * 从原始套接字中读取数据
 *
 * Parameters:
 *	_raw 	-	原始套接字
 *	_buflen	-	读取数据的长度
 *
 * Returns:
 *	成功	-	返回读取数据
 *	失败	-	NULL
 *
 * See Also:
 *	使用后清除缓存
 *
 */
extern unsigned char* x12_socket_raw_read(x12_socket_raw_t *_raw, int *_buflen);

/*
 * Function: x12_socket_raw_free
 *
 * 释放一个原始套接字
 *
 * Parameters:
 *	_raw 	-	原始套接字
 *
 *
 * See Also:
 *
 */
extern void x12_socket_raw_free(x12_socket_raw_t *_raw);

#endif

#endif
