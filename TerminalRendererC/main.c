#include "render.h"

// Example / Test:

static camera player = {
    .position = {0, 0, -2.25f},
    .rotation = {0, 0, 0},
    
    .znear = .025f,
    .zfar  = 50.0f,
    .field_of_view = 60.0f
};

static color_rgb FogColor = {185, 185, 185};

static mesh suzanne, cube;
static Scene mainScene;
// static TriangleBuffer trizbuffer;

static double delta_time = 0;
static double total_time_elapsed = 0;

void Frame(ScreenPixelBuffer* buffer)
{
    // Some Stuff If You Wanna Try:
    // ------------------------------------------------------------------------------------------------------------------ //
    // This Was Supposed To Sort Triangles From Front To Back. However, This Results In An Insane Drop To FPS :((         //
    // Because This Isn't Really Used, You'll See On More Complex Objects - Some Triangles Rendering Infront Of Others.   //
    // Implementing An Actual Z - Buffer Isn't Really Hard, I Just Thought Making Everything Like The PS1 Is Cooler Ig..  //
    //                                                                                                                    //
    // SortTriangleBuffer(player, mainScene, &trizbuffer, 0, trizbuffer.tri_count-1);                                     //
    // RenderZbuffer(buffer, mainScene, player, trizbuffer);                                                              //
    //                                                                                                                    //
    // This Was Supposed To Change The Vertex Color Of The Meshs In Scene To Make A Fog Effect Like In                    //
    // Silent Hill 1. However, This Also Has Problems. (Not Performance Related Ones This Time Tho)                       //
    //                                                                                                                    //
    // for (size_t i = 0; i < mainScene.object_count; i++)                                                                //
    //     FogMesh(player, &mainScene.scene_objects[i], FogColor, 0.0f, 15.0f);                                           //
    // ------------------------------------------------------------------------------------------------------------------ //

    RenderMesh(buffer, player, mainScene.scene_objects[1]);
    RenderMesh(buffer, player, mainScene.scene_objects[0]);

    // Rotates Objects.
    mainScene.scene_objects[0].rotation.y -= 72.0f * delta_time;
    mainScene.scene_objects[1].rotation.y += 72.0f * delta_time;

    // Reduces To ~150 FPS From ~500 Cuz Of How The Screen Is Presented In The Terminal AND Has Issues With Colors.
    // Dither(buffer, 15.0f);
}

int main()
{
    printf("\e[1;1H\e[2J\033[?25l");

    ScreenPixelBuffer buffer = CreateScreenPixelbuffer(120, 90);
    
    // Scene Object Setup:
    suzanne = Suzanne();
    suzanne.position.z += 2;
    suzanne.position.y += 1;
    
    mainScene = InitSceneWithMesh(suzanne);

    cube = Cube();
    cube.position.x -= 2;
    cube.position.z += 4;
    
    AddMeshToScene(&mainScene, cube);

    // trizbuffer = InitBufferFromScene(mainScene);

    // Gives Every Mesh In The Scene Random Vertex Colors.
    srand(time(NULL));
    for (size_t i = 0; i < mainScene.object_count; i++)
        for (size_t j = 0; j < mainScene.scene_objects[i].verts_size; j++)
            mainScene.scene_objects[i].verts[j].color = RandomColor();

    double time_elapsed = 0;
    uint64_t frame_count = 0, fps = 0;

    // Main Loop.
    while (total_time_elapsed <= 10)
    {
        clock_t start = clock();

        printf("\033[0;0H");
        ClearBuffer(&buffer, IntToColor(0xA7A7FF00));

        Frame(&buffer);

        // Compare Between The Two If You Wanna.

        // DisplayScreen(buffer);
        RenderScreen(buffer);

        clock_t req_time = (clock() - start);
        delta_time = (double)(req_time);
        delta_time /= CLOCKS_PER_SEC;

        frame_count++;
        time_elapsed += delta_time;
        total_time_elapsed += delta_time;

        // Update FPS Every Second.
        if (time_elapsed >= 1)
        {
            fps = frame_count;
            time_elapsed = 0;
            frame_count  = 0;
        }

        // Debug Data:
        printf("\033[0mFPS: %d, Time: %.3lf, Delta Time: %lf            \n", fps, total_time_elapsed, delta_time);
    }

    printf("\033[?25h");

    // FreeTriangleBuffer(trizbuffer);

    free_meshs(2, &cube, &suzanne);
    FreeScene(&mainScene);

    FreeScreenPixelBuffer(&buffer);

    return EXIT_SUCCESS;
}
