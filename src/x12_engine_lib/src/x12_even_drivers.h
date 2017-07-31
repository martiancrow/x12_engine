/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 engine 事件设备列表
 *
 *
 *作者:Crow
 *时间:2016-11-13
 *最后修改时间:2016-11-13
 */

#ifndef _X12_EVEN_DRIVERS_H_INCLUDE_
#define _X12_EVEN_DRIVERS_H_INCLUDE_

#include "x12_pthread.h"

#define driverlist_align(d, a) (((d) + (a - 1)) & ~(a - 1))	//内存对齐

#define DRIVERLIST_DEF_BLOCK_SIZE			128		//默认数据块大小

typedef int even_drivers_size_t;

typedef struct x12_even_driver_s x12_even_driver_t;
typedef struct x12_even_driverlist_s x12_even_driverlist_t;

struct x12_even_driver_s
{
	void* linkdata;				//链接数据
	int datalen;				//链接数据长度
	int driverid;				//设备id
	unsigned short eventype;	//事件类型
};

struct x12_even_driverlist_s
{
	int *nullid;						//空id列表
	even_drivers_size_t nullidoffset;	//空id偏移量
	even_drivers_size_t nullidsize;		//空id指针长度
	x12_even_driver_t *driverlist;		//设备列表
	even_drivers_size_t dataoffset;		//最后位置偏移量
	even_drivers_size_t drivercount;	//在编设备总数
	even_drivers_size_t listsize;		//列表分配大小
	even_drivers_size_t blocksize;		//默认块大小
	x12_pthread_mutex_t mutex;			//设备列表互斥锁
};

/*
 * Function: x12_even_drivers_init
 *
 * 初始化设备队列
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern char x12_even_drivers_init(void);

/*
 * Function: x12_even_drivers_destroy
 *
 * 销毁设备列表
 *
 * See Also:
 *
 */
extern void x12_even_drivers_destroy(void);

/*
 * Function: x12_even_drivers_search
 *
 * 查找设备
 *
 * Parameters:
 *	_id 	-	设备id
 *
 * Returns:
 *	成功	-	设备数据
 *	失败	-	null
 *
 * See Also:
 *
 */
extern x12_even_driver_t* x12_even_drivers_search(int _id);

/*
 * Function: x12_even_drivers_add
 *
 * 添加设备
 *
 * Parameters:
 *  _id			-	设备id
 *  _datalink	-	链接数据
 *  _eventype	-	事件类型
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *	重复	-	-2
 *
 * See Also:
 *
 */
extern char x12_even_drivers_add(int *_id, void* _linkdata, int _datalen, unsigned short _eventype);

/*
 * Function: x12_even_drivers_del
 *
 * 删除设备
 *
 * Parameters:
 *  _id			-	设备id
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern char x12_even_drivers_del(int _id);

#endif



