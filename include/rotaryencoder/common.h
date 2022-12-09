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

#ifndef INCLUDE_ROTARYENCODER_COMMON_H
#define INCLUDE_ROTARYENCODER_COMMON_H

#define LIBROTARYENCODER_VERSION_MAJOR 1
#define LIBROTARYENCODER_VERSION_MINOR 0
#define LIBROTARYENCODER_VERSION_PATCH 0
#define LIBROTARYENCODER_VERSION                                               \
  (LIBROTARYENCODER_VERSION_MAJOR * 1000000L +                                 \
   LIBROTARYENCODER_VERSION_MINOR * 1000L + LIBROTARYENCODER_VERSION_PATCH)

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <stdint.h>
#elif defined(__cplusplus) && __cplusplus >= 201103L
#include <cstdint>
#endif

#define ENCODER_INTERNAL_ACTION_SHIFT_TT 4
#define ENCODER_INTERNAL_STATE_MASK_TT 0x0F

#ifndef ENCODER_CONST_MEMORY
#ifdef __AVR__
#define ENCODER_CONST_MEMORY const __flash
#else
#define ENCODER_CONST_MEMORY const
#endif
#endif

#ifndef ENCODER_INLINE
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) ||              \
    defined(__cplusplus)
#define ENCODER_INLINE inline
#elif defined(__GNUC__)
#define ENCODER_INLINE __inline__
#else
#define ENCODER_INLINE
#endif
#endif

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) ||              \
    (defined(__cplusplus) && __cplusplus >= 201103L)
typedef uint_fast8_t encoder_fast_byte_t;
typedef uint8_t encoder_byte_t;
#else
typedef unsigned char encoder_fast_byte_t;
typedef unsigned char encoder_byte_t;
#endif

enum encoder_terminal
{
  ENCODER_TERMINAL_A = (1 << 0),
  ENCODER_TERMINAL_B = (1 << 1)
};

enum encoder_action
{
  ENCODER_ACTION_NONE,
  ENCODER_ACTION_TURN_CW = 1,
  ENCODER_ACTION_TURN_CCW = 2
};

typedef encoder_byte_t encoder_state;

#ifdef __cplusplus
extern "C"
{
#endif

  enum encoder_action
  encoder_internal_update_tt(encoder_state* s, encoder_fast_byte_t terminal,
                             encoder_byte_t ENCODER_CONST_MEMORY table[][4]);

#ifdef __cplusplus
}
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L

namespace rotaryencoder
{

class encoder_interface
{
 public:
  virtual ~encoder_interface() = default;

  virtual void init(::encoder_fast_byte_t terminal) = 0;
  virtual enum ::encoder_action update(::encoder_fast_byte_t terminal) = 0;
};

template <typename Impl>
class encoder_poly_wrapper : public encoder_interface
{
 public:
  encoder_poly_wrapper() = default;

  encoder_poly_wrapper(::encoder_fast_byte_t terminal) { init(terminal); }

  void init(::encoder_fast_byte_t terminal) override final
  {
    impl_.init(terminal);
  }

  enum ::encoder_action update(::encoder_fast_byte_t terminal) override final
  {
    return impl_.update(terminal);
  }

 private:
  Impl impl_;
};

}

#endif

#endif
