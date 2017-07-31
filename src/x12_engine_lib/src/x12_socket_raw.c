/*
 *
 * Copyright (C) 刘晓龙
 * Copyright (C) 昆明黑海科技有限公司
 *
 */


/*
 * 
 * 原始套接字操作
 *
 * 作者:Crow
 * 时间:2017-03-22
 * 最后修改时间:2017-03-22
 *      
 */

#ifdef _OS_LINUX_

#include "x12_socket_raw.h"

x12_socket_raw_t* x12_socket_raw_creat(void)
{
	struct x12_socket_raw_s *raw = (x12_socket_raw_t *)malloc(sizeof(x12_socket_raw_t));
	memset(raw, 0, sizeof(x12_socket_raw_t));

	raw->fd = -1;
	raw->addrlen = sizeof(raw->addr);

	return raw;
}

x12_socket_raw_t* x12_socket_raw_creat_monitor(const char *_devname)
{

    char *newdevname = "mon0";

    nl80211_t *state = nl80211_socket_creat();

	if(!state)
	{
		return NULL;
	}

    nl80211_interface_add(_devname, newdevname, NL80211_IFTYPE_MONITOR);

    //启动网卡
    nl80211_interface_dev_up(newdevname, IFF_PROMISC);

    x12_socket_raw_t *raw = x12_socket_raw_creat();

	if ((raw->fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) 
    {
    	x12_socket_raw_free(raw);
        return NULL;
    }

    //设置为非阻塞socket
    int flags = fcntl(raw->fd, F_GETFL, 0);
	if(fcntl(raw->fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		return NULL;
	}
	

    uint8_t src_mac[6];
    memset(src_mac, 0, sizeof(src_mac));


    nl80211_get_mac(newdevname, src_mac);
	
	//printf("%02x:%02x:%02x:%02x:%02x:%02x\n", src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5]);

    (raw->addr).sll_ifindex = if_nametoindex(newdevname);
    (raw->addr).sll_family = AF_PACKET;
    memcpy ((raw->addr).sll_addr, src_mac, 6);
    (raw->addr).sll_halen = htons(6);

    if(bind(raw->fd, (struct sockaddr *)&(raw->addr), raw->addrlen) < 0)
    {
    	x12_socket_raw_free(raw);
        return NULL;
    }

    raw->type = 1;

	return raw;
}

unsigned char* x12_socket_raw_read(x12_socket_raw_t *_raw, int *_buflen)
{
	if(!_raw)
	{
		return NULL;
	}

	int bufblock = 64;
	int buflen = bufblock;
	unsigned char *buf = (unsigned char *)malloc(buflen);
	memset(buf, 0, buflen);

	int readlen = 0;
	
	do
	{

		if(readlen == buflen)
		{
			buflen += bufblock;
			buf = realloc(buf, buflen);
		}

		readlen = recv(_raw->fd, buf, buflen, MSG_PEEK);

		if(readlen < 0)
		{
			if(errno == EAGAIN || errno == EDEADLK)
			{
				continue;
			}
			else
			{
				*_buflen = readlen;
				return NULL;
			}
		}
		
		
	}
	while(readlen == buflen || readlen < 0);
		
	memset(buf, 0, buflen);
	readlen = recv(_raw->fd, buf, buflen, 0);

	*_buflen = readlen;
	return buf;
}

void x12_socket_raw_free(x12_socket_raw_t *_raw)
{
	if(_raw)
	{

		nl80211_interface_del_forname("mon0");

		if(_raw->type == 1)
		{
			nl80211_interface_del_foridx(_raw->addr.sll_ifindex);
		}

		free(_raw);
		_raw = NULL;
	}
}

#endif

