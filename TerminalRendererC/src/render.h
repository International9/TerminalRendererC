#ifndef RENDER
#define RENDER

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

// #include "stb_image.h" <-- Used To Exist But Now Gone/
#include "vector.h"
#include "scene.h"
#include "color.h"

static const uint8_t PIXEL = 223;

typedef struct {
    size_t height, width;
    color_rgb* pixels;
} ScreenPixelBuffer;

static const int Dither_Matrix[4][4] = {
    {-4, 0, -3, 1},
    {2, -2, 3, -1},
    {-3, 1, -4, 0},
    {3, -1, 2, -2},
};

void SetForeColor(color_rgb color);
void SetBackColor(color_rgb color);

void ClearBuffer(ScreenPixelBuffer* buffer, color_rgb initValue);

ScreenPixelBuffer CreateScreenPixelbuffer(int width, int height);
void FreeScreenPixelBuffer(ScreenPixelBuffer* buffer);

// bool LoadImageToPixelbuffer(ScreenPixelBuffer* buffer, const char* imgname);

// DEPRECATEDLIRAN! Use 'RenderScreen' Instead.
void DisplayScreen(const ScreenPixelBuffer buffer);

// Prints The ANSI Escape Sequence For Changing Foreground Color Of The Text In The Terminal.
static inline int SetPixelColor(char* ret, color_rgb col) {
    return sprintf(ret, "\033[38;2;%d;%d;%dm", col.r, col.g, col.b);
}

// Prints The ANSI Escape Sequence For Changing Background Color Of The Text In The Terminal.
static inline int SetDownPixelColor(char* ret, color_rgb col) {
    return sprintf(ret, "\033[48;2;%d;%d;%dm", col.r, col.g, col.b);
}

// Like A Billion Times Faster From The OG Implementation Because I'm Fucking GOATED.
void RenderScreen(const ScreenPixelBuffer buffer);

int PositionToBufferIndex(ScreenPixelBuffer buffer, Vector2 p);
int PositionToBufferIndex_Standard(ScreenPixelBuffer buffer, Vector2 p);
Vector2 BufferIndexToPosition(ScreenPixelBuffer buffer, int index);

// VERY Expensive.
void Dither(ScreenPixelBuffer* buffer, float scale);

// ------------ RENDERING FUNCTIONS ------------ //

bool RasterizeTriangle(ScreenPixelBuffer* buffer, const TRIANGLE tri);
void RenderZbuffer(ScreenPixelBuffer* buffer, const Scene scene, const camera cam, const TriangleBuffer zbuffer);
void RenderMesh(ScreenPixelBuffer* buffer, const camera cam, const mesh m);

#endif
