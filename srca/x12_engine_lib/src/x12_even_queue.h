/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */


/*
 *x12 engine 事件数据队列(底层事件封装)
 *
 *
 *作者:Crow
 *时间:2016-11-10
 *最后修改时间:2016-11-10
 */

#ifndef _X12_EVEN_QUEUE_H_INCLUDE_
#define _X12_EVEN_QUEUE_H_INCLUDE_

#include "x12_data_queue.h"
#include "x12_pthread.h"

typedef int even_queue_size_t;

typedef struct x12_evens_queue_item_s x12_evens_queue_item_t;

struct x12_evens_queue_item_s
{
	unsigned char* data;				//事件数据
	even_queue_size_t datalen;			//数据长度
	int evendriverid;					//事件设备id
	unsigned short eventype;			//事件类型
};

/*
 * Function: x12_evens_queue_willread_funp
 *
 * 数据预读事件回调函数
 * 
 * Parameters:
 * 	_dataqueue	-	数据队列
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
typedef char (*x12_evens_queue_willread_funp)(void* _dataqueue);

typedef struct x12_evens_queue_pthread_data_s x12_evens_queue_pthread_data_t;

struct x12_evens_queue_pthread_data_s
{
	x12_pthread_t pid;									//线程id
	x12_evens_queue_willread_funp willreadcallback;		//读取回调函数
	void* queue;										//数据队列
	char isexit;										//是否退出
};

typedef struct x12_evens_queue_pthread_list_s x12_evens_queue_pthread_list_t;

struct x12_evens_queue_pthread_list_s
{
	x12_evens_queue_pthread_data_t *pthreadlist;	//线程队列
	even_queue_size_t count;						//队列长度
};

//队列结构体
typedef struct x12_evens_queue_s x12_evens_queue_t;

struct x12_evens_queue_s
{
	x12_data_queue_t *msgqueue;
	x12_evens_queue_pthread_list_t *plist;
};

/*
 * Function: x12_evens_queue_creat
 *
 * 初始化事件队列
 *
 * Parameters:
 * 	
 *
 * Returns:
 *	成功	-	队列结构体
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern x12_evens_queue_t* x12_evens_queue_creat(void);

/*
 * Function: x12_evens_queue_destroy
 *
 * 销毁事件列表
 *
 * Parameters:
 * 	_queue	-	需要销毁的队列
 *
 * See Also:
 *
 */
extern void x12_evens_queue_destroy(x12_evens_queue_t *_queue);

/*
 * Function: x12_evens_queue_enter
 *
 * 进入队列
 *
 * Parameters:
 * 	_queue		-	数据队列
 *	_data 		-	数据指针
 *	_datalen	-	数据长度
 *	_driverid	-	设备id
 *	_type		-	事件类型
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern char x12_evens_queue_enter(x12_evens_queue_t *_queue, unsigned char *_data, even_queue_size_t _datalen, int _driverid, unsigned short _type);

/*
 * Function: x12_evens_queue_denter
 *
 * 出队列
 *
 * Parameters:
 * 	_queue	-	数据队列
 * 	_even	-	事件指针
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern char x12_evens_queue_denter(x12_evens_queue_t *_queue, x12_evens_queue_item_t *_even);

/*
 * Function: x12_evens_queue_giveupdenter
 *
 * 放弃出栈
 *
 * Parameters:
 * 	_queue	-	数据队列
 *  _count	-	放弃出栈数量
 *
 * Returns:
 * 	成功	-	0
 * 	失败	-	-1
 *
 * See Also:
 *
 */
extern char x12_evens_queue_giveupdenter(x12_evens_queue_t *_queue, int _count);

/*
 * Function: x12_evens_queue_pthread_creat
 *
 * 创建一个读取线程
 * 
 * Parameters:
 * 	_queue	-	数据队列
 * 	_funcp	-	预读回调函数指针
 * 	_pid	-	返回线程id
 * 
 * Returns:
 *	失败	-	-1
 *	成功	-	线程id
 *
 * See Also:
 *
 */
extern x12_pthread_t x12_evens_queue_pthread_creat(x12_evens_queue_t *_queue, x12_evens_queue_willread_funp _funcp);

/*
 * Function: x12_evens_queue_pthread_loopforever
 *
 * 等待一个读取线程退出
 * 
 * Parameters:
 * 	_pid	-	线程id
 *
 * See Also:
 *
 */
extern void x12_evens_queue_pthread_loopforever(x12_pthread_t _pid);

/*
 * Function: x12_evens_queue_pthread_finish
 *
 * 结束一个线程
 *
 * Parameters:
 * 	_queue	-	数据队列
 * 	_pid	-	线程id
 *
 * See Also:
 *
 */
extern void x12_evens_queue_pthread_finish(x12_evens_queue_t *_queue, x12_pthread_t _pid);

/*
 * Function: x12_evens_queue_isempty
 *
 * 创建一个读取线程
 * 
 * Parameters:
 * 	_queue	-	数据队列
 * 
 * Returns:
 *	空			-	1
 *	不为空		-	0
 *	参数错误	-	-1
 *
 * See Also:
 *
 */
extern char x12_evens_queue_isempty(x12_evens_queue_t *_queue);

#endif



