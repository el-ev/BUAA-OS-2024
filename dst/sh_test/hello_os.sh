#!/bin/bash
if [ $# -ne 2 ]; then
   exit 1
fi

touch $2

awk 'NR==8 || NR==32 || NR ==128 || NR==512 || NR==1024' $1 > $2

