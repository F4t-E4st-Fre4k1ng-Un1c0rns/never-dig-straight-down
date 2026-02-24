#include <stdint.h>
#ifndef COLORS_H_INCLUDED
#define COLORS_H_INCLUDED
#define PALETTE_COLORS 7

void setPalette(int8_t index);

extern uint8_t colorPalettes[PALETTE_COLORS][48];

#endif
