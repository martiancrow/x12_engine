#!/usr/bin/env python
# -*- coding: UTF-8 -*-

'''
 Filename : smartapp.py
 author : Crow
 creat time: 2017-06-11
 updata time: 2017-06-11
'''

def load_init():

	cfg = {
		'path': '/Volumes/DATA/virtualboxHost/ubuntu16.04_64/x12_engine/bin/testcilent/bin/smartconn', 
		'module': 'smartconn', 
		'func': 'smartconninit'
	}

	return cfg

if __name__ == '__main__':
    load_init()
