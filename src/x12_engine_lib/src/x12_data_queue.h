/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */


/*
 *x12 队列,建议放入等长数据,队列本身不记录数据长度
 *
 *
 *作者:Crow
 *时间:2016-07-17
 *最后修改时间:2016-07-17
 */

#ifndef _X12_DATA_QUEUE_H_INCLUDE_
#define _X12_DATA_QUEUE_H_INCLUDE_

#include <stdlib.h>
#include <string.h>
#include "x12_pthread.h"

#define data_queue_align(d, a) (((d) + (a - 1)) & ~(a - 1))	//内存对齐

#define DATA_QUEUE_DEF_CLEAN_SIZE			512		//默认数据清理阀值
#define DATA_QUEUE_DEF_BLOCK_SIZE			128		//默认数据块大小

#define DATA_QUEUE_ARGS_NULL				-1		//参数为空
#define DATA_QUEUE_LOCK_FAIL				-2		//获取锁失败
#define DATA_QUEUE_LOCK_NONE				-3		//线程不拥有锁

typedef enum  data_queue_errno_e data_queue_errno_t;

enum data_queue_errno_e
{
	success = 0,
	args_null = -1,
	lock_fail = -2,
	lock_none = -3,
	blocksizeerr = -4,
	endatasizeerr = -5,
};

typedef int dq_size_t;

/*
 * Function Pointer: data_queue_cleanpre_funp
 *
 * 清除数据预处理函数
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
typedef short (*data_queue_cleanpre_funp)(unsigned char *_cleandata, dq_size_t _datalen);

typedef struct x12_data_queue_s x12_data_queue_t;

struct x12_data_queue_s 
{
	dq_size_t size;							//队列大小
	dq_size_t dataoffset;					//队列头偏移量
	dq_size_t readhead;						//读取首地址
	dq_size_t datalen;						//数据长度
	unsigned char *data;					//数据指针
	dq_size_t blocksize;					//每次分配的块大小
	dq_size_t cleansize;					//清理阀值
	data_queue_cleanpre_funp cleanprefun;	//清理预处理函数
	x12_pthread_mutex_t mutex;				//队列互斥锁
	x12_pthread_cond_t can_read;			//队列可读信号量
	x12_pthread_cond_t can_write;			//队列可写信号量
};

/*
 * 错误编号
 */
extern data_queue_errno_t dq_errno;

/*
 * Function: x12_data_queue_creat
 *
 * 创建队列
 *
 * Parameters:
 * _funp	-	清理预处理函数
 *
 * Returns:
 *	成功	-	队列指针
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern x12_data_queue_t* x12_data_queue_creat(dq_size_t _blocksize, dq_size_t _cleansize, data_queue_cleanpre_funp _funp);

/*
 * Function: x12_data_queue_destroy
 *
 * 销毁队列
 *
 * Parameters:
 *	_queue 		-	队列
 *
 * See Also:
 *
 */
extern void x12_data_queue_destroy(x12_data_queue_t *_queue);

/*
 * Function: x12_data_queue_enqueue
 *
 * 进入队列
 *
 * Parameters:
 *	_queue			-	队列
 *	_data 			-	数据指正
 *	_datalen 		-	数据长度
 *
 * Returns:
 *	成功	-	写入数据长度
 *	失败	-	负数错误码
 *
 * See Also:
 *
 */
extern dq_size_t x12_data_queue_enqueue(x12_data_queue_t *_queue, unsigned char *_data, dq_size_t _datalen);

/*
 * Function: x12_data_queue_dequeue
 *
 * 读取数据
 *
 * Parameters:
 *	_queue 			-	队列
 *	_buf 			-	读取缓存
 *	_readlen		-	要读取的
 *
 * Returns:
 *	成功	-	实际读取数据长度
 *	失败	-	错误码
 *
 * See Also:
 *	读取完后请使用x12_data_queue_unlock释放,如需回退数据请使用x12_data_queue_reset
 */
extern dq_size_t x12_data_queue_dequeue(x12_data_queue_t *_queue, unsigned char *_buf, dq_size_t _readlen);

/*
 * Function: x12_data_queue_lock
 *
 * 对资源上锁但不读取数据
 *
 * Parameters:
 *	_queue 			-	队列
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误码
 *
 * See Also:
 *	读取完后请使用x12_data_queue_unlock释放,如需回退数据请使用x12_data_queue_reset
 */
extern short x12_data_queue_lock(x12_data_queue_t *_queue);

/*
 * Function: x12_data_queue_unlock
 *
 * 读取数据
 *
 * Parameters:
 *	_queue 			-	队列
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误码
 *
 * See Also:
 *	只有在读取后才能使用,否者报错
 */
extern short x12_data_queue_unlock(x12_data_queue_t *_queue);

/*
 * Function: x12_data_queue_reset
 *
 * 重置数据
 *
 * Parameters:
 *	_queue 			-	队列
 *	_redatalen		-	回退数据长度
 *
 * Returns:
 *	成功	-	回退的数据长度
 *	失败	-	错误码
 *
 * See Also:
 *	可代替unlock使用,同样在读取数据后才能使用
 */
extern dq_size_t x12_data_queue_reset(x12_data_queue_t *_queue, dq_size_t _redatalen);

/*
 *
 * Function: x12_data_queue_getsize
 *
 * 获取数据池大小
 *
 * Parameters:
 *	_queue 			-	队列
 *
 * Returns:
 *	数据池长度
 *
 * See Also:
 *
 */
extern dq_size_t x12_data_queue_getsize(x12_data_queue_t *_queue);

/*
 * Function: x12_data_queue_getdatalen
 *
 * 获取数据大小
 *
 * Parameters:
 *	_queue 			-	队列
 *
 * Returns:
 *	队列数据长度
 *
 * See Also:
 *
 */
extern dq_size_t x12_data_queue_getdatalen(x12_data_queue_t *_queue);

/*
 * Function: x12_data_queue_isempty
 *
 * 队列是否为空
 *
 * Parameters:
 *	_queue 		-	队列
 *
 * Returns:
 *	0	-	不为空
 *	1	-	为空
 *
 * See Also:
 *
 */
extern char x12_data_queue_isempty(x12_data_queue_t *_queue);


#endif



