#!/bin/sh
# -*- coding: utf8 -*-
##Filename:    demo.sh
 #Author:      wyb
 #Email:       yingbin.wang@vipshop.com
 #Date:        2017-08-11 10:33:05
 #
dir=$(cd $(dirname $0); pwd)

# build
echo "\n\nbuilding ......"
cd $dir/../lib/
make clean; make
cd $dir/
make clean; make

# generate random training set
echo "\n\nmake data ......"
cd $dir/../data/
./generate_data.py 1000000

# run train
echo "\n\ntraining ......"
cd $dir
./spear config.ini

echo "done."

# output files are in ../data/
cd $dir/../data/
ls -l

