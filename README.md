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

## Run in QEMU (ARMv9)
With QEMU's AArch64 system emulator installed, run the built ELF on a virtual
machine using an ARMv9 Cortex-A710 CPU:

```bash
qemu-system-aarch64 \
  -machine virt,secure=on,gic-version=3 \
  -cpu cortex-a710 \
  -nographic -monitor none -serial stdio \
  -kernel build/firmware.elf
```

Press `Ctrl-A` then `X` to exit QEMU.

or use max features configuration which enables all armv9 features:
```bash
qemu-system-aarch64 \
    -M virt,secure=on,virtualization=off \
    -cpu max,aarch64=on \
    -accel tcg \
    -smp 1 \
    -m 2048 \
    -nographic \
    -bios path/to/your/baremetal.bin
```

## 3. GDB Connect
```bash
aarch64-none-elf-gdb
...
(gdb) source aarch64.gdb
(gdb) c
```
