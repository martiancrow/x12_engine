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
		'path': './bin/smartserver', 
		'module': 'smartserver', 
		'func': 'smartserverinit'
	}

	return cfg

if __name__ == '__main__':
    load_init()
