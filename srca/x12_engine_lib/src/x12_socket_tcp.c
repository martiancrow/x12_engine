/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 */


/*
 *x12 tcp socket套接字
 *
 *
 *作者:Crow
 *时间:2017-01-05
 *最后修改时间:2017-01-05
 */

#include "x12_socket_tcp.h"
#include <stdio.h>

char x12_socket_tcp_noblock(int _fd)
{
	int flags = fcntl(_fd, F_GETFL, 0);
	if(fcntl(_fd, F_SETFL, flags|O_NONBLOCK) < 0)
	{
		return -1;
	}

	return 0;
}

x12_socket_tcp_t* x12_socket_tcp_creat(void)
{
	struct x12_socket_tcp_s *tcp = (x12_socket_tcp_t *)malloc(sizeof(x12_socket_tcp_t));
	memset(tcp, 0, sizeof(x12_socket_tcp_t));

	tcp->addr.sin_family = AF_INET;
	tcp->addrlen = sizeof(tcp->addr);
	tcp->fd = -1;

	return tcp;
	
}

x12_socket_tcp_t* x12_socket_tcp_creat_accept(int _fd)
{
	struct x12_socket_tcp_s *tcp = (x12_socket_tcp_t *)malloc(sizeof(x12_socket_tcp_t));
	memset(tcp, 0, sizeof(x12_socket_tcp_t));

	tcp->type = SOCKET_TCP_TYPE_ACCEPT;
	
	memset(&(tcp->addr), 0, sizeof(tcp->addr));
	tcp->addrlen = sizeof(tcp->addr);
	
	tcp->fd = accept(_fd, (struct sockaddr *)&(tcp->addr), &(tcp->addrlen));

	tcp->accfd = _fd;

	if(tcp->fd < 0)
	{
		x12_socket_tcp_distory(tcp);
		return NULL;
	}


	x12_socket_tcp_noblock(tcp->fd);

	return tcp;
}

x12_socket_tcp_t* x12_socket_tcp_creat_listen(const char *_addr, int _port)
{
	struct x12_socket_tcp_s *tcp = (x12_socket_tcp_t *)malloc(sizeof(x12_socket_tcp_t));
	memset(tcp, 0, sizeof(x12_socket_tcp_t));
	
	tcp->type = SOCKET_TCP_TYPE_LISTEN;
	memset(&(tcp->addr), 0, sizeof(tcp->addr));
	tcp->addrlen = sizeof(tcp->addr);
	tcp->addr.sin_family = AF_INET;

	if(_addr == NULL)
	{
		tcp->addr.sin_addr.s_addr = htons(INADDR_ANY);
	}
	else
	{
		tcp->addr.sin_addr.s_addr = inet_addr(_addr);
	}

	tcp->addr.sin_port = htons(_port);

	tcp->fd = socket(AF_INET, SOCK_STREAM, 0);

	if(tcp->fd < 0)
	{
		x12_socket_tcp_distory(tcp);
		return NULL;
	}
	else
	{
		int opt = 1;
		setsockopt(tcp->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	}

	if(bind(tcp->fd, (struct sockaddr *)&(tcp->addr), sizeof(tcp->addr)) == -1)
	{
		x12_socket_tcp_distory(tcp);
		return NULL;
	}

	x12_socket_tcp_noblock(tcp->fd);

	if(listen(tcp->fd, SOCKET_TCP_LISTEN_QUEUE) < 0)
	{
		x12_socket_tcp_distory(tcp);
		return NULL;
	}

	return tcp;
}

x12_socket_tcp_t* x12_socket_tcp_creat_connection(const char *_addr, int _port)
{
	if(_addr == NULL || _port == 0)
	{
		return NULL;
	}

	struct x12_socket_tcp_s *tcp = (x12_socket_tcp_t *)malloc(sizeof(x12_socket_tcp_t));
	memset(tcp, 0, sizeof(x12_socket_tcp_t));
	
	tcp->type = SOCKET_TCP_TYPE_CONN;
	memset(&(tcp->addr), 0, sizeof(tcp->addr));
	tcp->addrlen = sizeof(tcp->addr);
	
	tcp->addr.sin_addr.s_addr = inet_addr(_addr);
	tcp->addr.sin_port = htons(_port);

	tcp->fd = socket(AF_INET, SOCK_STREAM, 0);

	if(tcp->fd < 0)
	{
		x12_socket_tcp_distory(tcp);
		return NULL;
	}
	
	if(connect(tcp->fd, (struct sockaddr *)&(tcp->addr), sizeof(tcp->addr)) < 0)
	{
		x12_socket_tcp_distory(tcp);
		return NULL;
	}

	return tcp;
}

unsigned char * x12_socket_tcp_read(x12_socket_tcp_t *_tcp, x12_socket_tcp_size_t *_buflen)
{
	if(_tcp == NULL || _buflen == NULL)
	{
		return NULL;
	}

	int readlen = 0;
	int buffoffset = 0;
	unsigned char *readbuff;
	int buffblocksize = 512;
	int buffsize = buffblocksize;
	readbuff = (unsigned char *)malloc(buffblocksize);

	do
	{
		if(readlen == buffblocksize)
		{
			buffsize += buffblocksize;
			readbuff = (unsigned char *)realloc(readbuff, buffsize);
			memset(readbuff + buffoffset, 0, buffblocksize);
		}

		readlen = recv(_tcp->fd, readbuff + buffoffset, buffblocksize, 0);

		if(readlen == 0)
		{
			if(readbuff != NULL)
			{
				free(readbuff);
				readbuff = NULL;
			}

			*_buflen = 0;
			return NULL;
		}
		else if(readlen < 0)
		{
			if(errno == EAGAIN)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		else
		{
			buffoffset += readlen;
		}
	}
	while(readlen == buffblocksize || readlen < 0);

	*_buflen = buffoffset;
	return readbuff;
}

x12_socket_tcp_size_t x12_socket_tcp_send(x12_socket_tcp_t *_tcp, unsigned char *_buf, x12_socket_tcp_size_t _buflen)
{
	if(_tcp == NULL || _buf == NULL)
	{
		return -1;
	}

	x12_socket_tcp_size_t sendlen = 0;
	x12_socket_tcp_size_t sendret;

	do
	{
		sendret = send(_tcp->fd, _buf + sendlen, _buflen - sendlen, 0);

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
			else if(errno == EPIPE)
			{
				return -1;
			}
			else
			{
				return -2;
			}
		}

	}
	while(sendlen < _buflen);


	return sendlen;
}

int x12_socket_tcp_close(x12_socket_tcp_t *_tcp)
{
	if(_tcp == NULL)
	{
		return -1;
	}

	int ret = close(_tcp->fd);

	return ret;
}

void x12_socket_tcp_distory(x12_socket_tcp_t *_tcp)
{
	if(_tcp != NULL)
	{
		free(_tcp);
		_tcp = NULL;
	}
}


