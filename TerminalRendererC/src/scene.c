#include "scene.h"

Scene InitSceneWithMesh(mesh m)
{
    Scene ret;

    ret.scene_objects = (mesh*)malloc(sizeof(mesh));
    ret.object_count = 1;

    ret.scene_objects[0] = m;

    return ret;
}

void AddMeshToScene(Scene *scene, mesh m)
{
    scene->scene_objects = (mesh*)realloc(scene->scene_objects, sizeof(mesh) * (++scene->object_count));
    scene->scene_objects[scene->object_count-1] = m;    
}

void FreeScene(Scene *scene)
{
    for (size_t i = 0; i < scene->object_count; i++)
        free_mesh(&scene->scene_objects[i]); 
 
    free(scene->scene_objects);
    scene->scene_objects = NULL;

    scene->object_count = 0;
}

// --------------------- TRIANGLE Z BUFFER FUNCTIONS ---------------------

TriangleBuffer InitBufferFromScene(const Scene scene)
{
    TriangleBuffer ret;
    ret.tri_count = 0;
    
    // Allocating Memory:

    size_t i = 0;
    for (i = 0; i < scene.object_count; i++)
        ret.tri_count += scene.scene_objects[i].tri_size;
    
    ret.buffer_tris = (tri_z_info*)malloc(sizeof(tri_z_info) * ret.tri_count);
    if (ret.buffer_tris == NULL) return (TriangleBuffer){.buffer_tris = NULL, .tri_count = 0};

    size_t cnt = 0;
    for (i = 0; i < scene.object_count; i++)
        for (size_t j = 0; j < scene.scene_objects[i].tri_size; j++)
            ret.buffer_tris[cnt++] = (tri_z_info){i, j};

    return ret;
}

float FindMidz(const Scene scene, const camera cam, const TriangleBuffer buffer, const size_t i)
{
    mesh tri_mesh = scene.scene_objects[buffer.buffer_tris[i].mesh_index];
    tri tri_idx = tri_mesh.tris[buffer.buffer_tris[i].triangle_index];

    TRI_VERT3 a = tri_mesh.verts[tri_idx.i-1];
    TRI_VERT3 b = tri_mesh.verts[tri_idx.j-1];
    TRI_VERT3 c = tri_mesh.verts[tri_idx.k-1];
    float az = World_To_Camera(cam, Object_To_World(tri_mesh, a.position)).z;
    float bz = World_To_Camera(cam, Object_To_World(tri_mesh, b.position)).z;
    float cz = World_To_Camera(cam, Object_To_World(tri_mesh, c.position)).z;

    return (az + bz + cz) / 3.0f;
}

void swap(tri_z_info* a, tri_z_info* b) 
{
    tri_z_info temp = *a;
    *a = *b;
    *b = temp;
}

int partition(const camera cam, const Scene scene, TriangleBuffer* buffer, int low, int high) 
{
    float p = FindMidz(scene, cam, *buffer, low);
    size_t i = low, j = high;

    while (i < j) 
    {
        while (FindMidz(scene, cam, *buffer, i) <= p && i <= high - 1) i++;
        while (FindMidz(scene, cam, *buffer, j) > p && j >= low + 1) j--;

        if (i < j) swap(&buffer->buffer_tris[i], &buffer->buffer_tris[j]);
    }

    swap(&buffer->buffer_tris[low], &buffer->buffer_tris[j]);
    return j;
}

void SortTriangleBuffer(const camera cam, const Scene scene, TriangleBuffer* buffer, int low, int high)
{
    if (low < high) 
    {
        int pi = partition(cam, scene, buffer, low, high);

        SortTriangleBuffer(cam, scene, buffer, low, pi - 1);
        SortTriangleBuffer(cam, scene, buffer, pi + 1, high);
    }
}

void FreeTriangleBuffer(TriangleBuffer *buffer)
{
    free(buffer->buffer_tris);
    buffer->buffer_tris = NULL;

    buffer->tri_count = 0;
}
