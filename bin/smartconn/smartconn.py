#!/usr/bin/env python
# -*- coding: UTF-8 -*-

'''
 Filename : hello.py
 author : Crow
 creat time: 2017-06-03
 updata time: 2017-06-03
'''

import sys
import datetime

sys.path.append('/Volumes/DATA/virtualboxHost/ubuntu16.04_64/x12_engine/bin')
import x12_python

def smartconninit():
	#print("this is smartconninit")

	path = '/Volumes/DATA/virtualboxHost/ubuntu16.04_64/x12_engine/bin/smartconn'

	drvid = x12_python.x12_coap_udp_conn('127.0.0.1', 9095)

	msgcbd = {
		'path':path,
		'module':'smartconn',
		'func':'msgcb'
	}

	#print("drvid = " + str(drvid))

	x12_python.x12_coap_set_msgcb(drvid, msgcbd)

	req = {'code': 'GET', 'ver': 1, 'msgid': 18705, 'tkl': 0, 't': 'CON', 'uri-port': 9095, 'uri-path':'/time'}
	#print(req)
	sendlen = x12_python.x12_coap_msg_send(drvid, req)
	#print('sendlen = ' + str(sendlen))

def msgcb(send, msg):
	print("----------------------msgcb-------------------------")
	print("server time is :" + msg['POST'])
	
	print("----------------------msgcb-------------------------")

if __name__ == '__main__':
    smartconninit()
