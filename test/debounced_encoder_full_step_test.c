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

#include <greatest.h>

#include <rotaryencoder/debounced_encoder.h>

typedef enum encoder_action (*update_func)(encoder_state*, encoder_fast_byte_t);

TEST basic(update_func update)
{
  encoder_state es;

  encoder_debounced_full_step_init(&es,
                                   ENCODER_TERMINAL_A | ENCODER_TERMINAL_B);

  for (int i = 0; i < 10; ++i)
  {
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_TURN_CW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  for (int i = 0; i < 20; ++i)
  {
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_TURN_CCW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  for (int i = 0; i < 10; ++i)
  {
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_TURN_CW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  PASS();
}

TEST initial_state(update_func update)
{
  {
    encoder_state es;

    encoder_debounced_full_step_init(&es, 0);

    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_NONE,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_TURN_CW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  {
    encoder_state es;

    encoder_debounced_full_step_init(&es, 0);

    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_TURN_CCW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  {
    encoder_state es;

    encoder_debounced_full_step_init(&es, ENCODER_TERMINAL_A);

    ASSERT_EQ(ENCODER_ACTION_NONE,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_TURN_CW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  {
    encoder_state es;

    encoder_debounced_full_step_init(&es, ENCODER_TERMINAL_A);

    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_TURN_CCW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  {
    encoder_state es;

    encoder_debounced_full_step_init(&es, ENCODER_TERMINAL_B);

    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_NONE,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_TURN_CW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  {
    encoder_state es;

    encoder_debounced_full_step_init(&es, ENCODER_TERMINAL_B);

    ASSERT_EQ(ENCODER_ACTION_NONE,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_TURN_CCW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  PASS();
}

TEST error(update_func update)
{
  {
    encoder_state es;

    encoder_debounced_full_step_init(&es, 0);

    ASSERT_EQ(ENCODER_ACTION_NONE,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_TURN_CW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  {
    encoder_state es;

    encoder_debounced_full_step_init(&es, ENCODER_TERMINAL_A);

    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_TURN_CW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  {
    encoder_state es;

    encoder_debounced_full_step_init(&es, ENCODER_TERMINAL_B);

    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_NONE,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_TURN_CW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  {
    encoder_state es;

    encoder_debounced_full_step_init(&es,
                                     ENCODER_TERMINAL_A | ENCODER_TERMINAL_B);

    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_NONE,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_B));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, 0));
    ASSERT_EQ(ENCODER_ACTION_NONE, update(&es, ENCODER_TERMINAL_A));
    ASSERT_EQ(ENCODER_ACTION_TURN_CW,
              update(&es, ENCODER_TERMINAL_A | ENCODER_TERMINAL_B));
  }

  PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char** argv)
{
  GREATEST_MAIN_BEGIN();

  RUN_TESTp(basic, encoder_debounced_full_step_update);
  RUN_TESTp(initial_state, encoder_debounced_full_step_update);
  RUN_TESTp(error, encoder_debounced_full_step_update);

  RUN_TESTp(basic, encoder_debounced_full_step_update_tt);
  RUN_TESTp(initial_state, encoder_debounced_full_step_update_tt);
  RUN_TESTp(error, encoder_debounced_full_step_update_tt);

  GREATEST_MAIN_END();
}
