/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 python coap 插件接口
 *
 *作者:Crow
 *时间:2017-06_03
 *最后修改时间:2017-06-03
 */

#include "x12_py_coap_kit.h"

PyObject* x12_py_coap_udp_listen(PyObject* self, PyObject* args)
{
	char *addr = NULL;
	int port;

    if(!PyArg_ParseTuple(args, "s|i", &addr, &port))
    {
        return NULL;
    }

	void* x12_handle = NULL;

	x12_handle = dlopen("./bin/x12.so", RTLD_LAZY);

	if(!x12_handle)
	{
		return NULL;
	}

	if(strcmp(addr, "ALL") == 0)
	{
		addr = NULL;
	}

	int (*x12_py_coap_kit_udp_listen)(const char*, int);
	x12_py_coap_kit_udp_listen = dlsym(x12_handle, "x12_py_coap_kit_udp_listen");
	
	int driverid = -1;

	driverid = x12_py_coap_kit_udp_listen(addr, port);
	
	dlclose(x12_handle);

    return Py_BuildValue("i", driverid);

}

PyObject* x12_py_coap_udp_conn(PyObject* self, PyObject* args)
{
	char *addr;
	int port;

    if(!PyArg_ParseTuple(args, "s|i", &addr, &port))
    {
        return NULL;
    }

	void* x12_handle = NULL;

	x12_handle = dlopen("./bin/x12.so", RTLD_LAZY);

	if(!x12_handle)
	{
		return NULL;
	}

	int (*x12_py_coap_kit_udp_conn)(const char*, int);
	x12_py_coap_kit_udp_conn = dlsym(x12_handle, "x12_py_coap_kit_udp_conn");
	
	int driverid = -1;

	driverid = x12_py_coap_kit_udp_conn(addr, port);
	
	dlclose(x12_handle);

    return Py_BuildValue("i", driverid);

}

PyObject* x12_py_coap_set_msgcb(PyObject* self, PyObject* args)
{
	int drvid = 0;
	PyObject *pMsgcb;
    
	if(!PyArg_ParseTuple(args, "i|O", &drvid, &pMsgcb))
    {
        return NULL;
    }

	void* x12_handle = NULL;

	x12_handle = dlopen("./bin/x12.so", RTLD_LAZY);

	if(!x12_handle)
	{
		return NULL;
	}

	int (*x12_py_coap_kit_set_msgcb)(int, char*, char*, char*);
	x12_py_coap_kit_set_msgcb = dlsym(x12_handle, "x12_py_coap_kit_set_msgcb");

	char *path;
	char *mod;
	char *func;
	
	PyObject *pPath = PyDict_GetItemString(pMsgcb, "path");
	PyObject *pMod = PyDict_GetItemString(pMsgcb, "module");
	PyObject *pFunc = PyDict_GetItemString(pMsgcb, "func");

	PyArg_Parse(pPath, "s", &path);
	PyArg_Parse(pMod, "s", &mod);
	PyArg_Parse(pFunc, "s", &func);
		
	int ret = x12_py_coap_kit_set_msgcb(drvid, path, mod, func);
	
	dlclose(x12_handle);
	
    return Py_BuildValue("i", ret);

}

PyObject* x12_py_coap_route_add(PyObject* self, PyObject* args)
{
	int drvid = 0;
	PyObject *pRouteList;

    if(!PyArg_ParseTuple(args, "i|O", &drvid, &pRouteList))
    {
        return NULL;
    }

	void* x12_handle = NULL;

	x12_handle = dlopen("./bin/x12.so", RTLD_LAZY);

	if(!x12_handle)
	{
		return NULL;
	}

	int (*x12_py_coap_kit_ri_add)(int, char*, char*, char*, char*);
	x12_py_coap_kit_ri_add = dlsym(x12_handle, "x12_py_coap_kit_ri_add");
	
	int ret = -1;
	
	Py_ssize_t count = PyList_GET_SIZE(pRouteList);

	Py_ssize_t i;
	PyObject *pRItem = NULL;


	for(i = 0; i < count; i++)
	{
		pRItem = PyList_GetItem(pRouteList, i);
	
		if(!pRItem)
		{
			break;
		}

		char *uri = NULL;
		char *path = NULL;
		char *mod = NULL;
		char *func = NULL;
			
		PyObject *pUri = PyDict_GetItemString(pRItem, "uri");
		PyObject *pPath = PyDict_GetItemString(pRItem, "path");
		PyObject *pMod = PyDict_GetItemString(pRItem, "module");
		PyObject *pFunc = PyDict_GetItemString(pRItem, "func");

		PyArg_Parse(pUri, "s", &uri);
		PyArg_Parse(pPath, "s", &path);
		PyArg_Parse(pMod, "s", &mod);
		PyArg_Parse(pFunc, "s", &func);
		
		ret = x12_py_coap_kit_ri_add(drvid, uri, path, mod, func);

		if(ret < 0)
		{
			break;
		}

	}

	dlclose(x12_handle);
	
    return Py_BuildValue("i", ret);
	
}

PyObject* x12_py_coap_msg_send(PyObject* self, PyObject* args)
{
	int drvid = 0;
	PyObject *pCtx;

    if(!PyArg_ParseTuple(args, "i|O", &drvid, &pCtx))
    {
        return NULL;
    }

	void* x12_handle = NULL;

	x12_handle = dlopen("./bin/x12.so", RTLD_LAZY);

	if(!x12_handle)
	{
		return NULL;
	}

	int ret = 0;

	void* (*pyc_pyobj_to_ctx)(PyObject*);
	pyc_pyobj_to_ctx = dlsym(x12_handle, "pyc_pyobj_to_ctx");
	
	void* ctx = pyc_pyobj_to_ctx(pCtx);
	
	uint8_t* (*smart_coap_tool_context_encode)(void*, uint32_t*);
	smart_coap_tool_context_encode = dlsym(x12_handle, "smart_coap_tool_context_encode");
	
	uint32_t msglen = 0; 
	uint8_t *msg = smart_coap_tool_context_encode(ctx, &msglen);

	int (*x12_msg_send)(int, uint8_t*, int);
	x12_msg_send = dlsym(x12_handle, "x12_msg_send");

	ret = x12_msg_send(drvid, msg, msglen);
	
	dlclose(x12_handle);

    return Py_BuildValue("i", ret);
}

