#!/usr/bin/env bash

set -e 

echo "Executing UART mode"

./build/debug/iot_telemetry_app --device rpi4-gateway-001 --samples 1 --mode uart --port /dev/ttyUSB0 --baud 115200 --log data/uart.csv --verbose

echo "Done"
