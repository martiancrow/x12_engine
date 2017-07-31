/*
 *
 * Copyright (C) 刘晓龙
 * Copyright (C) 昆明黑海科技有限公司
 *
 */


/*
 * 
 * nl80211设备封装
 *
 * 作者:Crow
 * 时间:2017-03-17
 * 最后修改时间:2017-03-17
 *      
 */

#ifdef _OS_LINUX_

#ifndef _DRIVER_NL80211_H_INCLUDE_
#define _DRIVER_NL80211_H_INCLUDE_

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdbool.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/family.h>
#include <netlink/genl/ctrl.h>
#include <netlink/socket.h>
#include <netlink/msg.h>
#include <netlink/attr.h>
#include <endian.h>

#include <linux/nl80211.h>

#include <linux/if_packet.h>  // struct sockaddr_ll (see man 7 packet)
#include <linux/if_ether.h>   // ETH_P_IP = 0x0800, ETH_P_IPV6 = 0x86DD
#include <sys/ioctl.h>        // macro ioctl is defined
#include <bits/ioctls.h>      // defines values for argument "request" of ioctl.

typedef struct nl80211_s nl80211_t;

struct nl80211_s
{
	struct nl_sock *nl80211_sock;
	int nl80211_id;
};

/*
 * Function: handle_cmd
 *
 * 执行nl80211命令
 *
 * Parametrs:
 *	_msg		-	发送往内核的信息
 *	_cb			-	回调结构
 *
 */
extern int handle_cmd(nl80211_t *_state, struct nl_msg *_msg, struct nl_cb *_cb);


/*
 * Function: nl80211_socket_creat
 *
 * 创建nl执行结构体
 * 
 * Return:
 * 	成功	-	返回结构体
 * 	失败	-	NULL
 *
 */
extern nl80211_t* nl80211_socket_creat(void);

/*
 * Function: nl80211_scan_msg_alloc
 *
 * 创建扫描消息
 *
 * Parametrs:
 *	_state		-	执行结构体
 *	_devname	-	设备名称
 * 
 * Return:
 * 	成功	-	返回已建立的扫描命令信息
 * 	失败	-	NULL
 *
 */
extern struct nl_msg* nl80211_scan_msg_alloc(nl80211_t *_state, const char *_devname);

/*
 * Function: nl80211_scan_msg_add_ssid
 *
 * 添加ssid信息
 *
 * Parametrs:
 *	_msg		-	需要添加的信息
 *	_ssid_data	-	ssid数据
 *	_ssid_len	-	ssid数据长度
 * 
 * Return:
 * 	成功	-	返回添加过ssid的扫描命令信息
 * 	失败	-	NULL
 *
 * Warring:
 * 	ssid不能大于32个byte否者报错
 *
 */
extern struct nl_msg* nl80211_scan_msg_add_ssid(struct nl_msg *_msg, uint8_t *_ssid_data, uint8_t _ssid_len);

/*
 * Function: nl80211_scan_msg_add_ie
 *
 * 添加ssid信息
 *
 * Parametrs:
 *	_msg		-	需要添加的信息
 *	_ie_data	-	ie数据
 *	_ie_len		-	ie数据长度
 * 
 * Return:
 * 	成功	-	返回添加过ie信息的扫描命令信息
 * 	失败	-	NULL
 *
 *
 */
extern struct nl_msg* nl80211_scan_msg_add_ie(struct nl_msg *_msg, uint8_t *_ie_data, int _ie_len);

/*
 * Function: nl80211_get_mac
 *
 * 获取网卡mac地址
 *
 * Parametrs:
 *	_devname	-	设备名称
 *	_mac		-	用于存放mac的内存
 * 
 * Return:
 * 	成功	-	0
 * 	失败	-	-1
 *
 *
 */
extern int nl80211_get_mac(const char *_devname, uint8_t *_mac);

/*
 * Function: nl80211_interface_add
 *
 * 删除无线设备
 *
 * Parametrs:
 *	_devname	-	复制设备名称
 *	_newdevname	-	新增设备名称
 *	_mode		-	设备模式
 * 
 * Return:
 * 	成功	-	0
 * 	失败	-	-1
 *
 * See Also:
 *	设备模式参考nl80211.h nl80211_iftype枚举
 *
 */
extern int nl80211_interface_add(const char *_devname, const char *_newdevname, int _mode);

/*
 * Function: nl80211_interface_del_forname
 *
 * 删除无线设备
 *
 * Parametrs:
 *	_devname	-	设备名称
 * 
 * Return:
 * 	成功	-	0
 * 	失败	-	-1
 *
 *
 */
extern int nl80211_interface_del_forname(const char *_devname);

/*
 * Function: nl80211_interface_del_foridx
 *
 * 删除无线设备
 *
 * Parametrs:
 *	_idx	-	设备编号
 * 
 * Return:
 * 	成功	-	0
 * 	失败	-	-1
 *
 *
 */
extern int nl80211_interface_del_foridx(int _idx);

/*
 * Function: nl80211_interface_dev_up
 *
 * 启动设备
 *
 * Parametrs:
 *	_devname	-	设备名称
 *	_flags		-	启动标识
 * 
 * Return:
 * 	成功	-	0
 * 	失败	-	-1
 *
 *
 */
extern int nl80211_interface_dev_up(const char *_devname, short _flags);

#endif

#endif
