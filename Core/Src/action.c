#include "keycode.h"
#include "keyboard.h"
#include "action.h"
#include "keymap.h"
#include "usbd_hid.h"


#define RO_ADD(a, b) ((a + b) % KEYBOARD_REPORT_KEYS)
#define RO_SUB(a, b) ((a - b + KEYBOARD_REPORT_KEYS) % KEYBOARD_REPORT_KEYS)
#define RO_INC(a) RO_ADD(a, 1)
#define RO_DEC(a) RO_SUB(a, 1)
static int8_t cb_head = 0;
static int8_t cb_tail = 0;
static int8_t cb_count = 0;
static uint8_t layer = 0;

// TODO: pointer variable is not needed
//report_keyboard_t keyboard_report = {};
static report_keyboard_t *keyboard_report = &(report_keyboard_t){};

extern USBD_HandleTypeDef hUsbDeviceFS;

void send_keyboard_report(void) {
    USBD_HID_SendReport(&hUsbDeviceFS, keyboard_report, 8);
}

void switch_layer(void){
    if (layer == 0 ){
        layer = 1;
    }
    else{
        layer = 0;
    }
}

uint8_t action_exec(keyevent_t event)
{

    if (IS_NOEVENT(event)) { return; }

    uint8_t code = keymap_key_to_keycode(layer, event.key);
    //dprint("ACTION: "); debug_action(action);

    if (code == KC_APP){
        switch_layer();
        return 0;
    }

    if (event.pressed) {
        register_code(code);
    }
    else {
        unregister_code(code);
    }
    return 1;
}


/*
 * Utilities for actions.
 */
void register_code(uint8_t code)
{
    if (code == KC_NO) {
        return;
    }
    else if IS_KEY(code) {
        add_key(code);
    }
    else if IS_MOD(code) {
        add_mods(MOD_BIT(code));
    }
}

void unregister_code(uint8_t code)
{
    if (code == KC_NO) {
        return;
    }
    else if IS_KEY(code) {
        del_key(code);
    }
    else if IS_MOD(code) {
        del_mods(MOD_BIT(code));
    }
}

/* local functions */
void add_key(uint8_t code)
{
    int8_t i = cb_head;
    int8_t empty = -1;
    if (cb_count) {
        do {
            if (keyboard_report->keys[i] == code) {
                return;
            }
            if (empty == -1 && keyboard_report->keys[i] == 0) {
                empty = i;
            }
            i = RO_INC(i);
        } while (i != cb_tail);
        if (i == cb_tail) {
            if (cb_tail == cb_head) {
                // buffer is full
                if (empty == -1) {
                    // pop head when has no empty space
                    cb_head = RO_INC(cb_head);
                    cb_count--;
                }
                else {
                    // left shift when has empty space
                    uint8_t offset = 1;
                    i = RO_INC(empty);
                    do {
                        if (keyboard_report->keys[i] != 0) {
                            keyboard_report->keys[empty] = keyboard_report->keys[i];
                            keyboard_report->keys[i] = 0;
                            empty = RO_INC(empty);
                        }
                        else {
                            offset++;
                        }
                        i = RO_INC(i);
                    } while (i != cb_tail);
                    cb_tail = RO_SUB(cb_tail, offset);
                }
            }
        }
    }
    // add to tail
    keyboard_report->keys[cb_tail] = code;
    cb_tail = RO_INC(cb_tail);
    cb_count++;
}

void del_key(uint8_t code)
{
    uint8_t i = cb_head;
    if (cb_count) {
        do {
            if (keyboard_report->keys[i] == code) {
                keyboard_report->keys[i] = 0;
                cb_count--;
                if (cb_count == 0) {
                    // reset head and tail
                    cb_tail = cb_head = 0;
                    break;
                }
                if (i == cb_head) {
                    // left shift when next to tail
                    do {
                        if (keyboard_report->keys[RO_INC(cb_head)] != 0) {
                            break;
                        }
                        cb_head = RO_INC(cb_head);
                    } while (cb_tail != cb_head);
                    break;
                }
                if (i == RO_DEC(cb_tail)) {
                    // left shift when next to tail
                    do {
                        cb_tail = RO_DEC(cb_tail);
                        if (keyboard_report->keys[RO_DEC(cb_tail)] != 0) {
                            break;
                        }
                    } while (cb_tail != cb_head);
                    break;
                }
            }
            i = RO_INC(i);
        } while (i != cb_tail);
    }
}

/* modifier */
void add_mods(uint8_t mods) {
	keyboard_report->mods |= mods;
}
void del_mods(uint8_t mods) {
	keyboard_report->mods ^= mods;
}
