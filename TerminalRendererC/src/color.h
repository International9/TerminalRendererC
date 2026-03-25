#ifndef COLOR
#define COLOR

#define CLAMP(value, min, max) \
    ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {uint8_t r, g, b;} color_rgb;

static const color_rgb RED = {255, 0, 0};
static const color_rgb GREEN = {0, 255, 0};
static const color_rgb BLUE = {0, 0, 255};
static const color_rgb WHITE = {255, 255, 255};
static const color_rgb BLACK = {0, 0, 0};

color_rgb IntToColor(uint32_t col);

bool Color_IsEqual(color_rgb a, color_rgb b);

color_rgb RandomColor();

static inline color_rgb Convert32To16(color_rgb col)
{
    uint8_t r = col.r / 8;
    uint8_t g = col.g / 8;
    uint8_t b = col.b / 8;

    return (color_rgb) {
        CLAMP(r, 0, 31) * 8,
        CLAMP(g, 0, 31) * 8,
        CLAMP(b, 0, 31) * 8
    };
}

color_rgb LerpColor(const color_rgb a, const color_rgb b, float t);

#endif
