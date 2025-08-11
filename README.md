# Embedded Systems Projects (LPC21xx)

A cohesive collection of embedded C projects for NXP LPC2124/LPC2148 (ARM7TDMI‑S), organized by feature area. All projects target IAR Embedded Workbench for ARM.

## Folder Structure
```
embedded-systems-projects/
├── gpio-led/
│   ├── iar-blinky/          # Minimal GPIO LED toggle
│   └── workbench-blink/     # Alternative blink project
├── lcd/
│   ├── display-hello/       # 4‑bit LCD driver + Hello World demo
│   └── teachLDC-lib/        # Simple 8‑bit LCD helper library
├── interrupts/
│   └── vic/                 # Vectored Interrupt Controller setup (IRQ/FIQ)
└── adc-temperature/
    └── tempInclass/         # ADC read → LED display demo
```

## Highlights
- GPIO control, software delays, and clean init loops
- Robust 4‑bit HD44780 LCD sequence (proper 8‑>4 bit init, nibble writes, timing)
- VIC configuration with ISR installation (IRQ/FIQ) for LPC2148 class MCUs
- ADC configuration and polling, displaying conversion results on LEDs

## Noise Filter (separate repository)
The Noise Filter application (keypad input, LCD UI, typed conversions, multi‑file design) is now a standalone repository:
- Repository: https://github.com/Nebiyu46/lpc-noise-filter
- Tech: LPC21xx + IAR EW, modular C (lcd.c, keypad.c, typeConv.c, DataConv.c)

## Build (IAR EW for ARM)
1. Open the `.ewp` project inside the desired folder (e.g., `lcd/display-hello/hello_world.ewp`).
2. Select the Debug configuration.
3. Build (F7) and download to target (Ctrl+D) with J‑Link/LPC‑Link.

## Target
- LPC2124 / LPC2148 (12 MHz crystal typical)
- Peripherals used: GPIO, LCD (HD44780), VIC, ADC

## Pinning Notes (examples)
- LED example: P0.0 as output (`PINSEL0_bit.P0_0=0`, `IO0DIR_bit.P0_0=1`)
- LCD 4‑bit example: P0.4–P0.7 as data, RS/EN on GPIO, with microsecond/millisecond timing
- ADC example: configure AD0 channel, start conversion via `ADCR_bit.START`, poll `ADGDR_bit.DONE`

## Educational Value
- Clear progression from GPIO → LCD → ADC → interrupts
- Real hardware timing and register‑level programming
- Multiple implementations for comparison and learning

## License
Educational use. Verify pin mappings and timings for your specific board.
