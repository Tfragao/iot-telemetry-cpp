#!/usr/bin/env bash

set -e   # if the program fails, the script stops immediately

echo "Executing fake mode ..."
./build/debug/iot_telemetry_app --mode fake --samples 5 --verbose

echo "Done"