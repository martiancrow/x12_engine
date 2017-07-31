/*
 *
 *Copyright (C) 刘晓龙
 *Copyright (C) 昆明黑海科技有限公司
 *
 *
 */

/*
 *x12 py 插件接口封装
 *
 *
 *作者:刘晓龙
 *时间:2017-06_03
 *最后修改时间:2017-06-03
 */

#include "x12_py_wrapper.h"

int sendtest(int _driverid, PyObject *_ctx)
{

	if(!_ctx)
	{
		return -1;
	}

	void* x12_handle = NULL;
	
	x12_handle = dlopen("./bin/x12.so", RTLD_LAZY);

	if(!x12_handle)
	{
		return -1;
	}

	int ret = 0;

	void* (*pyc_pyobj_to_ctx)(PyObject*);
	pyc_pyobj_to_ctx = dlsym(x12_handle, "pyc_pyobj_to_ctx");
	
	void* ctx = pyc_pyobj_to_ctx(_ctx);
	
	uint8_t* (*smart_coap_tool_context_encode)(void*, uint32_t*);
	smart_coap_tool_context_encode = dlsym(x12_handle, "smart_coap_tool_context_encode");
	
	uint32_t msglen = 0; 
	uint8_t *msg = smart_coap_tool_context_encode(ctx, &msglen);

	int (*x12_msg_send)(int, uint8_t*, int);
	x12_msg_send = dlsym(x12_handle, "x12_msg_send");

	ret = x12_msg_send(_driverid, msg, msglen);

	return ret;

}

//导出函数
PyObject* wrap_sendtest(PyObject* self, PyObject* args)
{
	int driverid;
    int result;
	PyObject *ctx;

    if(!PyArg_ParseTuple(args, "i|O", &driverid, &ctx))
    {
        return NULL;
    }

	result = sendtest(driverid, ctx);

    return Py_BuildValue("i", result);
}

int load(void* _data)
{

    printf("this is example.so load func\n");

    //value_set("hello example.so\n");

    void* handle;

    handle = dlopen("./x12.so", RTLD_LAZY);
    
    if(!handle)
    {
        return -1;
    }

    char* (*value_get)();

    value_get = dlsym(handle, "value_get");

    printf("this is example.so %s\n", value_get());

    return 0;
}

int fact(int n)
{
    if(n <= 1)
    {
        return 1;
    }
    else
    {
        return n*fact(n-1);
    }
}

//导出函数
PyObject* wrap_fact(PyObject* self, PyObject* args)
{
    int n, result;

    if(!PyArg_ParseTuple(args, "i:fact", &n))
    {
        return NULL;
    }

    result = fact(n);

    return Py_BuildValue("i", result);
}

//方法列表
static PyMethodDef x12PyMethods[] =
{
    {"fact", wrap_fact, METH_VARARGS, "Caculate N!"},
    {"sendtest", wrap_sendtest, METH_VARARGS, "Caculate N!"},
    {"x12_coap_msg_send", x12_py_coap_msg_send, METH_VARARGS, "Caculate N!"},
    {"x12_coap_set_msgcb", x12_py_coap_set_msgcb, METH_VARARGS, "Caculate N!"},
    {"x12_coap_route_add", x12_py_coap_route_add, METH_VARARGS, "Caculate N!"},
    {"x12_coap_udp_listen", x12_py_coap_udp_listen, METH_VARARGS, "Caculate N!"},
    {"x12_coap_udp_conn", x12_py_coap_udp_conn, METH_VARARGS, "Caculate N!"},
    {NULL, NULL}
};

//初始化函数
void initx12_python()
{
    PyObject* m;
    m = Py_InitModule("x12_python", x12PyMethods);
}
