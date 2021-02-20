#include <stdint.h>
#include "keyboard.h"
#include "matrix.h"
#include "keymap.h"
#include "keycode.h"
#include "action.h"
#include "usbd_hid.h"

void keyboard_setup(void)
{
    matrix_setup();
}

void keyboard_init(void)
{
    matrix_init();
}

/*
 * Do keyboard routine jobs: scan matrix, light LEDs, ...
 * This is repeatedly called as fast as possible.
 */
void keyboard_task(void)
{
	static matrix_row_t matrix_prev[MATRIX_ROWS];
    matrix_row_t matrix_row = 0;
    matrix_row_t matrix_change = 0;
    uint8_t is_changed = 0;
    uint8_t ret = 0;

    matrix_scan();
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {

        matrix_row = matrix_get_row(r);

        matrix_change = matrix_row ^ matrix_prev[r];
        if (matrix_change) {
        	is_changed = 1;

            matrix_row_t col_mask = 1;
            for (uint8_t c = 0; c < MATRIX_COLS; c++, col_mask <<= 1 ) {
                if (matrix_change & col_mask) {
                    keyevent_t e = (keyevent_t){
                        .key = (keypos_t){ .row = r, .col = c },
                        .pressed = (matrix_row & col_mask),
                        .time = 1/* time should not be 0 */
                    };
                    ret = action_exec(e);
                    if (ret==0){
                        break;
                    }

                    // record a processed key
                    matrix_prev[r] ^= col_mask;
                }
            }
        }
    }
    if (is_changed == 1 && ret == 1){
    	send_keyboard_report();
    	is_changed = 0;
    }
}
