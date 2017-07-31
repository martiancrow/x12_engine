/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */


/*
 *x12 engine 底层事件
 *
 *
 *作者:Crow
 *时间:2016-07-17
 *最后修改时间:2016-07-17
 */

#ifndef _X12_EVEN_H_INCLUDE_
#define _X12_EVEN_H_INCLUDE_

typedef enum eventype_e {
	testtype,
	testtype2,
} eventype_t;

typedef int even_size_t;

typedef struct x12_listen_even_s x12_listen_even_t;

struct x12_listen_even_s
{
	void* data;					//事件数据
	even_size_t datalen;		//数据长度
	void* dataqueue;			//数据队列
	unsigned short eventype;	//数据类型
	int fd;						//监听fd
};

typedef struct x12_listen_evens_s x12_listen_evens_t;

struct x12_listen_evens_s
{
	even_size_t listen_len;
	x12_listen_even_t *listen_evens;
	even_size_t listen_size;
};

/*
 * Function: x12_listen_evens_init
 *
 * 初始化监听事件函数
 *
 * Returns:
 *	成功	-	监听事件函数
 *	失败	-	null
 *
 * See Also:
 *
 */
extern char x12_listen_evens_init(void);

/*
 * Function: x12_listen_evens_destroy
 *
 * 销毁监听事件列表
 *
 * See Also:
 *
 */
extern void x12_listen_evens_destroy(void);

/*
 * Function: x12_listen_evens_forever
 *
 * 持久化监听事件
 *
 * See Also:
 *
 */
extern void x12_listen_evens_forever(void);

/*
 * Function: x12_listen_evens_add
 *
 * 添加事件
 *
 * Parameters:
 *	_fd				-	监听句柄
 *	_data			-	预制数据
 *	_datalen		-	数据长度
 *	_dataqueue		-	监听数据队列
 *	_eventype		-	监听数据类型
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern char x12_listen_evens_add(int _fd, void* _data, even_size_t _datalen, void* _dataqueue, unsigned short _eventype);

/*
 * Function: x12_listen_evens_del
 *
 * 删除监听事件
 *
 * Parameters:
 *	_fd 	-	互斥锁
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern char x12_listen_evens_del(int _fd);

/*
 * Function: x12_even_callback
 *
 * 总体事件回调函数
 *
 * Parameters:
 *	_fd 	-	监听句柄
 *
 * Returns:
 *	成功	-	0
 *	失败	-	错误代码
 *
 * See Also:
 *
 */
extern char x12_even_callback(int _fd);

/*
 * Function: x12_listen_evens_search
 *
 * 通过fd查找事件
 *
 * Parameters:
 *	_fd 	-	监听句柄
 *
 * Returns:
 *	成功	-	事件索引位置
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern even_size_t x12_listen_evens_search(int _fd);

#endif



