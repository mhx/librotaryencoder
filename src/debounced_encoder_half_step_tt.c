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

enum
{
  ES_S000,
  ES_N001,
  ES_N010,
  ES_S011,
  ES_P101,
  ES_P110,

  ES_S100 = ES_S000,
  ES_S111 = ES_S011,

  ESERR11 = ES_S011,
  ESERR00 = ES_S000,
  ESERRxx = ES_S011,
  CW_S000 =
      ES_S000 | (ENCODER_ACTION_TURN_CW << ENCODER_INTERNAL_ACTION_SHIFT_TT),
  CW_S011 =
      ES_S011 | (ENCODER_ACTION_TURN_CW << ENCODER_INTERNAL_ACTION_SHIFT_TT),
  CC_S100 =
      ES_S100 | (ENCODER_ACTION_TURN_CCW << ENCODER_INTERNAL_ACTION_SHIFT_TT),
  CC_S111 =
      ES_S111 | (ENCODER_ACTION_TURN_CCW << ENCODER_INTERNAL_ACTION_SHIFT_TT)
};

ENCODER_CONST_MEMORY encoder_byte_t encoder_debounced_half_step_table[6][4] = {
    /* clang-format off */
    /*             00       01       10       11    */
    /* ES_S000 */ {ES_S100, ES_N001, ES_P110, ESERR11},    /*       E // P N P N */
    /* ES_N001 */ {ES_S000, ES_N001, ESERRxx, CW_S011},    /*     E   // N N N N */
    /* ES_N010 */ {CW_S000, ESERRxx, ES_N010, ES_S011},    /*   E     // N N N N */
    /* ES_S011 */ {ESERR00, ES_P101, ES_N010, ES_S011},    /* E       // N P N P */

    /* ES_S100    {ES_S100, ES_N001, ES_P110, ESERR11}  */ /*       E // P N P N */
    /* ES_P101 */ {CC_S100, ES_P101, ESERRxx, ES_S111},    /*     E   // P P P P */
    /* ES_P110 */ {ES_S100, ESERRxx, ES_P110, CC_S111}     /*   E     // P P P P */
    /* ES_S111    {ESERR00, ES_P101, ES_N010, ES_S111}  */ /* E       // N P N P */
    /* clang-format on */
};
