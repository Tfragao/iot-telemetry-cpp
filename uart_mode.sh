#!/usr/bin/env bash

set -e 

echo "Executing UART mode"

./build/debug/iot_telemetry_app --mode system --samples 5 --verbose

echo "Done"