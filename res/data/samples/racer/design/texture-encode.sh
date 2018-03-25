#!/bin/sh

DIR_OUT=encoded
mkdir -p $DIR_OUT

EXT=png
for i in *.$EXT; do
    base=`basename $i .$EXT`
    echo $i
    ./texturecRelease -f $i -o $DIR_OUT/$base.dds -m -t BC3
done

