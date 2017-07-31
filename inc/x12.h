/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *X12是一个轻量级的嵌入式http引擎,以及web开发框架,
 *简单实现HTTP1.1协议,以及动态网站中的get,post,cookie,session,路由功能
 *以lib形式发布,旨在为嵌入式程序提供使用网页交互能力
 *
 */


/*
 *X12 HTTP ENGINE的全局程序头文件
 *
 *
 *作者:Crow
 *时间:2016-01-08
 *最后修改时间:2016-01-08
 */

#ifndef _X12_H_INCLUDE_
#define _X12_H_INCLUDE_

#include "x12_socket_udp.h"
#include "x12_socket_tcp.h"

extern x12_socket_udp_t *server;
extern x12_socket_udp_t *connection;
extern x12_socket_tcp_t *tcp_server;
extern x12_socket_tcp_t *tcp_client;

#endif



