#ifndef SCENE
#define SCENE

#include "mesh.h"

typedef struct
{
    mesh* scene_objects;
    size_t object_count;
} Scene;

typedef struct {size_t mesh_index, triangle_index;} tri_z_info;

// Orders Triangle From Back To Front.
// Should've Been A Linked List Like The How The ACTUAL PS1 Did It.
typedef struct
{
    tri_z_info* buffer_tris;
    size_t tri_count;
} TriangleBuffer;

Scene InitSceneWithMesh(mesh m);
void AddMeshToScene(Scene* scene, mesh m);
void FreeScene(Scene* scene);

// --------------------- TRIANGLE Z BUFFER FUNCTIONS ---------------------

TriangleBuffer InitBufferFromScene(const Scene scene);

// Quick Sort Algorithm Stolen Straight From Geeks4Geeks <3

float FindMidz(const Scene scene, const camera cam, const TriangleBuffer buffer, const size_t i);
void swap(tri_z_info* a, tri_z_info* b);
int partition(const camera cam, const Scene scene, TriangleBuffer* buffer, int low, int high);
void SortTriangleBuffer(const camera cam, const Scene scene, TriangleBuffer* buffer, int low, int high);

void FreeTriangleBuffer(TriangleBuffer* buffer);

#endif
