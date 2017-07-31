/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */


/*
 *x12 队列,建议放入等长数据,队列本身不记录数据长度
 *
 *
 *作者:Crow
 *时间:2016-07-17
 *最后修改时间:2016-07-17
 */

#include "x12.h"
#include "x12_data_queue.h"
#include <stdio.h>

data_queue_errno_t  dq_errno;

x12_data_queue_t* x12_data_queue_creat(dq_size_t _blocksize, dq_size_t _cleansize, data_queue_cleanpre_funp _funp)
{
	struct x12_data_queue_s *queue = (x12_data_queue_t *)malloc(sizeof(x12_data_queue_t));
	memset(queue, 0, sizeof(x12_data_queue_t));

	x12_pthread_mutexattr_t attr;

	if(x12_pthread_mutexattr_init(&attr) != X12_PTHREAD_LOCK_OK)
	{
		dq_errno = lock_fail;
		return NULL;
	}

	if(x12_pthread_mutexattr_settype(&attr, X12_PTHREAD_MUTEX_ERRORCHECK) != X12_PTHREAD_LOCK_OK)
	{
		dq_errno = lock_fail;
		return NULL;
	}

	if(x12_pthread_mutex_init(&(queue->mutex), &attr) != X12_PTHREAD_LOCK_OK)
	{
		dq_errno = lock_fail;
		return NULL;
	}

	if(x12_pthread_cond_init(&(queue->can_read), NULL) != X12_PTHREAD_LOCK_OK)
	{
		dq_errno = lock_fail;
		x12_pthread_mutex_destroy(&(queue->mutex));
		return NULL;
	}

	if(x12_pthread_cond_init(&(queue->can_write), NULL) != X12_PTHREAD_LOCK_OK)
	{
		dq_errno = lock_fail;
		x12_pthread_cond_destroy(&(queue->can_read));
		x12_pthread_mutex_destroy(&(queue->mutex));
		return NULL;
	}
	
	if(_blocksize == 0)
	{
		queue->blocksize = DATA_QUEUE_DEF_BLOCK_SIZE;
	}
	else
	{

		if(_blocksize % 2 != 0)
		{
			dq_errno = blocksizeerr;
			x12_data_queue_destroy(queue);
			return NULL;
		}

		queue->blocksize = _blocksize;
	}

	if(_cleansize == 0)
	{
		queue->cleansize = DATA_QUEUE_DEF_CLEAN_SIZE;
	}
	else
	{
		queue->cleansize = _cleansize;
	}

	queue->size = queue->blocksize;
	queue->dataoffset = 0;
	queue->readhead = 0;
	queue->datalen = 0;
	queue->data = (unsigned char *)malloc(queue->size);
	memset(queue->data, 0, queue->size);
	queue->cleanprefun = _funp;

	return queue;
}

void x12_data_queue_destroy(x12_data_queue_t *_queue)
{
	if(_queue != NULL)
	{
		if(_queue->cleanprefun != NULL)
		{
			_queue->dataoffset = _queue->datalen;
		
			unsigned char dataclean[_queue->dataoffset];
			memset(dataclean, 0, sizeof(dataclean));
			memcpy(dataclean, _queue->data, sizeof(dataclean));
		
			_queue->cleanprefun(dataclean, sizeof(dataclean));
		}


		if(_queue->data != NULL)
		{
			free(_queue->data);
			_queue->data = NULL;
		}

		x12_pthread_cond_destroy(&(_queue->can_read));
		x12_pthread_cond_destroy(&(_queue->can_write));
		x12_pthread_mutex_destroy(&(_queue->mutex));

		free(_queue);
		_queue = NULL;
	}
}

dq_size_t x12_data_queue_enqueue(x12_data_queue_t *_queue, unsigned char *_data, dq_size_t _datalen)
{
	if(_queue == NULL || _data == NULL)
	{
		dq_errno = args_null;
		return DATA_QUEUE_ARGS_NULL;
	}
	
	int lockret = x12_pthread_mutex_lock(&(_queue->mutex));

	
	if(lockret != X12_PTHREAD_LOCK_OK)
	{
		dq_errno = lock_fail;
		return DATA_QUEUE_LOCK_FAIL;
	}
	
	dq_size_t datalen = _queue->datalen;
	dq_size_t nsize = data_queue_align(datalen + _datalen, _queue->blocksize);
	dq_size_t dataoffset = _queue->dataoffset;
	
	if(datalen != 0)
	{
		if(datalen % _datalen != 0)
		{
			dq_errno = endatasizeerr;
			return -1;
		}
	}

	if(nsize == 0)
	{
		nsize = _queue->size;
	}

	if(nsize > _queue->size)
	{
		if(nsize > _queue->cleansize)
		{
			//callback pretreatment fun
			if(_queue->cleanprefun != NULL)
			{
				unsigned char dataclean[dataoffset];
				memset(dataclean, 0, sizeof(dataclean));
				memcpy(dataclean, _queue->data, sizeof(dataclean));
				
				//_queue->cleanprefun(_queue->data, dataoffset);
				_queue->cleanprefun(dataclean, sizeof(dataclean));
			
			}
			

			datalen = datalen - dataoffset;
			unsigned char datatmp[datalen];
			memset(datatmp, 0, sizeof(datatmp));

			memcpy(datatmp, _queue->data + dataoffset, sizeof(datatmp));
			nsize = data_queue_align(datalen +  _datalen, _queue->blocksize);

			dataoffset = 0;

			_queue->data = (unsigned char *)realloc(_queue->data, nsize);
			memset(_queue->data, 0, sizeof(datatmp));
			memcpy(_queue->data, datatmp, sizeof(datatmp));
		
		}
		else
		{
			_queue->data = (unsigned char *)realloc(_queue->data, nsize);
		}
	}


	memcpy(_queue->data + datalen, _data, _datalen);
	_queue->datalen = datalen + _datalen;
	_queue->size = nsize;
	_queue->dataoffset = dataoffset;

	x12_pthread_mutex_unlock(&(_queue->mutex));
	x12_pthread_cond_signal(&(_queue->can_read));

	return _datalen;
}

