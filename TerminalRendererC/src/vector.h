#ifndef VECTOR
#define VECTOR

#include <stdbool.h>
#include <math.h>

#define DEG2RAD 3.14159265358979323846 / 180

/// @brief A 2 Dimensional Vector.
typedef struct {float x, y;} Vector2;

/// @brief A 3 Dimensional Vector.
typedef struct {float x, y, z;} Vector3;

typedef struct {
    Vector3 position, rotation;
    float znear, zfar, field_of_view;
} camera;

Vector2 Add(const Vector2 a, const Vector2 b);
Vector2 Sub(const Vector2 a, const Vector2 b);
Vector2 Mul(const Vector2 a, const float   m);
Vector2 Div(const Vector2 a, const float   m);

Vector3 Add3(const Vector3 a, const Vector3 b);
Vector3 Sub3(const Vector3 a, const Vector3 b);
Vector3 Mul3(const Vector3 a, const float   m);
Vector3 Div3(const Vector3 a, const float   m);

Vector3 Rotate(Vector3 vec, Vector3 euler);

// Z Value Of The Vectors' Cross Product.
float VectorCross(const Vector2 a, const Vector2 b, const Vector2 p);
float Distance3(const Vector3 a, const Vector3 b);

Vector3 World_To_Camera(camera cam, Vector3 p);
bool Camera_To_Screen(const camera cam, const Vector3 p, const Vector2 screen_dimensions, Vector2* ret);

#endif
