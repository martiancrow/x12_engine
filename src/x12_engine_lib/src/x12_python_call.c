/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 python 回调工具
 *
 *
 *作者:Crow
 *时间:2017-06_03
 *最后修改时间:2017-06-03
 */

#include "x12_python_call.h"

static char *rootPath = NULL;
static x12_py_plugin_list_t *pyplist = NULL;

void load_py_plugin_cb(PyObject *_ret)
{
	if(_ret)
	{
		int ret = 0;

		x12_py_plugin_item_t *item = (x12_py_plugin_item_t *)malloc(sizeof(x12_py_plugin_item_t));	
		memset(item, 0, sizeof(x12_py_plugin_item_t));

		PyObject *pPath = PyDict_GetItemString(_ret, "path");
		if(pPath)
		{
			char *path = NULL;
			ret = PyArg_Parse(pPath, "s", &path);
			if(ret)
			{
				memcpy(item->path, path, strlen(path));	
			}
		}
		
		PyObject *pModule = PyDict_GetItemString(_ret, "module");
		if(pModule)
		{
			char *module = NULL;
			ret = PyArg_Parse(pModule, "s", &module);
			if(ret)
			{
				memcpy(item->module, module, strlen(module));	
			}
		}
		
		PyObject *pFunc = PyDict_GetItemString(_ret, "func");
		if(pFunc)
		{
			char *func = NULL;
			ret = PyArg_Parse(pFunc, "s", &func);
			if(ret)
			{
				memcpy(item->func, func, strlen(func));	
			}
		}

		if(pyplist)
		{
			if(pyplist->head)
			{
				item->next = pyplist->head;
			}
			
			pyplist->head = item;
		}

		x12_python_call_eval(item->path, item->module, item->func, NULL, "()");
	}
}

int x12_python_call_init(char *_rootpath)
{
	rootPath = _rootpath;

	Py_Initialize();

	if(!pyplist)
	{
		pyplist = (x12_py_plugin_list_t *)malloc(sizeof(x12_py_plugin_list_t));
		memset(pyplist, 0, sizeof(x12_py_plugin_list_t));
	}

	struct dirent* ent = NULL;
    DIR *pDir;
	pDir=opendir(_rootpath);
	
	ent = readdir(pDir);

	while(ent != NULL)
	{
			
		if(ent->d_type == 8)
		{

			char *pyext = ent->d_name + strlen(ent->d_name) - 3;

			if(strcmp(pyext, ".py") == 0)
			{
				*pyext = '\0';
				x12_python_call_eval(rootPath, ent->d_name, "load_init", load_py_plugin_cb, "()");
			}

		}

		ent = readdir(pDir);
	}

	return 0;
}

void x12_python_call_pyp_free(x12_py_plugin_item_t *_item)
{
	if(_item)
	{
		x12_python_call_pyp_free(_item->next);

		free(_item);
		_item = NULL;
	}
}

void x12_python_call_reslease(void)
{
	if(pyplist)
	{
		x12_python_call_pyp_free(pyplist->head);
		free(pyplist);
		pyplist = NULL;
	}

	Py_Finalize();
}

void x12_python_call_setpath(const char *_rootpath)
{
	int cmdlen = 50;

	char rootpath[strlen(_rootpath) + 1];
	memset(rootpath, 0, sizeof(rootpath));
	memcpy(rootpath, _rootpath, strlen(_rootpath));

	
	if(_rootpath != NULL)
	{
		cmdlen += strlen(_rootpath) + 1;
	}
	
	char cmd[cmdlen];
	memset(cmd, 0, cmdlen);

	int cmdoffset = 0;
	char *cmdline = "import sys\n";
	sprintf(cmd + cmdoffset, "%s", cmdline);

	cmdoffset += strlen(cmdline);
	cmdline = "sys.path.append('";
	sprintf(cmd + cmdoffset, "%s", cmdline);

	cmdoffset += strlen(cmdline);
	cmdline = rootpath;
	sprintf(cmd + cmdoffset, "%s", cmdline);

	cmdoffset += strlen(cmdline);
	cmdline = "')";
	sprintf(cmd + cmdoffset, "%s", cmdline);

	PyRun_SimpleString(cmd);

}

int x12_python_call_eval(const char *_rootpath, const char *_module, const char *_function, pyeval_ret _func, const char *_format, ...)
{
	
	if(!_rootpath)
	{
		return -1;
	}
	
	x12_python_call_setpath(_rootpath);
	
	PyObject *pMod = PyImport_ImportModule(_module);
		
	if(!pMod)
	{
		return -1;
	}

	PyObject *pFunc = PyObject_GetAttrString(pMod, _function);

	if(!pFunc)
	{
		return -1;
	}

	va_list vargs;
	va_start(vargs, _format);

	PyObject *pArgs = NULL;

	pArgs = Py_VaBuildValue(_format, vargs);

	va_end(vargs);
	
	PyObject *pRetVal = PyEval_CallObject(pFunc, pArgs);
	
	if(_func != NULL)
	{
		_func(pRetVal);
	}
	
	return 0;

}







int x12_python_call_plugin_luacfg_do(lua_State *_sl)
{
	if(!_sl)
	{
		return -1;
	}

	if(!lua_istable(_sl, -1))
	{
		return -1;
	}

	int t_idx = lua_gettop(_sl);
	lua_pushnil(_sl);

	int i;

	int plugin_len = 0;
	char** plugin = NULL;

	while(lua_next(_sl, t_idx))
	{
		const char *key = lua_tostring(_sl, -2);
	
		if(strcmp(key, "family") == 0)
		{
			if(!lua_isstring(_sl, -1))
			{
				goto param_err;
			}

			const char *family = lua_tostring(_sl, -1);

			if(strcmp(family, "x12_plugin_py") != 0)
			{
				goto param_err;
			}

		}
		else if(strcmp(key, "pludir") == 0)
		{
			if(!lua_istable(_sl, -1))
			{
				goto param_err;
			}

			plugin_len = luaL_len(_sl, -1);

			plugin = (char **)malloc(sizeof(char *) * plugin_len);
			memset(plugin, 0, sizeof(char *) * plugin_len);

			for(i = 1; i <= plugin_len; i++)
			{
				lua_pushnumber(_sl, i);
				lua_gettable(_sl, -2);
				const char *val = lua_tostring(_sl, -1);

				if(val)
				{
					plugin[i - 1] = (char *)malloc(strlen(val) + 1);
					memset(plugin[i - 1], 0, strlen(val) + 1);
					memcpy(plugin[i - 1], val, strlen(val));
				}

				lua_pop(_sl, 1);
			}
		}
		
		lua_pop(_sl, 1);
	}

	x12_listen_evens_init();
	x12_even_drivers_init();
	x12_coap_kit_init();

	for(i = 0; i < plugin_len; i++)
	{
		printf("plugin is %s\n", plugin[i]);

		x12_python_call_init(plugin[i]);
	}


	if(plugin)
	{
		for(i = 0; i < plugin_len; i++)
		{
			if(plugin[i])
			{
				free(plugin[i]);
				plugin[i] = NULL;
			}
		}

		free(plugin);
		plugin = NULL;
	}
	

	lua_settop(_sl, t_idx);
	
	return 0;

param_err:
	//参数错误
	if(plugin)
	{
		for(i = 0; i < plugin_len; i++)
		{
			if(plugin[i])
			{
				free(plugin[i]);
				plugin[i] = NULL;
			}
		}

		free(plugin);
		plugin = NULL;
	}

	lua_settop(_sl, t_idx);
	return -1;
}

