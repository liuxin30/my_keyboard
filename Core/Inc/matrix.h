#ifndef MATRIX_H
#define MATRIX_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#define MATRIX_ROWS  5
#define MATRIX_COLS  14

#if (MATRIX_COLS <= 8)
typedef  uint8_t    matrix_row_t;
#elif (MATRIX_COLS <= 16)
typedef  uint16_t   matrix_row_t;
#elif (MATRIX_COLS <= 32)
typedef  uint32_t   matrix_row_t;
#else
#error "MATRIX_COLS: invalid value"
#endif

#if (MATRIX_ROWS > 255)
#error "MATRIX_ROWS must not exceed 255"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* number of matrix rows */
uint8_t matrix_rows(void);
/* number of matrix columns */
uint8_t matrix_cols(void);
/* should be called at early stage of startup before matrix_init.(optional) */
void matrix_setup(void);
/* intialize matrix for scaning. */
void matrix_init(void);
/* scan all key states on matrix */
uint8_t matrix_scan(void);
/* whether modified from previous scan. used after matrix_scan. */
bool matrix_is_modified(void) __attribute__ ((deprecated));
/* whether a swtich is on */
bool matrix_is_on(uint8_t row, uint8_t col);
/* matrix state on row */
matrix_row_t matrix_get_row(uint8_t row);
/* print matrix for debug */
void matrix_print(void);
/* clear matrix */
void matrix_clear(void);
matrix_row_t read_cols(void);

#ifdef __cplusplus
}
#endif

#endif
