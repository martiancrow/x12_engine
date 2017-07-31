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

#sys.path.append('/Volumes/DATA/virtualboxHost/ubuntu16.04_64/x12_engine/bin')
sys.path.append('./bin')

import x12_python

def smartserverinit():
	#print("this is smartserverinit")

	#path = '/Volumes/DATA/virtualboxHost/ubuntu16.04_64/x12_engine/bin/smartserver'
	path = './bin/smartserver'


	drvid = x12_python.x12_coap_udp_listen('ALL', 9095)

	print("drvid = " + str(drvid))

	routelist = [
		{
			'uri':'/time',
			'path':path,
			'module':'smartserver',
			'func':'time'
		},
		{
			'uri':'/login/:uid/:pwd',
			'path':path,
			'module':'smartserver',
			'func':'login'
		}
	]

	x12_python.x12_coap_route_add(drvid, routelist)


def time(send, msg):
	print("this is time action")
	#print(send)
	#print(msg)
	#print("")
	#print(msg["HEAD"].keys())
	#print(msg["HEAD"].items())
	#print("")
	#print('username = ' + msg['GET']['id'])
	#print('password = ' + msg['GET']['pwd'])

	res = {'code': 'Content', 'ver': 1, 'msgid': msg["HEAD"]["msgid"], 'tkl': 0, 't': 'ACK', 'uri-port': 9095, 'payload': str(datetime.datetime.now())}
	#print(res)
	sendlen = x12_python.x12_coap_msg_send(send, res)
	#print('sendlen = ' + str(sendlen))

def login(send, msg):
	print("this is login action")

	print('username = ' + msg['GET']['uid'])
	print('password = ' + msg['GET']['pwd'])

	res = {'code': 'Content', 'ver': 1, 'msgid': msg["HEAD"]["msgid"], 'tkl': 0, 't': 'ACK', 'uri-port': 9095, 'payload': 'OK'}
	#print(res)
	sendlen = x12_python.x12_coap_msg_send(send, res)
	#print('sendlen = ' + str(sendlen))
	#print("----------------------login-------------------------")


if __name__ == '__main__':
    smartinit()
