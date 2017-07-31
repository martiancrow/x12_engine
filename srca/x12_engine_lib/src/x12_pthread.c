/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */


/*
 *coap engine 线程封装
 *
 *
 *作者:Crow
 *时间:2016-07-17
 *最后修改时间:2016-07-17
 */

#include "x12_pthread.h"

int x12_pthread_mutexattr_init(x12_pthread_mutexattr_t *_attr)
{
	return pthread_mutexattr_init(_attr);
}

int x12_pthread_mutexattr_settype(x12_pthread_mutexattr_t *_attr, int _type)
{
	return pthread_mutexattr_settype(_attr, _type);
}

int x12_pthread_mutex_init(x12_pthread_mutex_t *_mutex, const x12_pthread_mutexattr_t *_attr)
{
	return pthread_mutex_init(_mutex, _attr);
}

int x12_pthread_mutex_destroy(x12_pthread_mutex_t *_mutex)
{
	return pthread_mutex_destroy(_mutex);
}

int x12_pthread_cond_init(x12_pthread_cond_t *_cond, const x12_pthread_condattr_t *_attr)
{
	return pthread_cond_init(_cond, _attr);
}

int x12_pthread_cond_destroy(x12_pthread_cond_t *_cond)
{
	return pthread_cond_destroy(_cond);
}

int x12_pthread_mutex_lock(x12_pthread_mutex_t *_mutex)
{
	return pthread_mutex_lock(_mutex);
}

int x12_pthread_mutex_trylock(x12_pthread_mutex_t *_mutex)
{
	return pthread_mutex_trylock(_mutex);
}

int x12_pthread_mutex_unlock(x12_pthread_mutex_t *_mutex)
{
	return pthread_mutex_unlock(_mutex);
}

int x12_pthread_cond_signal(x12_pthread_cond_t *_cond)
{
	return pthread_cond_signal(_cond);
}

int x12_pthread_cond_wait(x12_pthread_cond_t *_cond, x12_pthread_mutex_t *_mutex)
{
	return pthread_cond_wait(_cond, _mutex);
}

int x12_pthread_create(x12_pthread_t *_thread, x12_pthread_attr_t *_attr, void* (*_start_rtn)(void*), void* _arg)
{
	return pthread_create(_thread, _attr, _start_rtn, _arg);
}

int x12_pthread_join(x12_pthread_t _thread, void **_retval)
{
	return pthread_join(_thread, _retval);
}

void x12_pthread_exit(void* _retval)
{
	pthread_exit(_retval);
}






