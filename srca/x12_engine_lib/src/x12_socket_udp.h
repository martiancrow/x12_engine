/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 */


/*
 *x12 udp socket套接字
 *
 *
 *作者:Crow
 *时间:2016-08-20
 *最后修改时间:2016-08-20
 */

#ifndef _X12_SOCKET_UDP_H_INCLUDE_
#define _X12_SOCKET_UDP_H_INCLUDE_

#define SOCKET_UDP_TYPE_CONN	1	//udp链接
#define SOCKET_UDP_TYPE_LISTEN	2	//udp监听
#define SOCKET_UDP_TYPE_ACCEPT	3	//udp同意客户端

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>

#include "x12_even.h"
#include "x12_even_drivers.h"

typedef int x12_socket_size_t;
typedef struct x12_socket_udp_s x12_socket_udp_t;

struct x12_socket_udp_s 
{
	int fd;						//套接字
	struct sockaddr_in addr;	//socket地址族
	unsigned int addrlen;		//地址长度
	unsigned char type;			//udp类型(监听或链接)
};

/*
 * Function: x12_scoket_udp_creat
 *
 * 创建一个空的udp协议族
 *
 * Returns:
 *	成功	-	返回地址族
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern x12_socket_udp_t* x12_socket_udp_creat(void);

/*
 * Function: x12_scoket_udp_creat_accept
 *
 * 创建一个空的udp协议族
 *
 * Parameters:
 *  _fd		-	套接字句柄
 *
 * Returns:
 *	成功	-	返回地址族
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern x12_socket_udp_t* x12_socket_udp_creat_accept(int _fd);

/*
 * Function: x12_scoket_udp_creat_listen
 *
 * 创建一个监听udp协议族
 *
 * Parameters:
 *	_addr 	-	监听ip地址
 *	_port	-	监听端口
 *
 * Returns:
 *	成功	-	返回地址族
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern x12_socket_udp_t* x12_socket_udp_creat_listen(const char *_addr, int _port);

/*
 * Function: x12_scoket_udp_creat
 *
 * 创建一个连接udp协议族
 *
 * Parameters:
 *	_addr 	-	连接ip地址
 *	_port	-	连接端口
 *
 * Returns:
 *	成功	-	返回地址族
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern x12_socket_udp_t* x12_socket_udp_creat_connection(const char *_addr, int _port);

/*
 * Function: x12_socket_udp_read
 *
 * 读取数据
 *
 * Parameters:
 *	_udp 		-	套接字
 *	_buflen 	-	缓冲长度
 *	_from		-	发送端信息
 *
 * Returns:
 *	成功	-	返回缓冲区
 *	失败	-	返回NULL
 *
 * See Also:
 *  使用后请清除缓存
 */
extern unsigned char * x12_socket_udp_read(x12_socket_udp_t *_udp, x12_socket_size_t *_buflen, x12_socket_udp_t *_from);

/*
 * Function: x12_socket_udp_send
 *
 * 发送数据
 *
 * Parameters:
 *	_udp 		-	套接字
 *	_buf 		-	读数据缓存
 *	_buflen 	-	数据池
 *
 * Returns:
 *	成功	-	返回发送长度
 *	失败	-	负数代码
 *
 * See Also:
 *
 */
extern x12_socket_size_t x12_socket_udp_send(x12_socket_udp_t *_udp, unsigned char *_buf, x12_socket_size_t _buflen);

/*
 * Function: x12_socket_udp_sendbroadcast
 *
 * 发送udp广播
 *
 * Parameters:
 *	_udp 		-	套接字
 *	_buf 		-	发送数据
 *	_buflen 	-	数据长度
 *
 * Returns:
 *	成功	-	返回发送长度
 *	失败	-	负数代码
 *
 * See Also:
 *
 */
extern x12_socket_size_t x12_socket_udp_sendbroadcast(x12_socket_udp_t *_udp, unsigned char *_buf, x12_socket_size_t _buflen);

/*
 * Function: x12_socket_udp_distory
 *
 * 销毁udp套接字
 *
 * Parameters:
 *	_udp 		-	套接字
 *
 * See Also:
 *
 */
extern void x12_socket_udp_distory(x12_socket_udp_t *_udp);

/*
 * Function: x12_socket_udp_even_add
 *
 * 为udp添加监听事件
 *
 * Parameters:
 *	_udp 		-	套接字
 *	_queue 		-	数据队列
 *
 * Returns:
 *	成功	-	事件监听返回
 *	失败	-	负数代码
 *
 * See Also:
 *(注意:事件类型为1)
 */
extern char x12_socket_udp_even_add(void* _udp, void* _queue);

/*
 * Function: x12_socket_udp_even_add
 *
 * 为udp注册监听设备
 *
 * Parameters:
 *	_udp 		-	套接字
 *
 * Returns:
 *	成功	-	设备id
 *	失败	-	-1
 *
 * See Also:
 *(注意:事件类型为1)
 */
extern int x12_socket_udp_driver_add(void* _udp);

#endif



