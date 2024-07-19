/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "util.h"
#include "serial.h"
#include "matrix.h"
#include "debug.h"
#include "scancodes.h"

static uint8_t matrix[MATRIX_ROWS];
static bool is_modified = false;

bool is_pressed(uint8_t idx)
{
  return (matrix[MATRIX_ROW(idx)] >> MATRIX_COL(idx)) & 0x01;
}

void matrix_reset(bool full)
{
    // get state of locking keys
    bool intl = is_pressed(SICOMP_INTL_IDX);
    bool caps = is_pressed(SICOMP_CAPS_IDX);

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    // if we don't do a full reset, restore state of locking keys
    if (!full) {
        matrix[MATRIX_ROW(SICOMP_INTL_IDX)] |= (intl<<MATRIX_COL(SICOMP_INTL_IDX));
        matrix[MATRIX_ROW(SICOMP_CAPS_IDX)] |= (caps<<MATRIX_COL(SICOMP_CAPS_IDX));
    }
}

void matrix_init(void)
{
    is_modified = false;
    matrix_reset(true);

    _delay_ms(500); // wait for keyboard to start up
    serial_init();

    return;
}

uint8_t matrix_scan(void)
{
    if (is_modified) {
      matrix_reset(false);
      is_modified = false;
      return 0;
    }

    int16_t code = serial_recv2();
    if (code == -1) {
        return 0;
    }

    dprintf("%02X\n", code);
    scancode_t parsed = parse_scancode(code, is_pressed(SICOMP_INTL_IDX));

    if (parsed.key != -1) {
        matrix[MATRIX_ROW(parsed.key)] ^= (1<<MATRIX_COL(parsed.key));
        if (parsed.shift)
            matrix[MATRIX_ROW(SICOMP_SHFT_IDX)] |= (1<<MATRIX_COL(SICOMP_SHFT_IDX));
        if (parsed.ctrl)
            matrix[MATRIX_ROW(SICOMP_CTRL_IDX)] |= (1<<MATRIX_COL(SICOMP_CTRL_IDX));
        is_modified = true;
    }

    return code;
}

inline
uint8_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}
