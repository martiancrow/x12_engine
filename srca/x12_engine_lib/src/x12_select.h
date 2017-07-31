/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */


/*
 *x12 select 事件监控头文件
 *
 *
 *作者:Crow
 *时间:2016-08-20
 *最后修改时间:2016-08-20
 */

#ifndef _X12_SELECT_H_INCLUDE_
#define _X12_SELECT_H_INCLUDE_

#include <sys/select.h>
#include <signal.h>

#define X12_SELECT_DEF_TIMEOUT	5	//默认超时时间

/*
 * Function: x12_select_init
 *
 * 初始化select
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
extern char x12_select_init(unsigned int _timeout);

/*
 * Function: x12_select_loop
 *
 * select监听循环线程
 *
 *
 * See Also:
 *
 */
extern void x12_select_loop(void);

/*
 * Function: x12_select_loopforever
 *
 * select监听线程持久化
 *
 *
 * See Also:
 *
 */
extern void x12_select_loopforever(void);

/*
 * Function: x12_select_addlisten
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
extern char x12_select_addlisten(int _fd);

/*
 * Function: x12_select_dellisten
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
extern char x12_select_dellisten(int _fd);


#endif



