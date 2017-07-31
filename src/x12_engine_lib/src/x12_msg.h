/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 engine 消息模块
 *
 *
 *作者:Crow
 *时间:2016-12-10
 *最后修改时间:2016-12-10
 */

#ifndef _X12_MSG_H_INCLUDE_
#define _X12_MSG_H_INCLUDE_

typedef int msg_size_t;

/*
 * Function: x12_msg_send
 *
 * 发送消息
 *
 * Parameters:
 * 	_driverid	-	设备id
 * 	_data		-	发送数据
 * 	_datalen	-	数据长度
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern msg_size_t x12_msg_send(int _driverid, unsigned char *_data, msg_size_t _datalen);

#endif



