# opena64
Baremetal, standalone drivers for AArch64. This is not meant to support running OS, this is for baremetal applications only. In theory this drivers could also be used with RTOS applications but this drivers are still premature and have not been tested with RTOSes.

# Running Hello World Example

## 1. Build the app
```bash
cmake -S examples/unity_test -B build ; cmake --build build
```

## 2. Run in Renode
Run this with renode, one one terminal run renode with gui disabled

``` bash
renode --disable-gui --port 1234
```

Then on another run a telnet session
```bash
telnet localhost 1234
```

Then run
``` bash
Clear; include @renode/cortex-a53.resc ; machine StartGdbServer 3334 ; uart_connect uart0
```


## 3. GDB Connect
```bash
aarch64-none-elf-gdb
...
(gdb) source aarch64.gdb
(gdb) c
```
