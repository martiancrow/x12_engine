#!/usr/bin/env python
# -*- coding: UTF-8 -*-

'''
 Filename : hello.py
 author : Crow
 creat time: 2017-06-03
 updata time: 2017-06-03
'''

import sys
import types
import datetime

global _id

def main():
	functest(2)

def functest(_id):
	print(_id)
	apitest()

def apitest():
	print(_id)


if __name__ == '__main__':
    main()
