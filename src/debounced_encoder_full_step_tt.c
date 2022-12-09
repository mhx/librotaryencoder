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
  ES_N000,
  ES_N001,
  ES_N010,
  ES_S011,
  ES_P100,
  ES_P101,
  ES_P110,
  ES_P111 = ES_S011,

  ESERROR = ES_S011,
  CW_S011 =
      ES_S011 | (ENCODER_ACTION_TURN_CW << ENCODER_INTERNAL_ACTION_SHIFT_TT),
  CC_P111 =
      ES_P111 | (ENCODER_ACTION_TURN_CCW << ENCODER_INTERNAL_ACTION_SHIFT_TT)
};

ENCODER_CONST_MEMORY encoder_byte_t encoder_debounced_full_step_table[7][4] = {
    /* clang-format off */
    /*             00       01       10       11    */
    /* ES_N000 */ {ES_N000, ES_N001, ES_N010, ESERROR},    /*       E // N N N N */
    /* ES_N001 */ {ES_N000, ES_N001, ESERROR, CW_S011},    /*     E   // N N N N */
    /* ES_N010 */ {ES_N000, ESERROR, ES_N010, ES_S011},    /*   E     // N N N N */
    /* ES_S011 */ {ESERROR, ES_P101, ES_N010, ES_S011},    /* E       // N P N P */

    /* ES_P100 */ {ES_P100, ES_P101, ES_P110, ESERROR},    /*       E // P P P P */
    /* ES_P101 */ {ES_P100, ES_P101, ESERROR, ES_P111},    /*     E   // P P P P */
    /* ES_P110 */ {ES_P100, ESERROR, ES_P110, CC_P111}     /*   E     // P P P P */
    /* ES_P111    {ESERROR, ES_P101, ES_N010, ES_P111}  */ /* E       // N P N P */
    /* clang-format on */
};
