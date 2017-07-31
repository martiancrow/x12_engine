/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */


/*
 *x12 epoll 事件监控头文件
 *
 *
 *作者:刘晓龙
 *时间:2017-06-14
 *最后修改时间:2016-08-20
 */

#ifdef _OS_LINUX_

#ifndef _X12_EPOLL_H_INCLUDE_
#define _X12_EPOLL_H_INCLUDE_

#define MAX_SIZE 512

#include <sys/epoll.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "x12_pthread.h"
#include "x12_even.h"


/*
 * Function: x12_epoll_init
 *
 * 初始化epoll模型
 *
 *
 * Parameters:
 *	_timeout 	-     	超时时间
 *
 * Returns:
 * 	0	-	成功
 *	-1	-	失败
 * 	
 *
 * See Also:
 *
 */
extern char x12_epoll_init(unsigned int _timeout);

/*
 * Function: x12_epoll_loop
 *
 * epoll监听循环线程
 *
 *
 * See Also:
 *
 */
extern void x12_epoll_loop(void);

/*
 * Function: x12_epoll_loopforever
 *
 * select监听线程持久化
 *
 *
 * See Also:
 *
 */
extern void x12_epoll_loopforever(void);

/*
 * Function: x12_epoll_addlisten
 *
 * 添加监听
 *
 *
 * Parameters:
 *	_fd 	-     监听套接字
 *
 * Returns:
 * 	0	-	成功
 *	-1	-	失败
 * 	
 *
 * See Also:
 *
 */
extern char x12_epoll_addlisten(int _fd);

/*
 * Function: x12_epoll_dellisten
 *
 * 删除监听
 *
 *
 * Parameters:
 *	_fd 	-     监听套接字
 *
 * Returns:
 * 	0	-	成功
 *	-1	-	失败
 * 	
 *
 * See Also:
 *
 */
extern char x12_epoll_dellisten(int _fd);


#endif


#endif
