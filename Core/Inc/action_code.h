#ifndef ACTION_CODE_H
#define ACTION_CODE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Action Code Struct
 *
 * NOTE:
 * In avr-gcc bit field seems to be assigned from LSB(bit0) to MSB(bit15).
 * AVR looks like a little endian in avr-gcc.
 * Not portable across compiler/endianness?
 *
 * Byte order and bit order of 0x1234:
 *   Big endian:                Little endian:
 *   --------------------       --------------------
 *   FEDC BA98  7654 3210       0123 4567  89AB CDEF
 *   0001 0010  0011 0100       0010 1100  0100 1000
 *     0x12       0x34            0x34       0x12
 */
#define ACT_MODS 0

typedef union {
    uint16_t code;
    struct action_key {
        uint8_t  code   :8;
        uint8_t  mods   :4;
        uint8_t  kind   :4;
    } key;
} action_t;


/* action utility */
#define ACTION_NO                       { .code = 0 }
#define ACTION_TRANSPARENT              { .code = 1 }
#define ACTION(kind, param)             { .code = ((kind)<<12 | (param)) }

#define ACTION_KEY(key)                 ACTION(ACT_MODS, (key))

#ifdef __cplusplus
}
#endif

#endif 
