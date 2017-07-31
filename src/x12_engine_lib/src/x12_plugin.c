/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 插件相关操作
 *
 *
 *作者:Crow
 *时间:2017-06-06
 *最后修改时间:2017-06-06
 */

#include "x12_plugin.h"


int x12_plugin_load(const char *_path, const char *_loadname, void* _data)
{
	if(!_path || !_loadname)
	{
		return -1;
	}

	void* handle;
	plugin_load_func load_func;

	handle = dlopen(_path, RTLD_LAZY);
	
	if(!handle)
	{
		return -1;
	}

	load_func = dlsym(handle, _loadname);

	if(!load_func)
	{
		return -1;
	}

	uint16_t ret = load_func(_data);

	dlclose(handle);

	return ret;

}
