/*
 *
 * Copyright (C) 刘晓龙
 * Copyright (C) 昆明黑海科技有限公司
 *
 */


/*
 * 
 * 80211空气广播工具
 *
 * 作者:Crow
 * 时间:2017-05-01
 * 最后修改时间:2017-05-01
 *      
 */

#ifndef _IEEE80211_AIR_TOOL_H_INCLUDE_
#define _IEEE80211_AIR_TOOL_H_INCLUDE_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver_nl80211.h"

/*
 * Function: ieee80211_air_tool_scan_gettag
 *
 * 获取scan标签
 *
 * Parameters:
 * 	_data		-	80211帧
 * 	_datalen	-	帧长度
 * 	_tag		-	标签标识
 * 	_taglen		-	标签长度
 * 	_index		-	序列
 *
 * Returns:
 *	成功	-	返回标签内容
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern unsigned char* ieee80211_air_tool_scan_gettag_index(const unsigned char *_data, int _datalen, const unsigned char _tag, int *_taglen, int _index);

/*
 * Function: ieee80211_air_tool_scan_gettagcount
 *
 * 获取scan标签个数
 *
 * Parameters:
 * 	_data		-	80211帧
 * 	_datalen	-	帧长度
 * 	_tag		-	标签标识
 *
 * Returns:
 *	成功	-	返回标签数量
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int ieee80211_air_tool_scan_gettagcount(const unsigned char *_data, int _datalen, const unsigned char _tag);

/*
 * Function: ieee80211_air_tool_scan_gettag_merge
 *
 * 合并获取scan标签,如果标签相同则合并所有标签内容为一段内容
 *
 * Parameters:
 * 	_data		-	80211帧
 * 	_datalen	-	帧长度
 * 	_tag		-	标签标识
 * 	_taglen		-	标签长度
 *
 * Returns:
 *	成功	-	返回标签内容
 *	失败	-	NULL
 *
 * See Also:
 *
 */
extern unsigned char* ieee80211_air_tool_scan_gettag_merge(const unsigned char *_data, int _datalen, const unsigned char _tag, int *_taglen);

/*
 * Function: ieee80211_air_tool_scan_getsignal
 *
 * 获取scan信号强度
 *
 * Parameters:
 *	_data	 	-	80211帧
 *	_datalen	-	帧长度
 *
 * Returns:
 *	成功	-	返回信号强度(单位dBm)
 *	失败	-	返回1
 *
 * See Also:
 *
 */
extern char ieee80211_air_tool_scan_getsignal(const unsigned char *_data, int _datalen);

/*
 * Function: ieee80211_air_tool_scan_getsa
 *
 * 获取scan发送地址
 *
 * Parameters:
 * 	_sa			-	mac地址
 *	_data	 	-	80211帧
 *	_datalen	-	帧长度
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern char ieee80211_air_tool_scan_getsa(const unsigned char *_data, int _datalen, unsigned char _sa[6]);

/*
 * Function: ieee80211_air_tool_scan_getda
 *
 * 获取scan发送地址
 *
 * Parameters:
 * 	_da			-	mac地址
 *	_data	 	-	80211帧
 *	_datalen	-	帧长度
 *
 * Returns:
 *	成功	-	0
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern char ieee80211_air_tool_scan_getda(const unsigned char *_data, int _datalen, unsigned char _da[6]);

/*
 * Function: ieee80211_air_tool_scan_senddata
 *
 * 获取scan发送地址
 *
 * Parameters:
 * 	_driver		-	设备名称
 *	_ssid	 	-	ssid内容
 *	_ssidlen	-	ssid长度
 *	_tag		-	tag标识号
 *	_tagdata	-	tag数据
 *	_taglen		-	tag数据长度
 *
 * Returns:
 *	成功	-	发送数据总长度
 *	失败	-	-1
 *
 * See Also:
 *
 */
extern int ieee80211_air_tool_scan_senddata(const char *_driver, unsigned char *_ssid, unsigned char _ssidlen, char _tag, unsigned char *_tagdata, int _taglen);

#endif

