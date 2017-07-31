/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */


/*
 *x12 epoll 事件监控头文件
 *
 *
 *作者:刘晓龙
 *时间:2017-06-14
 *最后修改时间:2017-06-14
 */

#ifdef _OS_LINUX_

#include "x12_epoll.h"
#include "x12.h"

static fd_set master_read_fd_set;		//全局读fd
static fd_set work_read_fd_set;			//工作读fd
static struct timeval tv;				//超时时间
static int max_fd;						//最大监听fd

static int epfd;								//epoll专用fd
static int timeout = 5000;						//超时时间
static int max_size = MAX_SIZE;					//最大监听数量
static x12_pthread_t pid;						//监听线程id
static struct epoll_event ev;					//添加事件
static struct epoll_event events[MAX_SIZE];		//回调事件

char x12_epoll_init(unsigned int _timeout)
{	
	//屏蔽掉SIGPIPE信号,防止对端关闭链接时进程退出
	signal(SIGPIPE, SIG_IGN);

	if(_timeout != 0)
	{
		timeout = _timeout;
	}
	
	epfd = epoll_create(max_size);

	x12_pthread_create(&pid, NULL, (void *)x12_epoll_loop, NULL);
	
	return 0;
	
}

void x12_epoll_loop(void)
{
	
	int i;
	int nfds = 0;	//处理事件数目

	while(1)
	{
		nfds = epoll_wait(epfd, events, max_size, timeout);

		if(nfds == 0)
		{
			//第一个超时后才开始监听
			//超时回调
			//printf("select time out!\n");
		}
		else if(nfds > 0)
		{
			//触发事件
			for(i = 0; i < nfds; i++)
			{
				//事件回调
				x12_even_callback(events[i].data.fd);
			}
		}
		else
		{
			//错误回调
			printf("epoll is error!\n");
		}
	}

	pthread_exit(NULL);
}

void x12_epoll_loopforever(void)
{
	x12_pthread_join(pid, NULL);
}

char x12_epoll_addlisten(int _fd)
{
	ev.data.fd = _fd;
	ev.events = EPOLLIN | EPOLLET;

	epoll_ctl(epfd, EPOLL_CTL_ADD, _fd, &ev);

    return 0;
}

char x12_epoll_dellisten(int _fd)
{
    
	ev.data.fd = _fd;
	ev.events = EPOLLIN | EPOLLET;

	epoll_ctl(epfd, EPOLL_CTL_DEL, _fd, &ev);
	
	return 0;
}


#endif
