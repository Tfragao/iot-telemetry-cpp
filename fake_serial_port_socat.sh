#!/usr/bin/env bash

set -e

echo "Testing UART mode without physical HW using virtual serial ports"

#echo "First create two connected virtual ports: socat -d -d pty,raw,echo=0 pty,raw,echo=0"
#echo "It may print something like: "
#echo "2026/06/15 17:36:52 socat[21723] N PTY is /dev/pts/2"
#echo "2026/06/15 17:36:52 socat[21723] N PTY is /dev/pts/3"

#echo "Then run your app on one side like running this file."
#echo "And from another terminal, send data to the other side:"
#echo "for example: echo "TEMP=25.4,HUM=51.2,VOLT=3.3,DI0=1,DI1=0,DI2=1,DI3=0" > /dev/pts/3"

./build/debug/iot_telemetry_app --mode uart --port /dev/pts/2 --baud 115200 --samples 1 --verbose