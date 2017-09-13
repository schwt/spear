#!/usr/bin/env python
# -*- coding: utf8 -*-
##Filename:    generate.py
 #Author:      wyb
 #Email:       yingbin.wang@vipshop.com
 #Date:        2017-08-10 14:39:22
 #
import sys
import random as rd

output = "train.txt"
max_uid = 10000
max_pid = 1000
max_score = 10

def main():
    num = 1000000
    wf = open(output, 'w')
    if len(sys.argv) > 1:
        num = int(sys.argv[1])
        print "generate num:", num
    else:
        print "use default num:", num
    if num > 1000000:
        print "it may use several minutes..."
    for x in xrange(num):
        wf.write("%d %d %d\n" % (rd.randint(0, max_uid), rd.randint(0, max_pid), rd.randint(0, max_score)))
    wf.close()
    print "done."

if __name__ == '__main__':
    main()
