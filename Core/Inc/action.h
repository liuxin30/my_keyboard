#ifndef ACTION_H
#define ACTION_H

#include <stdint.h>
#include <stdbool.h>
#include "keyboard.h"
#include "keycode.h"
#include "action_code.h"

/*
 * keyboard report is 8-byte array retains state of 8 modifiers and 6 keys.
 *
 * byte |0       |1       |2       |3       |4       |5       |6       |7
 * -----+--------+--------+--------+--------+--------+--------+--------+--------
 * desc |mods    |reserved|keys[0] |keys[1] |keys[2] |keys[3] |keys[4] |keys[5]
 *
 * It is exended to 16 bytes to retain 120keys+8mods when NKRO mode.
 *
 * byte |0       |1       |2       |3       |4       |5       |6       |7        ... |15
 * -----+--------+--------+--------+--------+--------+--------+--------+--------     +--------
 * desc |mods    |bits[0] |bits[1] |bits[2] |bits[3] |bits[4] |bits[5] |bits[6]  ... |bit[14]
 *
 * mods retains state of 8 modifiers.
 *
 *  bit |0       |1       |2       |3       |4       |5       |6       |7
 * -----+--------+--------+--------+--------+--------+--------+--------+--------
 * desc |Lcontrol|Lshift  |Lalt    |Lgui    |Rcontrol|Rshift  |Ralt    |Rgui
 *
 */
#define KEYBOARD_REPORT_KEYS 6
typedef struct {
    uint8_t mods;
    uint8_t reserved;
    uint8_t keys[KEYBOARD_REPORT_KEYS];
} __attribute__ ((packed)) report_keyboard_t;


#ifdef __cplusplus
extern "C" {
#endif

/* Execute action per keyevent */
void action_exec(keyevent_t event);

/* Utilities for actions.  */
void register_code(uint8_t code);
void unregister_code(uint8_t code);

void send_keyboard_report(void);
void clear(void);

/* key */
void add_key(uint8_t key);
void del_key(uint8_t key);
void clear_keys(void);

/* modifier */
uint8_t get_mods(void);
void add_mods(uint8_t mods);
void del_mods(uint8_t mods);
void set_mods(uint8_t mods);
void clear_mods(void);

/* inspect */
uint8_t has_anykey(void);
uint8_t has_anymod(void);
uint8_t get_first_key(void);


#ifdef __cplusplus
}
#endif

#endif  /* ACTION_H */
