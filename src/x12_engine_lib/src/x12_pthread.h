/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */


/*
 *x12 engine 线程封装
 *
 *
 *作者:Crow
 *时间:2016-07-17
 *最后修改时间:2016-07-17
 */

#ifndef _X12_PTHREAD_H_INCLUDE_
#define _X12_PTHREAD_H_INCLUDE_

#include <pthread.h>

#define X12_PTHREAD_MUTEX_ERRORCHECK	PTHREAD_MUTEX_ERRORCHECK	//检错锁
#define X12_PTHREAD_LOCK_OK				0		//上锁成功
#define X12_PTHREAD_LOCK_EDEADLK		EDEADLK	//当前线程已获取互斥锁
#define X12_PTHREAD_LOCK_EINVAL			EINVAL	//未初始化互斥锁
#define X12_PTHREAD_LOCK_EAGAIN			EAGAIN	//超过递归锁最大数量

typedef pthread_t x12_pthread_t;
typedef pthread_attr_t x12_pthread_attr_t;

typedef pthread_mutex_t x12_pthread_mutex_t;
typedef pthread_mutexattr_t  x12_pthread_mutexattr_t;

typedef pthread_cond_t x12_pthread_cond_t;
typedef pthread_condattr_t x12_pthread_condattr_t; 

/*
 * Function: x12_pthread_mutexattr_init
 *
 * 互斥锁属性初始化函数
 *
 * Parameters:
 *	_attr			-	互斥锁属性
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_mutexattr_init(x12_pthread_mutexattr_t *_attr);

/*
 * Function: x12_pthread_mutexattr_settype
 *
 * 互斥锁属性设置函数
 *
 * Parameters:
 *	_attr			-	互斥锁属性
 *	_type			-	互斥锁类型
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_mutexattr_settype(x12_pthread_mutexattr_t *_attr, int _type);

/*
 * Function: x12_pthread_mutex_init
 *
 * 互斥锁初始化函数
 *
 * Parameters:
 *	_mutex 			-	互斥锁
 *	_attr			-	互斥锁属性
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_mutex_init(x12_pthread_mutex_t *_mutex, const x12_pthread_mutexattr_t *_attr);

/*
 * Function: x12_pthread_mutex_destroy
 *
 * 互斥锁销毁
 *
 * Parameters:
 *	_mutex 			-	互斥锁
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_mutex_destroy(x12_pthread_mutex_t *_mutex);

/*
 * Function: x12_pthread_cond_init
 *
 * 初始化信号量
 *
 * Parameters:
 *	_cond 			-	信号量
 *	_attr			-	信号量属性
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_cond_init(x12_pthread_cond_t *_cond, const x12_pthread_condattr_t *_attr);

/*
 * Function: x12_pthread_cond_destroy
 *
 * 销毁信号量
 *
 * Parameters:
 *	_cond 			-	信号量
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_cond_destroy(x12_pthread_cond_t *_cond);

/*
 * Function: x12_pthread_mutex_lock
 *
 * 上锁
 *
 * Parameters:
 *	_mutex 			-	互斥锁
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_mutex_lock(x12_pthread_mutex_t *_mutex);

/*
 * Function: x12_pthread_mutex_trylock
 *
 * 上锁
 *
 * Parameters:
 *	_mutex 			-	互斥锁
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_mutex_trylock(x12_pthread_mutex_t *_mutex);

/*
 * Function: x12_pthread_mutex_lock
 *
 * 解锁
 *
 * Parameters:
 *	_mutex 			-	互斥锁
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_mutex_unlock(x12_pthread_mutex_t *_mutex);

/*
 * Function: x12_pthread_cond_signal
 *
 * 发布一个信号
 *
 * Parameters:
 *	_cond 			-	信号量
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_cond_signal(x12_pthread_cond_t *_cond);

/*
 * Function: x12_pthread_cond_wait
 *
 * 等待一个信号
 *
 * Parameters:
 *	_cond 			-	信号量
 *	_mutex 			-	互斥锁
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_cond_wait(x12_pthread_cond_t *_cond, x12_pthread_mutex_t *_mutex);

/*
 * Function: x12_pthread_create
 *
 * 创建一个线程
 *
 * Parameters:
 *	_thread 		-	线程结构体
 *	_attr 			-	线程属性
 *	_start_rtn		-	函数入口
 *	_arg			-	参数列表
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_create(x12_pthread_t *_thread, x12_pthread_attr_t *_attr, void* (*_start_rtn)(void*), void* _arg);

/*
 * Function: x12_pthread_join
 *
 * 挂起一个线程
 *
 * Parameters:
 *	_thread 		-	线程结构体
 *	_retval			-	返回结果地址
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern int x12_pthread_join(x12_pthread_t _thread, void **_retval);

/*
 * Function: x12_pthread_exit
 *
 * 退出线程返回结果
 *
 * Parameters:
 *	_retval			-	返回结果
 *
 *
 * See Also:
 *
 */
extern void x12_pthread_exit(void* _retval);
#endif



