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

#if __cplusplus >= 201103L
#define GREATEST_VA_ARGS
#endif

#include <greatest.h>

#include <rotaryencoder/debounced_encoder.h>
#include <rotaryencoder/simple_encoder.h>

TEST cpp_simple_full()
{
  encoder_fast_byte_t term = ::random() % 4;

  rotaryencoder::simple_encoder_full_step enc(term);
  rotaryencoder::simple_encoder_full_step_tt enc_tt(term);

  for (int k = 0; k < 1000; ++k)
  {
    term = ::random() % 4;

    enum encoder_action action = enc.update(term);
    enum encoder_action action_tt = enc_tt.update(term);

    ASSERT_EQ_FMT(static_cast<int>(action), static_cast<int>(action_tt), "%d");
  }

  PASS();
}

TEST cpp_simple_half()
{
  encoder_fast_byte_t term = ::random() % 4;

  rotaryencoder::simple_encoder_half_step enc(term);
  rotaryencoder::simple_encoder_half_step_tt enc_tt(term);

  for (int k = 0; k < 1000; ++k)
  {
    term = ::random() % 4;

    enum encoder_action action = enc.update(term);
    enum encoder_action action_tt = enc_tt.update(term);

    ASSERT_EQ_FMT(static_cast<int>(action), static_cast<int>(action_tt), "%d");
  }

  PASS();
}

TEST cpp_debounced_full()
{
  encoder_fast_byte_t term = ::random() % 4;

  rotaryencoder::debounced_encoder_full_step enc(term);
  rotaryencoder::debounced_encoder_full_step_tt enc_tt(term);

  for (int k = 0; k < 1000; ++k)
  {
    term = ::random() % 4;

    enum encoder_action action = enc.update(term);
    enum encoder_action action_tt = enc_tt.update(term);

    ASSERT_EQ_FMT(static_cast<int>(action), static_cast<int>(action_tt), "%d");
  }

  PASS();
}

TEST cpp_debounced_half()
{
  encoder_fast_byte_t term = ::random() % 4;

  rotaryencoder::debounced_encoder_half_step enc(term);
  rotaryencoder::debounced_encoder_half_step_tt enc_tt(term);

  for (int k = 0; k < 1000; ++k)
  {
    term = ::random() % 4;

    enum encoder_action action = enc.update(term);
    enum encoder_action action_tt = enc_tt.update(term);

    ASSERT_EQ_FMT(static_cast<int>(action), static_cast<int>(action_tt), "%d");
  }

  PASS();
}

#if __cplusplus >= 201103L

TEST cpp_compare_poly(rotaryencoder::encoder_interface& e1,
                      rotaryencoder::encoder_interface& e2)
{
  encoder_fast_byte_t term = ::random() % 4;

  e1.init(term);
  e2.init(term);

  for (int k = 0; k < 1000; ++k)
  {
    term = ::random() % 4;

    auto action1 = e1.update(term);
    auto action2 = e2.update(term);

    ASSERT_EQ_FMT(static_cast<int>(action1), static_cast<int>(action2), "%d");
  }

  PASS();
}

#endif

GREATEST_MAIN_DEFS();

int main(int argc, char** argv)
{
  GREATEST_MAIN_BEGIN();

  srandom(42);

  RUN_TEST(cpp_simple_full);
  RUN_TEST(cpp_simple_half);
  RUN_TEST(cpp_debounced_full);
  RUN_TEST(cpp_debounced_half);

#if __cplusplus >= 201103L

  using namespace rotaryencoder;

  {
    encoder_poly_wrapper<simple_encoder_full_step> enc;
    encoder_poly_wrapper<simple_encoder_full_step_tt> enc_tt;

    RUN_TESTp(cpp_compare_poly, enc, enc_tt);
  }

  {
    encoder_poly_wrapper<simple_encoder_half_step> enc;
    encoder_poly_wrapper<simple_encoder_half_step_tt> enc_tt;

    RUN_TESTp(cpp_compare_poly, enc, enc_tt);
  }

  {
    encoder_poly_wrapper<debounced_encoder_full_step> enc;
    encoder_poly_wrapper<debounced_encoder_full_step_tt> enc_tt;

    RUN_TESTp(cpp_compare_poly, enc, enc_tt);
  }

  {
    encoder_poly_wrapper<debounced_encoder_half_step> enc;
    encoder_poly_wrapper<debounced_encoder_half_step_tt> enc_tt;

    RUN_TESTp(cpp_compare_poly, enc, enc_tt);
  }

#endif

  GREATEST_MAIN_END();
}
