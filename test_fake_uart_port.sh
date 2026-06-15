#!/usr/bin/env bash

set -e 

#adjust /dev/pts/3 accordingly

echo "TEMP=25.4,HUM=51.2,VOLT=3.3,DI0=1,DI1=0,DI2=1,DI3=0" > /dev/pts/3
