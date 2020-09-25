#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdint.h>
#include <stdbool.h>
#include "keyboard.h"
#include "action_code.h"

#ifdef __cplusplus
extern "C" {
#endif

/* converts key to action */
action_t action_for_key(uint8_t layer, keypos_t key);

/* translates keycode to action */
action_t keycode_to_action(uint8_t keycode);

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key);

#ifdef __cplusplus
}
#endif

#endif
