#!/usr/bin/env bash

set -e 

#adjust /dev/pts/3 accordingly

echo "TEMP=25.4,HUM=51.2,VOLT=3.3,DI0=1,DI1=0,DI2=1,DI3=0" > /dev/pts/3
echo "TEMP=31.7,HUM=48.0,VOLT=3.2,DI0=0,DI1=0,DI2=1,DI3=1" > /dev/pts/3
echo "TEMP=42.1,HUM=61.5,VOLT=3.4,DI0=1,DI1=1,DI2=0,DI3=0" > /dev/pts/3
