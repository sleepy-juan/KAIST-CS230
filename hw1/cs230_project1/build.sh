#!/bin/sh
# Do not touch this file!!!
# VERSION = 1
# EXTRAVERSION = 2018.spring
# NAME = kaist.resl.cs230_bit_manipulation.build
workdir=$PWD
dirs=`find -maxdepth 1 -mindepth 1 -type d | sed -e s/..//`
for dir in $dirs
do
	cd $workdir/$dir
	echo "make executable for student ID: $dir -> program_$dir"
	rm -rf ./program_$dir
	gcc -g ../main.c ./skeleton.c -o ./program_$dir
	cd $workdir
done

