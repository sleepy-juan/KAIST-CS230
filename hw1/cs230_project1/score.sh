#!/bin/sh
# Do not touch this file!!!
# VERSION = 1
# EXTRAVERSION = 2018.spring
# NAME = kaist.deal.cs230_bit_manipulation.score
workdir=$PWD
dirs=`find -maxdepth 1 -mindepth 1 -type d | sed -e s/..//`
for dir in $dirs
do
	cd $workdir/$dir

	echo "\n----- test start for id $dir -----"
	testnum=1
	exec 3<&0 0<$workdir/testset
	while read line
	do
		if test -n "$line"; then
			echo $line > temp
			trap 'rm temp;' 1 2 3 15
			i=0;
			echo "\n<test-case-$testnum>"
			./program_$dir < temp | \
			awk '{printf("#%d: %18s %18s %s\n", ++i, $6, $7, $6==$7 ? "pass":"fail")}'
			rm -f temp
			testnum=$(($testnum+1))			
		fi
	done 
	exec 0<&3 3<&-
	cd $workdir
done

