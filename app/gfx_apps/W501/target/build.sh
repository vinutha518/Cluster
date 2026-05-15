#!/bin/bash
echo cd $( cygpath -u  $PWD)
cd $( cygpath -u  $PWD)
export DEVENV=v800_ghs
. ~/.bashrc
make -j 10  $* 2>&1 |sed -r 's/\[asarm\] \(error\) ([a-zA-Z]:[^:]*) ([0-9]+)/[asarm] "\1", line \2: error /;s/\"[Cc]:/\"\/cygdrive\/c/g;s/\\/\//g;s/ \/$/ \\/;s/ *$//;s/(.{160}[^ ]*[^I ]) +/\1\n/g' 
#make $* 2>&1 |sed -r 's/\[asarm\] \(error\) ([a-zA-Z]:[^:]*) ([0-9]+)/[asarm] "\1", line \2: error /;s/\"[Cc]:/\"\/cygdrive\/c/g;s/\\/\//g;s/ \/$/ \\/;s/ *$//;s/(.{160}[^ ]*[^I ]) +/\1\n/g' 
