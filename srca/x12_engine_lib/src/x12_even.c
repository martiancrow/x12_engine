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

#include <stdio.h>
#include <unistd.h>

#include "x12.h"
#include "x12_even.h"
#include "x12_even_drivers.h"
#include "x12_even_queue.h"
#include "x12_socket_tcp.h"
#include "x12_socket_udp.h"
#include "x12_socket_raw.h"
#include "x12_msg.h"

#ifdef _OS_LINUX_

	#include "x12_epoll.h"

#else
	
	#include "x12_select.h"

#endif


static x12_listen_evens_t *levens;

char x12_listen_evens_init(void)
{
	if(levens != NULL)
	{
		return 0;
	}

	levens = malloc(sizeof(x12_listen_evens_t));
	memset(levens, 0, sizeof(x12_listen_evens_t));
	
	levens->listen_len = 0;
	levens->listen_evens = NULL;
	levens->listen_size = 0;

#ifdef _OS_LINUX_

	x12_epoll_init(0);
#else

	x12_select_init(0);
#endif

	return 0;
}

void x12_listen_evens_destroy(void)
{
	if(levens != NULL)
	{	
		if(levens->listen_evens != NULL)
		{
			free(levens->listen_evens);
			levens->listen_evens = NULL;
		}

		free(levens);
		levens = NULL;
	}
}

void x12_listen_evens_forever(void)
{

#ifdef _OS_LINUX_

	x12_epoll_loopforever();
#else
	
	x12_select_loopforever();
#endif
}

char x12_listen_evens_add(int _fd, void* _data, even_size_t _datalen, void* _dataqueue, unsigned short _eventype)
{
	if(levens == NULL)
	{
		return -1;
	}

	struct x12_listen_even_s even;
	
	even.fd = _fd;
	even.data = _data;
	even.datalen = _datalen;
	even.dataqueue = _dataqueue;
	even.eventype = _eventype;

	int blocksize = 128;
	int datasize = levens->listen_len * sizeof(x12_listen_even_t);

	if(levens->listen_evens == NULL)
	{
		levens->listen_size += blocksize;
		levens->listen_evens = malloc(levens->listen_size);
	}
	else
	{
		if(datasize + sizeof(x12_listen_even_t) > levens->listen_size)
		{
			levens->listen_size += blocksize;
			levens->listen_evens = realloc(levens->listen_evens, levens->listen_size);
		}	
	}
	memcpy(levens->listen_evens + levens->listen_len, &even, sizeof(x12_listen_even_t));
	//memcpy((levens->listen_evens)[levens->listen_len], &even, sizeof(x12_listen_even_t));
	
	levens->listen_len++;

#ifdef _OS_LINUX_

	x12_epoll_addlisten(_fd);

#else

	x12_select_addlisten(_fd);

#endif

	
	return 0;
}

char x12_listen_evens_del(int _fd)
{
	if(levens == NULL)
	{
		return -1;
	}

	even_size_t eindex = x12_listen_evens_search(_fd);

	if(eindex == -1)
	{
		return 0;
	}

	if(eindex + 1 == levens->listen_len)
	{
		levens->listen_len -= 1;
		//levens->listen_evens = realloc(levens->listen_evens, sizeof(x12_listen_even_t) * levens->listen_len);
	}
	else
	{
		memcpy(levens->listen_evens + eindex, levens->listen_evens + (levens->listen_len - 1), sizeof(x12_listen_even_t));
		levens->listen_len -= 1;
		//levens->listen_evens = realloc(levens->listen_evens, sizeof(x12_listen_even_t) * levens->listen_len);
	}

#ifdef _OS_LINUX_
	
	x12_epoll_dellisten(_fd);
#else

	x12_select_dellisten(_fd);
#endif

	return 0;
}

