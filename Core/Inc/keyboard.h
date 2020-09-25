#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* key matrix position */
typedef struct {
    uint8_t col;
    uint8_t row;
} keypos_t;

/* key event */
typedef struct {
    keypos_t key;
    bool     pressed;
    uint16_t time;
} keyevent_t;



/* equivalent test of keypos_t */
#define KEYEQ(keya, keyb)       ((keya).row == (keyb).row && (keya).col == (keyb).col)

/* Rules for No Event:
 * 1) (time == 0) to handle (keyevent_t){} as empty event
 * 2) Matrix(255, 255) to make TICK event available
 */
static inline bool IS_NOEVENT(keyevent_t event) { return event.time == 0 || (event.key.row == 255 && event.key.col == 255); }
static inline bool IS_PRESSED(keyevent_t event) { return (!IS_NOEVENT(event) && event.pressed); }
static inline bool IS_RELEASED(keyevent_t event) { return (!IS_NOEVENT(event) && !event.pressed); }

/* Tick event */
#define TICK                    (keyevent_t){           \
    .key = (keypos_t){ .row = 255, .col = 255 },        \
    .pressed = false,                                   \
    .time = (HAL_GetTick() | 1)                          \
}


/* it runs once at early stage of startup before keyboard_init. */
void keyboard_setup(void);
/* it runs once after initializing host side protocol, debug and MCU peripherals. */
void keyboard_init(void);
/* it runs repeatedly in main loop */
void keyboard_task(void);
/* it runs when host LED status is updated */
void keyboard_set_leds(uint8_t leds);

#ifdef __cplusplus
}
#endif

#endif
