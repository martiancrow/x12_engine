/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */


/*
 *
 *coap engine 事件数据队列(底层事件封装)
 *
 *作者:Crow
 *时间:2016-11-23
 *最后修改时间:2016-11-23
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "x12_even_drivers.h"
#include "x12_even.h"
#include "x12_even_queue.h"
#include "x12_data_queue.h"

//static x12_data_queue_t *msgqueue = NULL;
//static x12_evens_queue_pthread_list_t *plist = NULL;

short freedata(unsigned char *_cleandata, dq_size_t _datalen)
{
	
	if(_cleandata == NULL)
	{
		return 0;
	}

	x12_evens_queue_item_t *evenqueue = (x12_evens_queue_item_t *)_cleandata;

	dq_size_t count = _datalen / sizeof(x12_evens_queue_item_t);
	int i;

	x12_evens_queue_item_t *item = NULL;

	for(i = 0; i < count; i++)
	{
		item = NULL;

		item = &evenqueue[i];

		if(item != NULL)
		{
			if(item->data != NULL)
			{
				free(item->data);
				item->data = NULL;
			}
		}
	}


	return 0;
}

x12_evens_queue_t* x12_evens_queue_creat(void)
{
	struct x12_evens_queue_s *queue = (x12_evens_queue_t *)malloc(sizeof(x12_evens_queue_t));

	memset(queue, 0, sizeof(x12_evens_queue_t));

	if(queue->msgqueue == NULL)
	{
		queue->msgqueue = x12_data_queue_creat(512, 128, &freedata); 
	}

	if(queue->plist == NULL)
	{
		queue->plist = (x12_evens_queue_pthread_list_t *)malloc(sizeof(x12_evens_queue_pthread_list_t));
		queue->plist->pthreadlist = NULL;
		queue->plist->count = 0;
	}
	
	return queue;
}

void x12_evens_queue_destroy(x12_evens_queue_t *_queue)
{
	if(_queue)
	{
		int i;

		if(_queue->plist != NULL)
		{
			if(_queue->plist->pthreadlist != NULL)
			{
				x12_evens_queue_pthread_data_t *item = NULL;
				for(i = 0; i < _queue->plist->count; i++)
				{
					item = &(_queue->plist->pthreadlist)[i];

					if(item != NULL)
					{
						item->isexit = 1;
					}
				}

				sleep(1);

				free(_queue->plist);
				_queue->plist = NULL;
			}
		}

		if(_queue->msgqueue != NULL)
		{
			x12_data_queue_destroy(_queue->msgqueue);
		}

	}
}

char x12_evens_queue_enter(x12_evens_queue_t *_queue, unsigned char *_data, even_queue_size_t _datalen, int _driverid, unsigned short _type)
{
	if(!_queue)
	{
		return -1;
	}

	if(_queue->msgqueue == NULL)
	{
		return -1;
	}

	x12_evens_queue_item_t *even = (x12_evens_queue_item_t *)malloc(sizeof(x12_evens_queue_item_t));
	memset(even, 0, sizeof(x12_evens_queue_item_t));

	even->data = (unsigned char *)malloc(_datalen);
	memset(even->data, 0, _datalen);
	memcpy(even->data, _data, _datalen);

	even->datalen = _datalen;
	even->evendriverid = _driverid;
	even->eventype = _type;

	dq_size_t ret = x12_data_queue_enqueue(_queue->msgqueue, (unsigned char *)even, sizeof(x12_evens_queue_item_t));
	
	if(ret < 0)
	{
		return -1;
	}

	free(even);
	even = NULL;

	return 0;
}

char x12_evens_queue_denter(x12_evens_queue_t *_queue, x12_evens_queue_item_t *_even)
{
	if(!_queue)
	{
		return -1;
	}

	if(_queue->msgqueue == NULL || _even == NULL)
	{
		return -1;
	}

	dq_size_t rlen = x12_data_queue_dequeue(_queue->msgqueue, (unsigned char *)_even, sizeof(x12_evens_queue_item_t));

	if(rlen < 0)
	{
		return -1;
	}

	return 0;
}

char x12_evens_queue_giveupdenter(x12_evens_queue_t *_queue, int _count)
{

	if(!_queue)
	{
		return -1;
	}
	
	if(_queue->msgqueue == NULL)
	{
		return -1;
	}

	dq_size_t rlen = x12_data_queue_reset(_queue->msgqueue, _count * sizeof(x12_evens_queue_item_t));
	
	if(rlen < 0)
	{
		return -1;
	}

	return 0;
}

void x12_evens_queue_pthread_loop(x12_evens_queue_pthread_data_t *_data)
{
	if(_data != NULL)
	{
		x12_evens_queue_willread_funp cbfunp = _data->willreadcallback;
		x12_evens_queue_t *queue = _data->queue;

		while(_data->isexit == 0)
		{
			if(queue != NULL)
			{
				short retlock = x12_data_queue_lock(queue->msgqueue);

				if(retlock == 0)
				{

					if(cbfunp != NULL)
					{
						cbfunp(queue);
						x12_data_queue_unlock(queue->msgqueue);
					}
					else
					{
						x12_data_queue_unlock(queue->msgqueue);
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	x12_pthread_exit(NULL);
}

x12_pthread_t x12_evens_queue_pthread_creat(x12_evens_queue_t *_queue, x12_evens_queue_willread_funp _funcp)
{
	if(!_queue)
	{
		return 0;
	}

	if(_funcp == NULL && _queue->plist == NULL)
	{
		return 0;
	}

	x12_evens_queue_pthread_data_t *pthread = (x12_evens_queue_pthread_data_t *)malloc(sizeof(x12_evens_queue_pthread_data_t));
	memset(pthread, 0, sizeof(x12_evens_queue_pthread_data_t));

	pthread->isexit = 0;
	pthread->queue = _queue;
	pthread->willreadcallback = _funcp;

	x12_pthread_create(&(pthread->pid), NULL, (void *)x12_evens_queue_pthread_loop, pthread);
	
	even_queue_size_t count = _queue->plist->count;

	if(_queue->plist->count == 0)
	{
		count += 1;
		_queue->plist->pthreadlist = (x12_evens_queue_pthread_data_t *)malloc(sizeof(x12_evens_queue_pthread_data_t));
		memcpy(_queue->plist->pthreadlist + (sizeof(x12_evens_queue_pthread_data_t) * _queue->plist->count), pthread, sizeof(x12_evens_queue_pthread_data_t));
		_queue->plist->count = count;
	}
	else
	{
		count += 1;
		_queue->plist->pthreadlist = (x12_evens_queue_pthread_data_t *)realloc(_queue->plist->pthreadlist, sizeof(x12_evens_queue_pthread_data_t) * count);
		memcpy(_queue->plist->pthreadlist + (sizeof(x12_evens_queue_pthread_data_t) * _queue->plist->count), pthread, sizeof(x12_evens_queue_pthread_data_t));
		_queue->plist->count = count;
	}

	return pthread->pid;
}

void coap_evens_queue_pthread_loopforever(x12_pthread_t _pid)
{
	x12_pthread_join(_pid, NULL);
}

void x12_evens_queue_pthread_finish(x12_evens_queue_t *_queue, x12_pthread_t _pid)
{

	if(_queue)
	{
		if(_queue->plist != NULL)
		{
			even_queue_size_t i;
			x12_evens_queue_pthread_data_t *item = NULL;

			for(i = 0; i < _queue->plist->count; i++)
			{
				item = &(_queue->plist->pthreadlist)[i];
			
				if(item != NULL)
				{
					if(item->pid == _pid)
					{
						break;
					}
				}

				item = NULL;
			}

			if(i < _queue->plist->count)
			{
				if(i != _queue->plist->count - 1)
				{
					memcpy(&(_queue->plist->pthreadlist)[i], &(_queue->plist->pthreadlist)[_queue->plist->count - 1], sizeof(x12_evens_queue_pthread_data_t));
				}

				_queue->plist->pthreadlist = (x12_evens_queue_pthread_data_t *)realloc(_queue->plist->pthreadlist, sizeof(x12_evens_queue_pthread_data_t) * _queue->plist->count);
			}

		}
	
	}
}

char x12_evens_queue_isempty(x12_evens_queue_t *_queue)
{
	return x12_data_queue_isempty(_queue->msgqueue);
}
