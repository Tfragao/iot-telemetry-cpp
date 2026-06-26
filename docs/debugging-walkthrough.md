# Debugging Walkthrough 

This document describes how debug the IoT Telemetry C++ application using VS code, GDB, and virtual UART ports.

## 1. Build the project in Debug mode

From the project root:
```bash 
cmake --preset debug
cmake --build --preset debug
```
The executable should be generated at: 
build/debug/iot_telemetry_app 

## 2. Debug configurations 

The project provides multiple VS Code debug configurations in .vscode/launch.json:
```bash
- Debug Fake Mode
- Debug System Mode
- Debug UART Mode - Virtual Serial 
- Debug UART Mode - Real USB Serial
``

Use the Run and Debug panel in VS Code to select the desired configuration.

## 3. Recommended breakpoints

Useful breakpoints for understanding the project flow:
```bash
main()
 -> config::parse_arguments()
 -> app::run()
 -> create_sensor_reader()
 -> FakeSensorReader::read()
 -> SystemSensorReader::read()
 -> UartSensorReader::UartSensorReader()
 -> UartSensorReader::configure()
 -> UartSensorReader::read()
 -> UartSensorReader::read_line()
 -> parse_uart_line()
 -> telemetry::create_packet()
 -> telemetry::evaluate_status()
 -> logger::CsvLogger::write_packet()
``
## 4. Debugging fake mode

Select:

```bash
Debug Fake Mode 
```
This mode does not require hardware. It generates fake sensor reading and writes telemetry to CSV.
Expected flow:
```bash
main()
    -> parse_arguments()
    -> app::run()
    -> create_sensor_reader()
    -> FakeSensorReader::read()
    -> telemetry::create_packet()
    -> CsvLogger::write_packet()
``
## 5. Debugging system mode

Select:
```bash
Debug System Mode
```
This mode reads Linux system data, such as CPU temperature from: 
```bash
/sys/class/thermal/thermal_zone0/temp
```
If the file is not available, the implementation may fall back to simulated values.
Expected flow:
```bash
main()
    -> parse_arguments()
    -> app::run()
    -> create_sensor_reader()
    -> SystemSensorReader::read()
    -> telemetry::create_packet()
    -> CsvLogger::write_packet()
``
## 6. Debugging UART mode with virtual serial ports

UART mode can be tested without physical hardware using socat.

Start virtual serial ports:
```bash
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```
Example output:
App side:     /dev/pts/2
Sender side:  /dev/pts/3

In VsCode, select:
```bash
Debug UART Mode - Virtual Serial
```
Make sure the launch configuration uses the app-side port:
```bash
"--port",
"/dev/pts/2"
```
Then send test messages from another terminal:
```bash
echo "TEMP=25.4,HUM=51.2,VOLT=3.3,DI0=1,DI1=0,DI2=1,DI3=0" > /dev/pts/3
echo "TEMP=31.7,HUM=48.0,VOLT=3.2,DI0=0,DI1=0,DI2=1,DI3=1" > /dev/pts/3
echo "TEMP=42.1,HUM=61.5,VOLT=3.4,DI0=1,DI1=1,DI2=0,DI3=0" > /dev/pts/3
```
Expected UART flow:
```bash
main()
    -> parse_arguments()
    -> app::run()
    -> create_sensor_reader()
    -> UartSensorReader::UartSensorReader()
    -> UartSensorReader::configure()
    -> UartSensorReader::read()
    -> UartSensorReader::read_line()
    -> parse_uart_line()
    -> telemetry::create_packet()
    -> CsvLogger::write_packet()
``
## 7. UART message format

The expected UART input format is:
```bash
TEMP=25.4,HUM=51.2,VOLT=3.3,DI0=1,DI1=0,DI2=1,DI3=0
```
Fields:
```bash
TEMP: temperature in Celsius
HUM: humidity percentage
VOLT: voltage in volts
DI0 to DI3 : digital input states
```
Each UART message should end with a newline character.

## 8. Common issues

### The app waits forever in UART mode

This usually means the app is waiting inside read() for UART data.<br>
Send a valid message to the other virtual serial port

### The wrong /dev/pts/X port is used 

Every time socat is restarted, the port numbers may change.<br>
Update the VS Code launch configuration with the new app-side port.

### CSV file is not created 

Check that the working directory is correct.

The VS Code launch configuration should be use the project root as cwd.<br>
Example:
```bash
"cwd" : "${workspaceFolder}/iot-telemetry-cpp"
```
### Breakpoint are not hit

Make sure the project was built in Debug mode:
```bash
cmake --preset debug
cmake --build --preset debug
```

## 9. Debugging goal

The goal of this debugging process is to understand the complete telemetry flow:

```bash
Command-line arguments
    -> runtime configuration
    -> sensor reader selection 
    -> sensor data acquisition 
    -> telemetry packet creation 
    -> status evaluation 
    -> CSV logging 
``
