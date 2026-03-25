#include "vector.h"

#define VECTOR

Vector2 Add(const Vector2 a, const Vector2 b) {
    return (Vector2){.x = a.x + b.x, .y = a.y + b.y};
}

Vector2 Sub(const Vector2 a, const Vector2 b) {
    return (Vector2){.x = a.x - b.x, .y = a.y - b.y};
}

Vector2 Mul(const Vector2 a, const float m) {
    return (Vector2){.x = a.x * m, .y = a.y * m};
}

Vector2 Div(const Vector2 a, const float m) 
{
    // if (m <= 0.0f) return (Vector2){NAN, NAN};
    return (Vector2){.x = a.x / m, .y = a.y / m};
}

Vector3 Add3(const Vector3 a, const Vector3 b) {
    return (Vector3){.x = a.x + b.x, .y = a.y + b.y, a.z + b.z};
}

Vector3 Sub3(const Vector3 a, const Vector3 b) {
    return (Vector3){.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z};
}

Vector3 Mul3(const Vector3 a, const float m) {
    return (Vector3){.x = a.x * m, .y = a.y * m, .z = a.z * m};
}

Vector3 Div3(const Vector3 a, const float m) 
{
    // if (m <= 0.0f) return (Vector3){NAN, NAN, NAN};
    return (Vector3){.x = a.x / m, .y = a.y / m, .z = a.z / m};
}

// Z Value Of The Vectors' Cross Product.
float VectorCross(const Vector2 a, const Vector2 b, const Vector2 p) 
{
    Vector2 ab = Sub(a, b);
    Vector2 ap = Sub(a, p);
    return ab.x * ap.y - ab.y * ap.x;
}

float Distance3(const Vector3 a, const Vector3 b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return sqrtf(dx * dx + dy * dy + dz * dz);    
}

Vector3 Rotate(Vector3 vec, Vector3 euler)
{
    float sinx = sin(euler.x * DEG2RAD), siny = sin(euler.y * DEG2RAD), sinz = sin(euler.z * DEG2RAD);
    float cosx = cos(euler.x * DEG2RAD), cosy = cos(euler.y * DEG2RAD), cosz = cos(euler.z * DEG2RAD);

    Vector3 fin;

    fin.x = cosy * cosz * vec.x
           - cosy * sinz * vec.y
           + siny * vec.z;

    fin.y = (sinx * siny * cosz + cosx * sinz) * vec.x
          + (-sinx * siny * sinz + cosx * cosz) * vec.y
          - sinx * cosy * vec.z;

    fin.z = (-cosx * siny * cosz + sinx * sinz) * vec.x
          + (cosx * siny * sinz + sinx * cosz) * vec.y
          + cosx * cosy * vec.z;

    return fin;
}

Vector3 World_To_Camera(camera cam, Vector3 ret)
{
    Vector3 p = Sub3(ret, cam.position);

    float sinx = sin(cam.rotation.x * DEG2RAD), siny = sin(cam.rotation.y * DEG2RAD), sinz = sin(cam.rotation.z * DEG2RAD);
    float cosx = cos(cam.rotation.x * DEG2RAD), cosy = cos(cam.rotation.y * DEG2RAD), cosz = cos(cam.rotation.z * DEG2RAD);

    float cacheCosX_1 = cosy * p.z + siny * (sinz * p.y + cosz * p.x);
    float cacheCosX_2 = cosz * p.y - sinz * p.x;

    return (Vector3) {
        .x = cosy * (sinz * p.y + cosz * p.x) - siny * p.z,
        .y = sinx * cacheCosX_1 + cosx * cacheCosX_2,
        .z = cosx * cacheCosX_1 - sinx * cacheCosX_2
    };
}

// Shoutout To The GOAT Sebastian Lague: https://youtu.be/yyJ-hdISgnw?si=nTwC0CsXQ64z2sv9 And For Some Other Stuff That Helped Me In This Project.
bool Camera_To_Screen(const camera cam, const Vector3 p, const Vector2 screen_dimensions, Vector2* ret)
{
    if (p.z < cam.znear || p.z > cam.zfar) return false;

    float rads = cam.field_of_view * DEG2RAD;
    float screenheight_world = tanf(rads * .5f) * 2;

    float pixelunits_world = -screen_dimensions.y / screenheight_world / p.z;

    Vector2 pixeloffset = Mul((Vector2){p.x, p.y}, pixelunits_world);
    *ret = Add(Mul(screen_dimensions, .5f), pixeloffset);

    return true;
}
