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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "x12_msg.h"
#include "x12_even_drivers.h"
#include "x12_socket_udp.h"

msg_size_t x12_msg_send(int _driverid, unsigned char *_data, msg_size_t _datalen)
{
	if(_data == NULL)
	{
		return -1;
	}

	x12_even_driver_t *driver = x12_even_drivers_search(_driverid);

	msg_size_t retlen = 0;


	if(driver != NULL)
	{
		retlen = x12_socket_udp_send(driver->linkdata, _data, _datalen);
	}	

	return retlen;
}

