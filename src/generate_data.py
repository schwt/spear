#!/usr/bin/env python
# -*- coding: utf8 -*-
##Filename:    generate.py
 #Author:      wyb
 #Email:       yingbin.wang@vipshop.com
 #Date:        2017-08-10 14:39:22
 #
import sys
import random as rd

output = "../data/train.txt"
max_uid = 1000000
max_pid = 100000
max_score = 10
num = 10000000

def main():
    wf = open(output, 'w')
    for x in xrange(num):
        wf.write("%d %d %d\n" % (rd.randint(0, max_uid), rd.randint(0, max_pid), rd.randint(0, max_score)))
    wf.close()
    print "done."

if __name__ == '__main__':
    main()