dq_size_t x12_data_queue_dequeue(x12_data_queue_t *_queue, unsigned char *_buf, dq_size_t _readlen)
{
	if(_queue == NULL || _buf == NULL)
	{
		dq_errno = args_null;
		return DATA_QUEUE_ARGS_NULL;
	}

	int lockret = x12_pthread_mutex_lock(&(_queue->mutex));
	
	if(lockret != X12_PTHREAD_LOCK_OK && lockret != X12_PTHREAD_LOCK_EDEADLK && lockret != X12_PTHREAD_LOCK_EAGAIN)
	{
		dq_errno = lock_fail;
		return DATA_QUEUE_LOCK_FAIL;
	}

	while(x12_data_queue_isempty(_queue))
	{
		x12_pthread_cond_wait(&(_queue->can_read), &(_queue->mutex));
	}

	_queue->readhead = _queue->dataoffset;

	memset(_buf, 0, _readlen);
	memcpy(_buf, _queue->data + _queue->dataoffset, _readlen);
	_queue->dataoffset += _readlen;

	return _readlen;
}

short x12_data_queue_lock(x12_data_queue_t *_queue)
{
	if(_queue == NULL)
	{
		dq_errno = args_null;
		return DATA_QUEUE_ARGS_NULL;
	}

	int lockret = x12_pthread_mutex_lock(&(_queue->mutex));
	
	if(lockret != X12_PTHREAD_LOCK_OK && lockret != X12_PTHREAD_LOCK_EDEADLK && lockret != X12_PTHREAD_LOCK_EAGAIN)
	{
		dq_errno = lock_fail;
		return DATA_QUEUE_LOCK_FAIL;
	}

	while(x12_data_queue_isempty(_queue))
	{
		x12_pthread_cond_wait(&(_queue->can_read), &(_queue->mutex));
	}

	return 0;
}

short x12_data_queue_unlock(x12_data_queue_t *_queue)
{
	if(_queue == NULL)
	{
		dq_errno = args_null;
		return DATA_QUEUE_ARGS_NULL;
	}

	
	int lockret = x12_pthread_mutex_lock(&(_queue->mutex));
	
	if(lockret != X12_PTHREAD_LOCK_OK && lockret != X12_PTHREAD_LOCK_EDEADLK && lockret != X12_PTHREAD_LOCK_EAGAIN)
	{
		dq_errno = lock_fail;
		return DATA_QUEUE_LOCK_FAIL;
	}

	
	if(lockret == X12_PTHREAD_LOCK_OK)
	{
		dq_errno = lock_none;
		x12_pthread_mutex_unlock(&(_queue->mutex));
		return DATA_QUEUE_LOCK_NONE;
	}
	
	
	x12_pthread_mutex_unlock(&(_queue->mutex));

	return 0;
}

dq_size_t x12_data_queue_reset(x12_data_queue_t *_queue, dq_size_t _redatalen)
{
	
	if(_queue == NULL)
	{
		dq_errno = args_null;
		return DATA_QUEUE_ARGS_NULL;
	}
	
	int lockret = x12_pthread_mutex_lock(&(_queue->mutex));

	if(lockret != X12_PTHREAD_LOCK_OK && lockret != X12_PTHREAD_LOCK_EDEADLK && lockret != X12_PTHREAD_LOCK_EAGAIN)
	{
		dq_errno = lock_fail;
		return DATA_QUEUE_LOCK_FAIL;
	}

	if(lockret == X12_PTHREAD_LOCK_OK)
	{
		dq_errno = lock_none;
		x12_pthread_mutex_unlock(&(_queue->mutex));
		return DATA_QUEUE_LOCK_NONE;
	}
	
	dq_size_t resethead = _queue->dataoffset - _redatalen;

	if(resethead < _queue->readhead)
	{
		_redatalen = _redatalen - (_queue->readhead - resethead);
	}

	if(_redatalen > _queue->dataoffset)
	{
		_redatalen = _queue->dataoffset;
	}

	_queue->dataoffset -= _redatalen;

	//coap_data_queue_unlock(_queue);

	return _redatalen;
}

dq_size_t x12_data_queue_getsize(x12_data_queue_t *_queue)
{
	if(_queue == NULL)
	{
		dq_errno = args_null;
		return DATA_QUEUE_ARGS_NULL;
	}

	return _queue->size;
}

dq_size_t x12_data_queue_getdatalen(x12_data_queue_t *_queue)
{
	if(_queue == NULL)
	{
		dq_errno = args_null;
		return DATA_QUEUE_ARGS_NULL;
	}

	return _queue->datalen;
}

char x12_data_queue_isempty(x12_data_queue_t *_queue)
{
	if(_queue == NULL)
	{
		dq_errno = args_null;
		return DATA_QUEUE_ARGS_NULL;
	}

	if(_queue->datalen > _queue->dataoffset && _queue->data != NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
