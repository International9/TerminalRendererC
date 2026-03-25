#ifndef TRI
#define TRI

#include "vector.h"
#include "color.h"

// A File For Handling A Bunch Of Triangle Definitions.. Nothing Else...

typedef struct {Vector2 position; color_rgb color;} TRI_VERTEX;
typedef struct {TRI_VERTEX a, b, c;} TRIANGLE;

#endif