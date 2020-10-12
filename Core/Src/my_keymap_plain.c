#include "keymap.h"
#include "keycode.h"
#include "matrix.h"


static const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0: Default layer
     * ,-----------------------------------------------------------.
     * |Esc| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 | - | = | <-    |
     * |-------------------------- --------------------------------|
     * |Tab  | Q | W | E | R | T | | Y | U | I | O | P | [ | ] |Hom|
     * |-------------------------- --------------------------------|
     * |Ctrl | A | S | D | F | G | | H | J | K | L | ; | ' | \ |End|
     * |-------------------------- --------------------------------|
     * |Shift| Z | X | C | V | B | | N | M | , | . | / | ` |Up |Del|
     * |-------------------------- --------------------------------|
     * |Ctr|Gui |Alt |   Space   | |     Enter     |App|Rig|Dow|Lef|
     * `-------------------------- --------------------------------'
     */
    {
        { KC_ESC,   KC_1,   KC_2,   KC_3,   KC_4,   KC_5,  KC_6,   KC_7,   KC_8,   KC_9,   KC_0,KC_MINS, KC_EQL,KC_BSPC},
        { KC_TAB,   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,  KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,KC_LBRC,KC_RBRC,KC_HOME},
        {KC_CAPS,   KC_A,   KC_S,   KC_D,   KC_F,   KC_G,  KC_H,   KC_J,   KC_K,   KC_L,KC_SCLN,KC_QUOT,KC_BSLS, KC_END},
        {KC_LSFT,   KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,  KC_N,   KC_M,KC_COMM, KC_DOT,KC_SLSH, KC_GRV,  KC_UP, KC_DEL},
        {KC_LCTL,KC_LGUI,KC_LALT,  KC_NO, KC_SPC,  KC_NO, KC_NO, KC_ENT,  KC_NO,  KC_NO, KC_APP,KC_LEFT,KC_DOWN,KC_RGHT}
    },
    /* 1: F layer
     * ,-----------------------------------------------------------------------.
     * |NULL| F1 | F2 | F3 | F4 | F5 | F6 | F7 | F8 | F9 | F10 | F11 | F12 |   |
     * |------------------------------ ----------------------------------------|
     * |    |    |    |    |    |    | | 7  | 8  | 9  |    |    |    |    |    |
     * |------------------------------ ----------------------------------------|
     * |    |    |    |    |    |    | | 4  | 5  | 6  |    |    |    |    |    |
     * |------------------------------ ----------------------------------------|
     * |    |    |    |    |    |    | | 1  | 2  | 3  |    |    |    |    |    |
     * |------------------------------ ----------------------------------------|
     * |    |    |    |              | |     0             |App |    |    |    |
     * `------------------------------ ----------------------------------------'
     */
    {
        {KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,   KC_F6,   KC_F7,   KC_F8, KC_F9, KC_F10,KC_F11,KC_F12,KC_NO},
        {KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_KP_7, KC_KP_8, KC_KP_9, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO},
        {KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_KP_4, KC_KP_5, KC_KP_6, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO},
        {KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_KP_1, KC_KP_2, KC_KP_3, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO},
        {KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO, KC_KP_0,   KC_NO, KC_NO, KC_APP,KC_NO, KC_NO, KC_NO}
    }
};


/* converts key to action */
action_t action_for_key(uint8_t layer, keypos_t key)
{
    uint8_t keycode = keymap_key_to_keycode(layer, key);

    return keycode_to_action(keycode);
}

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
{
    return keymaps[(layer)][(key.row)][(key.col)];
}

/* translates keycode to action */
action_t keycode_to_action(uint8_t keycode)
{
    if(KC_A <= keycode && keycode <= KC_RGUI){
        return (action_t)ACTION_KEY(keycode);
    }
    else{
        return (action_t)ACTION_NO;
    }
}


