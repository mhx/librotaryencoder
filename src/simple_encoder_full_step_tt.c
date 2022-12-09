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

#include <rotaryencoder/simple_encoder.h>

enum
{
  ES_P00,
  ES_P01,
  ES_P10,
  ES_P11,

  ES_E00 = ES_P00,
  ES_E01 = ES_P01,
  ES_E10 = ES_P10,
  ES_E11 = ES_P11,

  ES_CWF =
      ES_P11 | (ENCODER_ACTION_TURN_CW << ENCODER_INTERNAL_ACTION_SHIFT_TT),
  ES_CCF =
      ES_P01 | (ENCODER_ACTION_TURN_CCW << ENCODER_INTERNAL_ACTION_SHIFT_TT)
};

ENCODER_CONST_MEMORY encoder_byte_t encoder_simple_full_step_table[4][4] = {
    /* clang-format off */
    /*               00      01      10      11   */
    /* ES_P00 00 */ {ES_P00, ES_P01, ES_P10, ES_E11},
    /* ES_P01 01 */ {ES_P00, ES_P01, ES_E10, ES_CWF},
    /* ES_P10 10 */ {ES_P00, ES_E01, ES_P10, ES_P11},
    /* ES_P11 11 */ {ES_E00, ES_CCF, ES_P10, ES_P11}
    /* clang-format on */
};
