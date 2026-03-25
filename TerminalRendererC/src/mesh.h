#ifndef MESH
#define MESH

#include "triangle.h"
#include <stdarg.h>

typedef struct {Vector3 position; color_rgb color;} TRI_VERT3; // A Mesh Vertex (Color And Object - Space Position).
typedef struct {size_t i, j, k;} tri; // i, j, And k Are Indices To Triangles In The 'verts' Array Of The Mesh.

typedef struct
{
    TRI_VERT3* verts;
    tri* tris;
    
    // For Object-To-World Space Conversion.
    Vector3 position, rotation;
    size_t verts_size, tri_size;
} mesh;

mesh Cube();
mesh Suzanne();
Vector3 Object_To_World(const mesh m, const Vector3 p);

void FogMesh(const camera cam, mesh *m, color_rgb fog_color, float fog_start, float fog_end);

void free_mesh(mesh* m); 
void free_meshs(size_t AmountToFree, ...);

#endif
