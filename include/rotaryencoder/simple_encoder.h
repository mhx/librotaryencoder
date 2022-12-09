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

#ifndef INCLUDE_ROTARYENCODER_SIMPLE_ENCODER_H
#define INCLUDE_ROTARYENCODER_SIMPLE_ENCODER_H

#include <rotaryencoder/common.h>

#ifdef __cplusplus
extern "C"
{
#endif

  extern ENCODER_CONST_MEMORY encoder_byte_t
      encoder_simple_full_step_table[4][4];
  extern ENCODER_CONST_MEMORY encoder_byte_t
      encoder_simple_half_step_table[4][4];

  static ENCODER_INLINE void
  encoder_simple_full_step_init(encoder_state* s, encoder_fast_byte_t terminal)
  {
    *s = terminal;
  }

  static ENCODER_INLINE void
  encoder_simple_half_step_init(encoder_state* s, encoder_fast_byte_t terminal)
  {
    *s = terminal;
  }

  enum encoder_action
  encoder_simple_full_step_update(encoder_state* s,
                                  encoder_fast_byte_t terminal);

  static ENCODER_INLINE enum encoder_action
  encoder_simple_full_step_update_tt(encoder_state* s,
                                     encoder_fast_byte_t terminal)
  {
    return encoder_internal_update_tt(s, terminal,
                                      encoder_simple_full_step_table);
  }

  enum encoder_action
  encoder_simple_half_step_update(encoder_state* s,
                                  encoder_fast_byte_t terminal);

  static ENCODER_INLINE enum encoder_action
  encoder_simple_half_step_update_tt(encoder_state* s,
                                     encoder_fast_byte_t terminal)
  {
    return encoder_internal_update_tt(s, terminal,
                                      encoder_simple_half_step_table);
  }

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace rotaryencoder
{

class simple_encoder_full_step
{
 public:
#if __cplusplus >= 201103L
  simple_encoder_full_step() = default;
#else
  simple_encoder_full_step() {}
#endif

  simple_encoder_full_step(::encoder_fast_byte_t terminal) { init(terminal); }

  void init(::encoder_fast_byte_t terminal)
  {
    ::encoder_simple_full_step_init(&s_, terminal);
  }

  enum ::encoder_action update(::encoder_fast_byte_t terminal)
  {
    return ::encoder_simple_full_step_update(&s_, terminal);
  }

 private:
  encoder_state s_;
};

class simple_encoder_full_step_tt
{
 public:
#if __cplusplus >= 201103L
  simple_encoder_full_step_tt() = default;
#else
  simple_encoder_full_step_tt() {}
#endif

  simple_encoder_full_step_tt(::encoder_fast_byte_t terminal)
  {
    init(terminal);
  }

  void init(::encoder_fast_byte_t terminal)
  {
    ::encoder_simple_full_step_init(&s_, terminal);
  }

  enum ::encoder_action update(::encoder_fast_byte_t terminal)
  {
    return ::encoder_simple_full_step_update_tt(&s_, terminal);
  }

 private:
  encoder_state s_;
};

class simple_encoder_half_step
{
 public:
#if __cplusplus >= 201103L
  simple_encoder_half_step() = default;
#else
  simple_encoder_half_step() {}
#endif

  simple_encoder_half_step(::encoder_fast_byte_t terminal) { init(terminal); }

  void init(::encoder_fast_byte_t terminal)
  {
    ::encoder_simple_half_step_init(&s_, terminal);
  }

  enum ::encoder_action update(::encoder_fast_byte_t terminal)
  {
    return ::encoder_simple_half_step_update(&s_, terminal);
  }

 private:
  encoder_state s_;
};

class simple_encoder_half_step_tt
{
 public:
#if __cplusplus >= 201103L
  simple_encoder_half_step_tt() = default;
#else
  simple_encoder_half_step_tt() {}
#endif

  simple_encoder_half_step_tt(::encoder_fast_byte_t terminal)
  {
    init(terminal);
  }

  void init(::encoder_fast_byte_t terminal)
  {
    ::encoder_simple_half_step_init(&s_, terminal);
  }

  enum ::encoder_action update(::encoder_fast_byte_t terminal)
  {
    return ::encoder_simple_half_step_update_tt(&s_, terminal);
  }

 private:
  encoder_state s_;
};

}
#endif

#endif
