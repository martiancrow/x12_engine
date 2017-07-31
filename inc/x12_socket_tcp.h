/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 */


/*
 *x12 tcp socket套接字
 *
 *
 *作者:Crow
 *时间:2017-01-05
 *最后修改时间:2017-01-05
 */

#ifndef _X12_SOCKET_TCP_H_INCLUDE_
#define _X12_SOCKET_TCP_H_INCLUDE_

#define SOCKET_TCP_TYPE_CONN	1	//tcp链接
#define SOCKET_TCP_TYPE_LISTEN	2	//tcp监听
#define SOCKET_TCP_TYPE_ACCEPT	3	//tcp同意客户端
#define SOCKET_TCP_LISTEN_QUEUE 128	//监听队列长度

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>

typedef int x12_socket_tcp_size_t;
typedef struct x12_socket_tcp_s x12_socket_tcp_t;

struct x12_socket_tcp_s 
{
	int fd;						//套接字
	struct sockaddr_in addr;	//socket地址族
	unsigned int addrlen;		//地址长度
	unsigned char type;			//tcp类型(监听或链接)
	int accfd;					//tcp链接fd
};

/*
 * Function: x12_scoket_tcp_creat
 *
 * 创建一个空的tcp协议族
 *
 * Returns:
 *	成功	-	返回地址族
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern x12_socket_tcp_t* x12_socket_tcp_creat(void);

/*
 * Function: x12_scoket_tcp_creat_accept
 *
 * 创建一个链接句柄
 *
 * Parameters:
 *  _fd		-	监听套接字句柄
 *
 * Returns:
 *	成功	-	返回地址族
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern x12_socket_tcp_t* x12_socket_tcp_creat_accept(int _fd);

/*
 * Function: x12_scoket_tcp_creat_listen
 *
 * 创建一个监听tcp协议族
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
extern x12_socket_tcp_t* x12_socket_tcp_creat_listen(const char *_addr, int _port);

/*
 * Function: x12_scoket_tcp_creat_connection
 *
 * 创建一个连接tcp协议族
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
extern x12_socket_tcp_t* x12_socket_tcp_creat_connection(const char *_addr, int _port);

/*
 * Function: x12_socket_tcp_read
 *
 * 读取数据
 *
 * Parameters:
 *	_tcp 		-	套接字
 *	_buflen 	-	缓冲长度
 *
 * Returns:
 *	成功	-	返回缓冲区
 *	失败	-	返回NULL
 *
 * See Also:
 *  使用后请清除缓存
 */
extern unsigned char * x12_socket_tcp_read(x12_socket_tcp_t *_tcp, x12_socket_tcp_size_t *_buflen);

/*
 * Function: x12_socket_tcp_send
 *
 * 发送数据
 *
 * Parameters:
 *	_tcp 		-	套接字
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
extern x12_socket_tcp_size_t x12_socket_tcp_send(x12_socket_tcp_t *_tcp, unsigned char *_buf, x12_socket_tcp_size_t _buflen);

/*
 * Function: x12_socket_tcp_close
 *
 * 销毁tcp套接字
 *
 * Parameters:
 *	_tcp 		-	套接字
 *
 * See Also:
 *
 */
extern int x12_socket_tcp_close(x12_socket_tcp_t *_tcp);

/*
 * Function: x12_socket_tcp_distory
 *
 * 销毁tcp套接字
 *
 * Parameters:
 *	_tcp 		-	套接字
 *
 * See Also:
 *
 */
extern void x12_socket_tcp_distory(x12_socket_tcp_t *_tcp);

#endif



