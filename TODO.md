## Known issue

UART mode currently opens/configures the serial port for each sample.
This works for one sample but may fail when using multiple samples with virtual serial ports.
Next step: refactor UART into a persistent reader object.