char x12_even_callback(int _fd)
{
	even_size_t eindex = x12_listen_evens_search(_fd);
	x12_listen_even_t *even = NULL;
	
	if(eindex != -1)
	{
		even = &(levens->listen_evens)[eindex];
	}

	if(even != NULL)
	{
		if(even->eventype != 0)
		{
			if(even->eventype == 1 || even->eventype == 2)
			{
				
				unsigned char *msg;
				int msglen;

				x12_socket_udp_t *from = x12_socket_udp_creat_accept(_fd);
				msg = x12_socket_udp_read(even->data, &msglen, from);

				int id;
				int dret = x12_even_drivers_add(&id, from, sizeof(*from), even->eventype);	

			
				if(msg != NULL && dret == 0)
				{
					int ret = x12_evens_queue_enter(even->dataqueue, msg, msglen, id, even->eventype);
					if(ret == -1)
					{
						printf("enter is fail\n");
					}
				}

			}
			else if(even->eventype == 11)
			{
				int id;

				x12_socket_tcp_t *client = x12_socket_tcp_creat_accept(_fd);
				x12_listen_evens_add(client->fd, (unsigned char *)client, sizeof(x12_socket_tcp_t), even->dataqueue, 13);
				x12_even_drivers_add(&id, client, sizeof(*client), 13);			
			}
			else if(even->eventype == 12)
			{
				unsigned char *msg;
				int msglen;
				int id;
				
				x12_even_drivers_add(&id, even->data, sizeof(x12_socket_tcp_t), even->eventype);
				
				msg = x12_socket_tcp_read(even->data, &msglen);
				
				if(msg != NULL)
				{
					x12_evens_queue_enter(even->dataqueue, msg, msglen, id, even->eventype);
				}
				else
				{
					if(msglen == 0)
					{
#ifdef _OS_LINUX_
						x12_epoll_dellisten(even->fd);
#else
						x12_select_dellisten(even->fd);
#endif
						x12_socket_tcp_close(even->data);
						x12_even_drivers_del(id);
					}
				}
			}
			else if(even->eventype == 13)
			{
				unsigned char *msg;
				int msglen;
				int id;

				x12_even_drivers_add(&id, even->data, sizeof(x12_socket_tcp_t), even->eventype);

				msg = x12_socket_tcp_read(even->data, &msglen);

				if(msg != NULL)
				{
					x12_evens_queue_enter(even->dataqueue, msg, msglen, id, even->eventype);
				}
				else
				{
					if(msglen == 0)
					{
#ifdef _OS_LINUX_
						x12_epoll_dellisten(even->fd);
#else
						x12_select_dellisten(even->fd);
#endif
						x12_socket_tcp_close(even->data);
						x12_even_drivers_del(id);
					}
				}
			}
			else if(even->eventype == 21)
			{

#ifdef _OS_LINUX_
				
				unsigned char *msg;
				int msglen;

				msg = x12_socket_raw_read(even->data, &msglen);

				if(msg != NULL)
				{
					x12_evens_queue_enter(even->dataqueue, msg, msglen, 0, even->eventype);
					free(msg);
					msg = NULL;
				}

#endif

			}
		}
		else
		{
			char msg[50];
			memset(msg, 0, sizeof(msg));
			int readlen = read(even->fd, msg, sizeof(msg));
			msg[readlen - 1] = 0;
			
			x12_evens_queue_enter(even->dataqueue, (unsigned char *)msg, readlen, -1, even->eventype);

			//if(connection != NULL)
			//{
			//	x12_socket_udp_send(connection, (unsigned char *)msg, strlen(msg) + 1);
			//}
			

			//x12_msg_send(0, (unsigned char *)msg, strlen(msg) + 1);
	
			//if(tcp_client != NULL)
			//{
			//	x12_socket_tcp_send(tcp_client, (unsigned char *)msg, strlen(msg) + 1);
			//}

		}
		
		/*
		if(even->eventype == 1)
		{
			unsigned char *msg;
			intmsglen;

			x12_socket_udp_t *from = x12_socket_udp_creat_accept(_fd);
			msg = x12_socket_udp_read(even->data, &msglen, from);

			int id;
			int dret = x12_even_drivers_add(&id, from, sizeof(*from), from->type);

			if(msg != NULL)
			{
				printf("driverid:%d - %s:%d %s\n", id, inet_ntoa((from->addr).sin_addr), (from->addr).sin_port, msg);
				x12_socket_udp_send(from, (unsigned char *)msg, msglen);
				free(msg);
				msg = NULL;
			}
			else
			{
				printf("read fail\n");
			}
		}
		else if(even->eventype == 2)
		{
			
			unsigned char *msg;
			int msglen;

			x12_socket_udp_t *from = x12_socket_udp_creat();
			msg = x12_socket_udp_read(even->data, &msglen, from);
			
			if(msg != NULL)
			{
				printf("%s:%d %s\n", inet_ntoa((from->addr).sin_addr), (from->addr).sin_port, msg);

				free(msg);
				msg = NULL;
			}
			else
			{
				//printf("read error code is %d\n", msgsize);
			}
			x12_socket_udp_distory(from);

		}
		else if(even->eventype == 0)
		{
			
			char msg[50];
			memset(msg, 0, sizeof(msg));

			int readlen = read(even->fd, msg, sizeof(msg));

			msg[readlen - 1] = 0;
			
			if(connection != NULL)
			{
				x12_socket_udp_send(connection, (unsigned char *)msg, strlen(msg) + 1);
			}

		}
		*/
	}

	return 0;
}

even_size_t x12_listen_evens_search(int _fd)
{
	if(levens == NULL)
	{
		return -1;
	}

	even_size_t eindex = 0;

	x12_listen_even_t *item;

	even_size_t i;
	
	for(i = 0; i < levens->listen_len; i++)
	{
		item = NULL;
		item = &(levens->listen_evens)[i];

		if(item != NULL)
		{

			if(item->fd == _fd)
			{
				eindex = i;
				break;
			}
		}
	}

	return eindex;
}

