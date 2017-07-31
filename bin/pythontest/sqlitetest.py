#!/usr/bin/env python
# -*- coding: UTF-8 -*-

'''
 Filename : hello.py
 author : Crow
 creat time: 2017-06-03
 updata time: 2017-06-03
'''

import sys
import sqlite3
import types
import datetime

dbpath = 'test.db'

def main():
	#CreatTable()
	#Inset('''INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) 
	#	VALUES (1, 'Paul', 32, 'California', 20000.00 )''')
	data = {'NAME':str(datetime.datetime.now()), 'AGE':32, 'ADDRESS':'ADDR', 'SALARY':50000.00}
	InsetDict('COMPANY', data)
	table = Select('select * from COMPANY')
	print(table)

	

def CreatTable():
	conn = sqlite3.connect(dbpath)
	conn.execute('''CREATE TABLE COMPANY
		(ID INT PRIMARY KEY NOT NULL,
		NAME TEXT NOT NULL,
		AGE INT NOT NULL,
		ADDRESS CHAR(50),
		SALARY REAL);''')
	conn.close()

def Inset(_sql):
	conn = sqlite3.connect(dbpath)
	conn.execute(_sql)
	conn.commit()
	conn.close()

def InsetDict(_table, _dict):
	if _dict is None:
		return None

	key_list = _dict.keys()
	col = ','.join(key_list)

	val_list = _dict.values()

	val = ''
	
	for key in key_list:
		if type(_dict[key]) == types.IntType or type(_dict[key]) == types.FloatType:
			val += str(_dict[key]) + ','
		else:
			val += '\'' + str(_dict[key]) + '\','

	val = val[:-1]

	sql = ''
	sql += 'INSERT INTO ' + _table + ' (' + col + ') '
	sql += 'VALUES (' + val + ') '
	
	Inset(sql)

def Select(_sql):
	conn = sqlite3.connect(dbpath)
	conn.row_factory = sqlite3.Row

	cur = conn.cursor()
	cur.execute(_sql)

	ret = cur.fetchall()

	conn.close()
	
	return ret




if __name__ == '__main__':
    main()
