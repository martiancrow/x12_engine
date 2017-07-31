/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 select 事件监控头文件
 *
 *
 *作者:Crow
 *时间:2016-08-20
 *最后修改时间:2016-08-20
 */

#include "x12_select.h"
#include "x12_pthread.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "x12.h"
#include "x12_even.h"

static fd_set master_read_fd_set;		//全局读fd
static fd_set work_read_fd_set;			//工作读fd
static struct timeval tv;				//超时时间
static int max_fd;						//最大监听fd
static x12_pthread_t pid;				//监听线程id

char x12_select_init(unsigned int _timeout)
{	
	//屏蔽掉SIGPIPE信号,防止对端关闭链接时进程退出
	signal(SIGPIPE, SIG_IGN);
	
	if(_timeout != 0)
	{
		tv.tv_sec = _timeout;
		tv.tv_usec = _timeout * 1000;
	}
	else
	{
		tv.tv_sec = X12_SELECT_DEF_TIMEOUT;
		tv.tv_usec = X12_SELECT_DEF_TIMEOUT * 1000;
	}
	
	
	FD_ZERO(&master_read_fd_set);
	
	max_fd = -1;

	x12_pthread_create(&pid, NULL, (void *)x12_select_loop, NULL);
	
	return 0;
	
}

void x12_select_loop(void)
{
	int fd;
	int sel_result;	
	struct timeval sel_timeout;
	
	while(1)
	{
		sel_timeout = tv;
		FD_ZERO(&work_read_fd_set);
		work_read_fd_set = master_read_fd_set;
		
		sel_result = select(max_fd + 1, &work_read_fd_set, NULL, NULL, &sel_timeout);

		if(sel_result == 0)
		{
			//第一个超时后才开始监听
			//select超时回调
			//printf("select time out!\n");
		}
		else if(sel_result > 0)
		{
			//触发事件
			for(fd = 0; fd < max_fd + 1; fd++)
			{
				if(FD_ISSET(fd, &work_read_fd_set))
				{

					//事件回调
					x12_even_callback(fd);

					/*				
					if(server != NULL && fd == server->fd)
					{
						char msg[50];
						memset(msg, 0, sizeof(msg));
						
						int msgsize;
						coap_socket_udp_t *from = coap_socket_udp_creat(NULL, 0);
					
						msgsize = coap_socket_udp_read(server, (unsigned char *)msg, sizeof(msg), from);

						if(msgsize >= 0)
						{
							printf("%s:%d %s\n", inet_ntoa((from->addr).sin_addr), (from->addr).sin_port, msg);
							coap_socket_udp_send(server, (unsigned char *)msg, sizeof(msg), from);
						}
						else
						{
							printf("read error code is %d\n", msgsize);
						}

					}
					else if(connection != NULL && fd == connection->fd)
					{
						char msg[50];
						memset(msg, 0, sizeof(msg));

						int msgsize;

						coap_socket_udp_t *from = coap_socket_udp_creat(NULL, 0);
					
						msgsize = coap_socket_udp_read(connection, (unsigned char *)msg, sizeof(msg), from);

						if(msgsize >= 0)
						{
							printf("%s:%d %s\n", inet_ntoa((from->addr).sin_addr), (from->addr).sin_port, msg);
						}
						else
						{
							//printf("read error code is %d\n", msgsize);

						}

						coap_socket_udp_distory(from);

					}
					else if(fd == 0 && connection != NULL)
					{
						char msg[50];
						memset(msg, 0, sizeof(msg));

						int readlen = read(fd, msg, sizeof(msg));

						msg[readlen - 1] = 0;

						coap_socket_udp_send(connection, (unsigned char *)msg, strlen(msg) + 1, connection);

					}
					*/
				}
				
			}
		}
		else
		{
			//错误回调
			printf("select is error!\n");
		}
	}

	pthread_exit(NULL);
}

void x12_select_loopforever(void)
{
	x12_pthread_join(pid, NULL);
}

char x12_select_addlisten(int _fd)
{
	FD_SET(_fd, &master_read_fd_set);
	
	if (max_fd < _fd) 
	{
		max_fd = _fd;
	}

    return 0;
}

char x12_select_dellisten(int _fd)
{
	FD_CLR(_fd, &master_read_fd_set);
    
	return 0;
}







