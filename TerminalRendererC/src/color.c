#include "color.h"

// RGB Order Is Reveresed But Fuck It ¯\_(ツ)_/¯
color_rgb IntToColor(uint32_t col) 
{
    color_rgb ret;
    ret.r = ((col >> 24) & 0xFF);
    ret.g = ((col >> 16) & 0xFF);
    ret.b = ((col >> 8 ) & 0xFF);

    return ret;
}

bool Color_IsEqual(color_rgb a, color_rgb b) {
    return (a.r == b.r) && (a.g == b.g) && (a.b == b.b);
}

color_rgb RandomColor() {
    return (color_rgb){rand() % 255, rand() % 255, rand() % 255};
}

// TODO (?): Make Inline:
color_rgb LerpColor(const color_rgb a, const color_rgb b, float t)
{
    return (color_rgb) {
        (uint8_t)(((1.0f - t) * a.r + t * b.r)),
        (uint8_t)(((1.0f - t) * a.g + t * b.g)),
        (uint8_t)(((1.0f - t) * a.b + t * b.b))
    };
}
