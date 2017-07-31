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

#ifndef _X12_PY_COAP_KIT_H_INCLUDE
#define _X12_PY_COAP_KIT_H_INCLUDE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <python2.7/Python.h>
#include <dirent.h>
#include <dlfcn.h>

extern PyObject* x12_py_coap_udp_listen(PyObject* self, PyObject* args);

extern PyObject* x12_py_coap_udp_conn(PyObject* self, PyObject* args);

extern PyObject* x12_py_coap_set_msgcb(PyObject* self, PyObject* args);

extern PyObject* x12_py_coap_route_add(PyObject* self, PyObject* args);

extern PyObject* x12_py_coap_msg_send(PyObject* self, PyObject* args);

#endif
