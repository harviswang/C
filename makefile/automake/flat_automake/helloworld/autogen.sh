#!/bin/bash

rm -fr autom4te.cache  autoscan.log config.log config.status configure depcomp Makefile aclocal.m4 hello install-sh Makefile.in missing mymain.o mytest.o
aclocal
autoconf 
automake --add-missing
./configure 
make
