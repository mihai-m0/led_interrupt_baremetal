# Button-Controlled LED with External Interrupt (INT0)

Toggles the on-board LED on every falling edge of a button connected to PD2, using the INT0 external interrupt. After setup, `main()` is an empty loop: all behavior is interrupt-driven.

**Target:** ATmega328P (Arduino Uno), plain avr-gcc, direct register access, no Arduino libraries.

## Hardware

| Part | Connection |
|---|---|
| Button | PD2 (Arduino pin 2) to GND, no external resistor |
| LED | PB5 (Arduino pin 13, on-board) |

## How it works

1. PD2 is configured as an input with the internal pull-up (`DDRD` bit = 0, `PORTD` bit = 1). The pin idles high; a press shorts it to GND, so a press is a falling edge.
2. `EICRA` bits `ISC01:ISC00 = 10` select falling-edge triggering. A matching edge sets the flag `INTF0` in `EIFR`.
3. The interrupt runs only if both enables are set: the per-source enable (`EIMSK`, bit `INT0`) and the global enable (`sei()`, SREG bit I).
4. When the CPU takes the interrupt, it pushes the PC, clears the I bit, and jumps through the vector table to `ISR(INT0_vect)`. The hardware clears `INTF0` on entry.
5. The ISR writes a 1 to `PINB` bit 5, which toggles `PORTB` bit 5 in a single instruction.

## Registers used

| Register | Purpose |
|---|---|
| `DDRB` | PB5 as output (LED) |
| `DDRD`, `PORTD` | PD2 as input with pull-up |
| `EICRA` | Edge selection for INT0 |
| `EIMSK` | Per-source enable for INT0 |
| `EIFR` | Pending flag `INTF0` (hardware-managed) |
| `SREG` (bit I) | Global interrupt enable, via `sei()` |
| `PINB` | Write-1-to-toggle `PORTB` |

## Observed behavior

The LED toggles on button presses, but inconsistently: sometimes it stays on only while the button is held. This is contact bounce. The contacts open and close several times within a few milliseconds, producing multiple falling edges, and each one runs the ISR (including on release). The interrupt is working correctly; it reports every edge it sees. Debouncing is handled in project 02.

## Concepts demonstrated

- What an interrupt is: a hardware flag plus two enable levels
- Interrupt entry sequence and the vector table
- `ISR()` macro and `sei()`
- Edge selection and internal pull-up resistors
- Level trigger vs. edge trigger (a low-level trigger would re-fire continuously while the button is held)

## Build and flash

```
make
make flash PORT=COM5
```

## References

- ATmega328P datasheet (DS40002061): "External Interrupts", "I/O-Ports", "Interrupts"
- avr-libc manual: `<avr/interrupt.h>`
