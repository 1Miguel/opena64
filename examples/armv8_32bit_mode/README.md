# Running AArch32 code from AArch64 on Armv8

This example demonstrates an AArch64 program running at EL3, then using an
SMC exception to enter a separately built AArch32 program on an Armv8
processor that supports AArch32 execution at a lower exception level.

The AArch64 firmware calls `smc1` with immediate `0x1F` and the AArch32 entry
address (`0x10000`). Its EL3 synchronous exception handler checks the SMC
exception class and immediate, reads the entry address from `x15`, clears
`SCR_EL3.RW` to select AArch32 below EL3, and uses `eret` to branch to the
AArch32 entry point.

The AArch32 startup code initializes its exception-mode stacks and runtime
data, then calls `main`. The payload initializes the PL011 UART at `0x09000000`
and prints `Hello from 32-bit mode!`. The AArch32 linker script places its
image at flash address `0x10000`, where the AArch64 firmware expects it.

## Build

Build the AArch32 payload first. The AArch64 CMake configuration expects its
binary at `build_aarch32/firmware_arm32.bin`:

```bash
cmake -S examples/armv8_32bit_mode/aarch32 -B build_aarch32
cmake --build build_aarch32
```

Then build the AArch64 firmware and merged image:

```bash
cmake -S examples/armv8_32bit_mode -B build
cmake --build build
```

The merged image places the AArch64 firmware at `0x0` and the AArch32 payload
at `0x10000`. Load it into Renode as a raw binary at address zero:

```text
sysbus LoadBinary @build/firmware_merged.bin 0x0
```

The machine must support Armv8 AArch32 execution at the lower exception level;
this is not an Armv9/AArch64-only execution test.
