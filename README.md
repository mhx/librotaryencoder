# librotaryencoder

Simple, small rotary encoder library

## Why?

Rotary encoder implementations suck. I've come across so many devices with
rotary encoders that are barely usable because of erratic encoders behaviour.

I don't know why there are so many bad rotary encoder implementations out
there. I know there are some good ones, too, but they have other issues
(e.g. they're bloated or specific to certain platforms).

The goals/features of this library are:

- **Easy to use, consistent interface:** provide the logic levels at the A, B
  terminals and receive back the rotation (clockwise, counter-clockwise,
  none).

- **Fully unit tested.**

- **Small code size:** simple update routine can compile down to about a
  dozen instructions on an 8-bit AVR MCU.

- **Error recovery** in case of illegal transitions without erratic rotation
  output.

- **Support arbitrarily many encoders**, using only one byte of state per
  encoder.

- **Support for half- and full-step encoders**, even combinations of both
  in the same project.

- **Support for both "simple" and debounced algorithms.** The simple
  implementation can be used if code size must be minimised by all
  means.

- **Support for both "pure C" and C++ environments.** Support for both
  "plain" and polymorphic classes in C++.

The library intentionally doesn't provide features such as rotation
speed or acceleration to keep it simple and small. Such features can
easily be added outside of the library if needed.

## Requirements

The library requires a C90 compliant C compiler and/or a C++98
compliant C++ compiler. If you want to use the polymorphic wrapper
template, a C++11 compliant compiler is required.

## How to use this library

First and foremost, in order for this library (or in fact *any*
code handling rotary encoders) to work correctly, you must avoid
losing transitions between encoder states. The simplest and most
effective way of doing this is to use edge-triggered interrupts,
and to avoid running unnecessary code in the interrupt handler.

Here's a simple, yet fully working, example of using a
[PEC11R](https://bourns.com/products/encoders/contacting-encoders/product/PEC11R)
encoder connected to pins `PA1` and `PA2` of an
[ATtiny824](https://www.microchip.com/en-us/product/ATtiny824)
microcontroller.

``` c
#include <avr/interrupt.h>
#include <avr/io.h>

#include <rotaryencoder/debounced_encoder.h>

static encoder_state es;
static volatile uint8_t encoder_value;

ISR(PORTA_PORT_vect)
{
  if (PORTA.INTFLAGS & (PIN1_bm | PIN2_bm))
  {
    PORTA.INTFLAGS = PIN1_bm | PIN2_bm; // Clear interrupt flags
    // Update encoder state
    switch (encoder_debounced_full_step_update(&es, (PORTA.IN >> 1) & 0x3))
    {
    case ENCODER_ACTION_TURN_CW:
      encoder_value++;
      break;
    case ENCODER_ACTION_TURN_CCW:
      encoder_value--;
      break;
    default:
      break;
    }
  }
}

int main(void)
{
  // Configure edge-triggered interrupts for both terminal inputs
  PORTA.PIN1CTRL = PORT_ISC_BOTHEDGES_gc; // Encoder Terminal A
  PORTA.PIN2CTRL = PORT_ISC_BOTHEDGES_gc; // Encoder Terminal B

  // Initialize encoder state
  encoder_debounced_full_step_init(&es, (PORTA.IN >> 1) & 0x3);

  sei(); // Enable interrupts

  uint8_t last_encoder_value = 0;

  for (;;)
  {
    if (encoder_value != last_encoder_value)
    {
      last_encoder_value = encoder_value;
      // Do something with last_encoder_value
    }
    else
    {
      SLPCTRL.CTRLA = SLPCTRL_SMODE_IDLE_gc | SLPCTRL_SEN_bm;
      __builtin_avr_sleep();
    }
  }

  return 0;
}
```

Most of this is boilerplate/setup code.

The code makes use of the fact that `ENCODER_TERMINAL_A` is defined
as `0x01` and `ENCODER_TERMINAL_B` is defined as `0x02`. Thus, the
`terminal` argument to both the `init` and `update` calls can be
computed by simply shifting and masking the value read from `PORTA`.

Note how the interrupt service routine only increments or decrements
the global variable `encoder_value`. This means the interrupt can be
re-triggered quickly, ensuring we're not missing any transitions.

All further processing is done in the idle loop, i.e. checking if the
encoder value has changed and acting upon the new value. This can
take much longer than the time between two interrupts without causing
any problems.

### Different encoder flavours

There are two different flavours of encoders: full-step and half-step.
Full-step encoders have detents every "full step", half-step encoders
every "half step". The two terminals of the encoder run through a
[gray code](https://en.wikipedia.org/wiki/Gray_code) sequence as follows:

```
  Full-Step Encoder                    Half-Step Encoder

    clockwise --->                       clockwise --->

      D       D           detents       D   D   D   D
      .       .                         .   .   .   .
    ___     ___       1             1   . ___   . ___
   |  .|   |  .|   |     A (bit 0)      .|  .|  .|  .|   |
 __|  .|___|  .|___|  0             0  __|  .|___|  .|___|
      .       .                         .   .   .   .
      ___     ___     1             1   .   ___ .   ___
 |   |.  |   |.  |       B (bit 1)     |.  |.  |.  |.  |
 |___|.  |___|.  |__  0             0  |___|.  |___|.  |__

 <--- counter-clockwise             <--- counter-clockwise

 ---------------------------------------------------------
        <---- counter-clockwise // clockwise ------------>
 ---------------------------------------------------------
  BA    11 <- 10 <- 00 <- 01 <- 11 -> 10 -> 00 -> 01 -> 11
 ---------------------------------------------------------
 half   ||          ||          ||          ||          ||
 ---------------------------------------------------------
 full   ||                      ||                      ||
 ---------------------------------------------------------

```

There are also encoders without detents. These work exactly the same,
but you're free to choose if you want to count full steps or half
steps.

Full-step encoders usually have their detents where both bits are
high. This is preferable for low-power applications, as both
encoder switches are off in this state, which means no current is
flowing through the pull-up resistors.

### Different strategies

This library implements two different strategies to generate encoder
events: `simple` and `debounced`.

As a rule of thumb, you should *always* use the `debounced` strategy,
unless you really need to save code memory.

The `simple` strategy will generate events at one particular transition
in either direction. This means if your encoder is stuck in a position
near this transition, it is possible that the library will generate a
lot of alternating "turn clockwise" and "turn counter-clockwise" events.
If your encoder also has a push button, pushing the button may easily
generate an unintentional event.

The `debounced` strategy will generate events at different transitions,
depending on direction, effectively implementing a hysteresis. This is
more complex (and thus requires more code), but it avoids the
aforementioned problems.

### Different implementations

For each combination of encoder flavour and strategy, the library also
offers two different implementations. One that is based on an explicit
transition table, and one that implements the logic of the transition
table in code. Both implementations behave identically.

The transition table based implementation (functions/classes with a
`_tt` suffix) all use *exactly* the same update code and different
transition tables. This *may* be beneficial if you're using different
encoder flavours in the same project, as both flavours can use the
same code.

On the other hand, the "pure code" implementation has more potential
to be optimised by the compiler, especially when using whole-program
optimisation.

### Code size

The following table shows the size of the code generated for both the
`main()` function and the interrupt service routine from the example
code earlier in this document. The code was compiled using `avr-gcc`
with whole program optimization enabled, so all code was eventually
inlined into those two functions. For the transition-table-based
implementations (`[tt]`), the size of the transition table is also
shown.

|                       | `main()` | ISR       | Table    |
| --------------------- | -------: | --------: | -------: |
| debounced/full        | 48 bytes | 130 bytes |        - |
| debounced/half        | 52 bytes | 140 bytes |        - |
| simple/full           | 50 bytes |  84 bytes |        - |
| simple/half           | 50 bytes |  82 bytes |        - |
| debounced/full `[tt]` | 48 bytes | 128 bytes | 28 bytes |
| debounced/half `[tt]` | 52 bytes | 128 bytes | 24 bytes |
| simple/full `[tt]`    | 50 bytes | 128 bytes | 16 bytes |
| simple/half `[tt]`    | 50 bytes | 128 bytes | 16 bytes |

You can see that the size of the `main()` function is identical for
the table-based and non-table-based implementations. That's because
the `init` code is identical. You can also see that the code size
of the interrupt service routine (ISR) doesn't change for the
table-based implementations. That's because the code is identical
and only the transition tables change. Last but not least, you can
see that the "pure code" implementations use less code space (the
transition tables are also stored in code memory).

### C++ wrappers

In a C++ environment, you can use wrapper classes for the C API,
for example:

``` cpp
rotaryencoder::debounced_encoder_full_step enc;

enc.init(terminal);
enc.update(terminal);
```

These wrappers have zero overhead compared to the C API.
Alternatively, you can use the `encoder_poly_wrapper` template
that implements `encoder_interface`. This incurs the usual vtable
and virtual function call overhead, but allows you to work with
interfaces instead of concrete classes:

``` cpp
using namespace rotaryencoder;

void handler(encoder_interface& ei)
{
  ei.init(terminal);
  ei.update(terminal);
}

encoder_poly_wrapper<debounced_encoder_full_step> enc;

handler(enc);
```
