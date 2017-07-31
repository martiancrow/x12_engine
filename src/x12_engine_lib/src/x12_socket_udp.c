/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 */


/*
 *x12 udp socket套接字
 *
 *
 *作者:Crow
 *时间:2016-08-20
 *最后修改时间:2016-08-20
 */

#include "x12_socket_udp.h"
#include <stdio.h>

x12_socket_udp_t* x12_socket_udp_creat(void)
{
	struct x12_socket_udp_s *udp = (x12_socket_udp_t *)malloc(sizeof(x12_socket_udp_t));
	memset(udp, 0, sizeof(x12_socket_udp_t));

	udp->addr.sin_family = AF_INET;
	udp->addrlen = sizeof(udp->addr);
	udp->fd = -1;

	return udp;
}

x12_socket_udp_t* x12_socket_udp_creat_accept(int _fd)
{
	struct x12_socket_udp_s *udp = (x12_socket_udp_t *)malloc(sizeof(x12_socket_udp_t));
	memset(udp, 0, sizeof(x12_socket_udp_t));

	udp->addr.sin_family = AF_INET;
	udp->addrlen = sizeof(udp->addr);
	udp->fd = _fd;
	udp->type = SOCKET_UDP_TYPE_ACCEPT;

	return udp;
}

x12_socket_udp_t* x12_socket_udp_creat_listen(const char *_addr, int _port)
{
	struct x12_socket_udp_s *udp = (x12_socket_udp_t *)malloc(sizeof(x12_socket_udp_t));
	memset(udp, 0, sizeof(x12_socket_udp_t));

	udp->addr.sin_family = AF_INET;
	udp->addr.sin_port = htons(_port);
	
	if(_addr == NULL)
	{
		udp->addr.sin_addr.s_addr = htons(INADDR_ANY);
	}
	else
	{
		udp->addr.sin_addr.s_addr = inet_addr(_addr);
	}

	udp->addrlen = sizeof(udp->addr);
	udp->fd = socket(AF_INET, SOCK_DGRAM, 0);
	udp->type = SOCKET_UDP_TYPE_LISTEN;
	bind(udp->fd, (struct sockaddr*)&(udp->addr), udp->addrlen);

	//设置监听广播
	int opt = 1;
	setsockopt(udp->fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
	
	//设置为非阻塞socket
	//int flags = fcntl(udp->fd, F_GETFL, 0);
	fcntl(udp->fd, F_GETFL, 0);
	if(fcntl(udp->fd, F_SETFL, O_NONBLOCK) < 0)
	{
		return NULL;
	}

	return udp;
}

x12_socket_udp_t* x12_socket_udp_creat_connection(const char *_addr, int _port)
{

	if(_addr == NULL)
	{
		return NULL;
	}

	struct x12_socket_udp_s *udp = (x12_socket_udp_t *)malloc(sizeof(x12_socket_udp_t));
	memset(udp, 0, sizeof(x12_socket_udp_t));

	udp->addr.sin_family = AF_INET;
	udp->addr.sin_addr.s_addr = inet_addr(_addr);
	udp->addr.sin_port = htons(_port);
	udp->addrlen = sizeof(udp->addr);
	udp->fd = socket(AF_INET, SOCK_DGRAM, 0);
	udp->type = SOCKET_UDP_TYPE_CONN;

	return udp;
}

unsigned char * x12_socket_udp_read(x12_socket_udp_t *_udp, x12_socket_size_t *_buflen, x12_socket_udp_t *_from)
{

	if(_udp == NULL || _from == NULL)
	{
		return NULL;
	}

	x12_socket_size_t bufblock = 128;
	x12_socket_size_t buflen = bufblock;
	unsigned char *buf = (unsigned char *)malloc(buflen);
	memset(buf, 0, buflen);

	x12_socket_size_t readlen = 0;
	
	do
	{

		if(readlen == buflen)
		{
			buflen += bufblock;
			buf = realloc(buf, buflen);
			memset(buf, 0, buflen);
		}

		readlen = recvfrom(_udp->fd, buf, buflen, MSG_PEEK, (struct sockaddr *)&(_from->addr), &(_from->addrlen));	

		if(readlen < 0)
		{
			if(errno == EAGAIN || errno == EDEADLK)
			{
				continue;
			}
			else
			{
				return NULL;
			}
		}

		
	}
	while(readlen == buflen || readlen < 0);
	
	readlen = recvfrom(_udp->fd, buf, buflen, 0, (struct sockaddr *)&(_from->addr), &(_from->addrlen));
	
	*_buflen = readlen;
	return buf;
}

x12_socket_size_t x12_socket_udp_sendbroadcast(x12_socket_udp_t *_udp, unsigned char *_buf, x12_socket_size_t _buflen)
{
	if(_udp == NULL)
	{
		return -1;
	}

	x12_socket_size_t sendlen = 0;
	int sendret;

	struct sockaddr_in baddr;

	baddr.sin_family = AF_INET;
	baddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	baddr.sin_port = _udp->addr.sin_port;

	do
	{
		printf("fd %d\n", _udp->fd);
		sendret = sendto(_udp->fd, _buf + sendlen, _buflen - sendlen, 0, (struct sockaddr *)&baddr, sizeof(baddr));

		if(sendret > 0)
		{
			sendlen += sendret;
		}
		else if(sendret < 0)
		{
			if(errno == EAGAIN)
			{
				continue;
			}
			else
			{
				return sendlen;
			}
		}
		else
		{
			break;
		}
	}
	while(sendret < _buflen);

	return sendlen;
}

x12_socket_size_t x12_socket_udp_send(x12_socket_udp_t *_udp, unsigned char *_buf, x12_socket_size_t _buflen)
{
	x12_socket_size_t sendlen = 0;
	int sendret;

	if(_udp == NULL)
	{
		return -1;
	}
	
	if(_udp->type == SOCKET_UDP_TYPE_LISTEN)
	{
		sendlen = x12_socket_udp_sendbroadcast(_udp, _buf, _buflen);
		return sendlen;
	}

	do
	{
		sendret = sendto(_udp->fd, _buf + sendlen, _buflen - sendlen, 0, (struct sockaddr *)&(_udp->addr), _udp->addrlen);

		if(sendret > 0)
		{
			sendlen += sendret;
		}
		else if(sendret < 0)
		{
			if(sendret == EAGAIN)
			{
				continue;
			}
			else
			{	
				return sendret;
			}
		}
		else
		{
			break;
		}
	}
	while(sendlen < _buflen);

	return sendlen;
}

void x12_socket_udp_distory(x12_socket_udp_t *_udp)
{
	if(_udp != NULL)
	{
		free(_udp);
	}
}

char x12_socket_udp_even_add(void* _udp, void* _queue)
{
	if(!_udp || !_queue)
	{
		return -1;
	}

	x12_socket_udp_t *udp = (x12_socket_udp_t *)_udp;

	char ret = x12_listen_evens_add(udp->fd, udp, sizeof(x12_socket_udp_t), _queue, 1);

	return ret;
}

int x12_socket_udp_driver_add(void* _udp)
{
	if(!_udp)
	{
		return -1;
	}

	int driverid = 0;
	
	int dret = x12_even_drivers_add(&driverid, _udp, sizeof(x12_socket_udp_t), 1);

	if(dret < 0)
	{
		return -1;
	}

	return driverid;
}
