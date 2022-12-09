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

#include <stdlib.h>

#include <greatest.h>

#include <rotaryencoder/debounced_encoder.h>
#include <rotaryencoder/simple_encoder.h>

typedef void (*init_func)(encoder_state*, encoder_fast_byte_t);

typedef enum encoder_action (*update_func)(encoder_state*, encoder_fast_byte_t);

TEST compare(init_func init, update_func update, update_func update_tt)
{
  for (int i = 0; i < 100; ++i)
  {
    encoder_state es, es_tt;

    uint8_t term = random() % 4;

    init(&es, term);
    init(&es_tt, term);

    for (int k = 0; k < 1000; ++k)
    {
      term = random() % 4;

      enum encoder_action action = update(&es, term);
      enum encoder_action action_tt = update_tt(&es_tt, term);

      ASSERT_EQ_FMT((int)action, (int)action_tt, "%d");
    }
  }

  PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char** argv)
{
  GREATEST_MAIN_BEGIN();

  srandom(42);

  RUN_TESTp(compare, encoder_simple_full_step_init,
            encoder_simple_full_step_update,
            encoder_simple_full_step_update_tt);
  RUN_TESTp(compare, encoder_simple_half_step_init,
            encoder_simple_half_step_update,
            encoder_simple_half_step_update_tt);
  RUN_TESTp(compare, encoder_debounced_full_step_init,
            encoder_debounced_full_step_update,
            encoder_debounced_full_step_update_tt);
  RUN_TESTp(compare, encoder_debounced_half_step_init,
            encoder_debounced_half_step_update,
            encoder_debounced_half_step_update_tt);

  GREATEST_MAIN_END();
}
