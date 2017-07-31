#!/usr/bin/env python
# -*- coding: UTF-8 -*-


import datetime
import example

def fact(n):
    if n <= 1:
        return 1
    else:
        return n * fact(n-1)

if __name__ == '__main__':
    now = datetime.datetime.now()
    
    for i in range(10000):
        n = fact(100)

    end = datetime.datetime.now()
    
    print 'the python fact takes:', end-now
    
    for i in range(10000):
        n=example.fact(100)
        
    print 'the c fact taks:', datetime.datetime.now()-end
