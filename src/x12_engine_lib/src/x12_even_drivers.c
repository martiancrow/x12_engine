/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 engine 事件设备列表
 *
 *
 *作者:Crow
 *时间:2016-11-13
 *最后修改时间:2016-11-13
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "x12_even_drivers.h"
#include "x12_even.h"

static x12_even_driverlist_t *driverlist = NULL;

char x12_even_drivers_init(void)
{
	if(driverlist == NULL)
	{
		driverlist = (x12_even_driverlist_t *)malloc(sizeof(x12_even_driverlist_t));
		
		if(x12_pthread_mutex_init(&(driverlist->mutex), NULL) != X12_PTHREAD_LOCK_OK)
		{
			free(driverlist);
			driverlist = NULL;
			return -1;
		}

		driverlist->listsize = DRIVERLIST_DEF_BLOCK_SIZE;
		driverlist->driverlist = (x12_even_driver_t *)malloc(driverlist->listsize);
		driverlist->nullid = (int *)malloc(driverlist->listsize);
		driverlist->blocksize = DRIVERLIST_DEF_BLOCK_SIZE;
		driverlist->dataoffset = 0;
		driverlist->drivercount = 0;
		driverlist->nullidoffset = 0;
		driverlist->nullidsize = DRIVERLIST_DEF_BLOCK_SIZE;
	}

	return 0;
}

void x12_even_drivers_destroy(void)
{
	if(driverlist != NULL)
	{
		if(driverlist->driverlist != NULL)
		{
			free(driverlist->driverlist);
			driverlist->driverlist = NULL;
		}

		if(driverlist->nullid != NULL)
		{
			free(driverlist->nullid);
			driverlist->nullid = NULL;
		}

		free(driverlist);
		driverlist = NULL;
	}
}

x12_even_driver_t* x12_even_drivers_search(int _id)
{
	if(driverlist == NULL)
	{
		return NULL;
	}

	if(driverlist->dataoffset / sizeof(x12_even_driver_t) < _id)
	{
		return NULL;
	}

	return &((driverlist->driverlist)[_id]);
}

char x12_even_drivers_add(int *_id, void* _linkdata, int _datalen, unsigned short _eventype)
{
	if(driverlist == NULL)
	{
		return -1;
	}
	
	int lockret = x12_pthread_mutex_lock(&(driverlist->mutex));

	
	if(lockret != X12_PTHREAD_LOCK_OK)
	{
		return -1;
	}
	
	
	even_drivers_size_t i;
	even_drivers_size_t count = driverlist->dataoffset / sizeof(x12_even_driver_t);
	x12_even_driver_t *driver = NULL;
	
	for(i = 0; i < count; i++)
	{
		driver = NULL;
		driver = &(driverlist->driverlist)[i];

		if(driver != NULL)
		{
			if(driver->datalen == _datalen && memcmp(_linkdata, driver->linkdata, _datalen) == 0)
			{
				*_id = driver->driverid;
				x12_pthread_mutex_unlock(&(driverlist->mutex));
				return 0;
			}
		}
	}

	struct x12_even_driver_s item;
	item.eventype = _eventype;
	item.linkdata = _linkdata;
	item.datalen = _datalen;

	if(driverlist->nullidoffset != 0)
	{
		item.driverid = (driverlist->nullid)[(driverlist->nullidoffset / sizeof(int)) - 1];
		driverlist->nullidoffset -= sizeof(int);
	}
	else
	{
		item.driverid = driverlist->dataoffset / sizeof(x12_even_driver_t);
		
		if(driverlist->dataoffset + sizeof(x12_even_driver_t) > driverlist->listsize)
		{
			even_drivers_size_t newsize = driverlist_align(driverlist->dataoffset + sizeof(x12_even_driver_t), driverlist->blocksize);
			driverlist->driverlist = (x12_even_driver_t *)realloc(driverlist->driverlist, newsize);
			driverlist->listsize = newsize;
		}

		driverlist->dataoffset += sizeof(x12_even_driver_t);
	}

	driverlist->drivercount += 1;

	memcpy(&(driverlist->driverlist)[item.driverid], &item, sizeof(item));
	
	*_id = item.driverid;

	x12_pthread_mutex_unlock(&(driverlist->mutex));

	return 0;
}

char x12_even_drivers_del(int _id)
{
	if(driverlist == NULL)
	{
		return -1;
	}

	int lockret = x12_pthread_mutex_trylock(&(driverlist->mutex));

	if(lockret != X12_PTHREAD_LOCK_OK)
	{
		return -2;
	}

	x12_even_driver_t *item = NULL;

	item = x12_even_drivers_search(_id);

	if(item != NULL)
	{
		free(item);
		item = NULL;

		if(driverlist->nullidoffset + sizeof(_id) > driverlist->nullidsize)
		{
			even_drivers_size_t newsize = driverlist_align(driverlist->nullidsize + driverlist->blocksize, driverlist->blocksize);
			driverlist->nullid = (int *)realloc(driverlist->nullid, newsize);
			driverlist->nullidsize = newsize;
		}

		memcpy(driverlist->nullid + driverlist->nullidoffset, &_id, sizeof(_id));
		driverlist->nullidoffset += sizeof(_id);
	}

	x12_pthread_mutex_unlock(&(driverlist->mutex));

	return 0;
}
