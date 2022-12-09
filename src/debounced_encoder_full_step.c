/*
 * Copyright (c) Marcus Holland-Moritz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <rotaryencoder/debounced_encoder.h>

enum encoder_action
encoder_debounced_full_step_update(encoder_state* s,
                                   encoder_fast_byte_t terminal)
{
#define EU_STATE_MASK 0x3
#define EU_ZERO_STATE 0x3
#define EU_CCW_FLAG 0x80
#define EU_CCW_SHIFT 7

  encoder_fast_byte_t const state = *s & EU_STATE_MASK;

  if ((state ^ terminal) != EU_STATE_MASK)
  {
    encoder_fast_byte_t const ccw = *s & EU_CCW_FLAG;

    *s = terminal |
         (state == EU_ZERO_STATE ? (terminal & 1) << EU_CCW_SHIFT : ccw);

    if (terminal == EU_ZERO_STATE && state == (ccw ? 2 : 1))
    {
      return (enum encoder_action)(state);
    }
  }
  else
  {
    /* invalid transition, reset to zero state (11) */
    *s = EU_ZERO_STATE;
  }

  return ENCODER_ACTION_NONE;
}
