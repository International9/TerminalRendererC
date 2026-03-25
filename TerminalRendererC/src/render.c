#include "render.h"

void SetForeColor(color_rgb color) {printf("\033[38;2;%d;%d;%dm", color.r, color.g, color.b);}
void SetBackColor(color_rgb color) {printf("\033[48;2;%d;%d;%dm", color.r, color.g, color.b);}

ScreenPixelBuffer CreateScreenPixelbuffer(int width, int height)
{
    ScreenPixelBuffer ret;
    
    ret.width  = width;
    ret.height = height;
    
    ret.pixels = (color_rgb*)malloc(sizeof(color_rgb) * (height * width));
    
    return ret;
}

void FreeScreenPixelBuffer(ScreenPixelBuffer* buffer)
{
    free(buffer->pixels);
    buffer->pixels = NULL;
    
    buffer->width  = 0;
    buffer->height = 0;
}

void ClearBuffer(ScreenPixelBuffer* buffer, color_rgb initValue)
{
    for (int i = 0; i < (buffer->height * buffer->width); i++) 
        buffer->pixels[i] = initValue;
}

// Not In Use, Need stb_image.h For Ts.
/*bool LoadImageToPixelbuffer(ScreenPixelBuffer* buffer, const char* imgname)
{
    int height, width;

    uint8_t* img = stbi_load(imgname, &width, &height, NULL, 3);
    if (img == NULL) return false;

    int j = 0;
    for (int i = 0; i < ((height * width) * 3); i += 3)
    {
        buffer->pixels[j].r = img[i];
        buffer->pixels[j].g = img[i+1];
        buffer->pixels[j++].b = img[i+2];
    }
    
    stbi_image_free(img);

    return true;
}*/

// !DEPRECATEDLIRAN! - Use 'RenderScreen' Instead.
// It's Still Here Only Because I Feel A Sentimental Value To This Thing.
void DisplayScreen(const ScreenPixelBuffer buffer)
{
    int area = buffer.height * buffer.width, i = 1, line = 1;
    color_rgb col_d = buffer.pixels[buffer.width], col_u = buffer.pixels[0];

    SetForeColor(col_u);
    SetBackColor(col_d);

    printf("%c", PIXEL);

    while (i < area)
    {
        if (line >= buffer.width)
        {
            putchar('\n');
            
            i += buffer.width;
            line = 0;
            
            continue;
        } line++;

        if (!Color_IsEqual(col_u, buffer.pixels[i]))
        {
            col_u = buffer.pixels[i];
            SetForeColor(col_u);
        }

        int bottomIndex = (i++) + buffer.width;
        if (bottomIndex >= area) continue;

        if (Color_IsEqual(col_d, buffer.pixels[bottomIndex]))
        {
            putchar(223);
            continue;
        }

        col_d = buffer.pixels[bottomIndex];
        SetBackColor(col_d);

        putchar(223);
    }

    printf("\033[0m");
}

void RenderScreen(const ScreenPixelBuffer buffer)
{
    // Explanation And Self Glaze:
    //
    // The Original Function And This One Too Function Works On A Technique I Originally Explained In This Video:
    // https://www.youtube.com/watch?v=6zfXM-6yPJQ <-- HERE
    //
    // ...Which I *Tried* To Copy, This Function However, Works By Concatanating ANSI Escape Sequences
    // And Upper Half Block Characters To A String Called 'result' And Then It Outputs It
    // To The Terminal In One Go Instead Of Doing It Individually For Every Pixel Like The Old
    // And Naive Algorithm. This Causes A Very Noticable Increase In Performance - A 120x90
    // Screen Used To Run At ~15 FPS With The Old Code While The Current One Manages To Easily
    // Overcome 100-200 FPS (At Least On My Machine) With A Very Cluttered Screen Containing
    // LOTS Of ANSI Escape Sequences. I Feel However That It Could Be Way Faster Then It Is Currently
    // But I'm Not The Smartest Programmer So This Is Where I'm Leaving It At This. Function Is
    // Also Very Poorly Written So Trying To Read It Without A Prior Explanation Might Be Confusing.

    int area = buffer.width * buffer.height;
    char pixel[22], result[(44*area)+1];
    
    color_rgb current_color = buffer.pixels[0];
    color_rgb current_color_bg = buffer.pixels[buffer.width];
    
    size_t total_len = SetPixelColor(pixel, current_color);
    strcpy(result, pixel);
    
    int offset = SetDownPixelColor(pixel, current_color_bg);
    memcpy(result + total_len, pixel, offset);
    
    total_len += offset;
    result[total_len++] = 223;
    
    int i = 1, line_count = 1; // Start After One Iteration.
    while (i < area)
    {
        if (line_count >= buffer.width)
        {
            result[total_len++] = '\n';
            
            i += buffer.width; // <-- This Skips Like Half Of The Rows.
            line_count = 0;
            
            continue;
        }
        
        line_count++;
        
        // Save On Your ANSI Escape Sequences.
        if (!Color_IsEqual(buffer.pixels[i], current_color))
        {
            current_color = buffer.pixels[i];
            offset = SetPixelColor(pixel, current_color);
            memcpy(result + total_len, pixel, offset);
            
            total_len += offset;
        }
        
        // Check And Set Color For Down Pixel:
        
        int down_index = (i++) + buffer.width;
        if (down_index >= area) continue; // ??? - TODO: Find A Way To Solve This.
        
        if (Color_IsEqual(buffer.pixels[down_index], current_color_bg))
        {
            result[total_len++] = 223;
            continue;
        }
        
        current_color_bg = buffer.pixels[down_index];
        offset = SetDownPixelColor(pixel, current_color_bg);
        memcpy(result + total_len, pixel, offset);
        
        total_len += offset;
        result[total_len++] = 223;
    }
    
    result[total_len] = '\0';
    puts(result);
}

int PositionToBufferIndex(ScreenPixelBuffer buffer, Vector2 p)
{
    if (p.x >= buffer.width || p.y >= buffer.height || p.x < 0 || p.y < 0) return -1;
    return (int)(p.x + p.y * buffer.width);
}

// By Standard It Means That The Center Of The Screen Is (0, 0) And Y = Up Instead Of Down - Unlike Normal Screen Coords.
int PositionToBufferIndex_Standard(ScreenPixelBuffer buffer, Vector2 p)
{
    int hw = buffer.width * .5, hh = buffer.height * .5;
    if (p.x > hw || p.y > hw || p.x < -hw || p.y < -hw) return -1;
    
    p.x += hw;
    p.y = hh - p.y;

    return floorl(p.y * buffer.width + p.x);
}

Vector2 BufferIndexToPosition(ScreenPixelBuffer buffer, int index) 
{
    if (index < 0 || index > (buffer.width * buffer.height)) return (Vector2){-1, -1};

    Vector2 fin;
    float rows = roundf(index / buffer.width);

    fin.y = rows;
    fin.x = index - (buffer.width * rows);

    return fin;
}

// VERY Expensive.
void Dither(ScreenPixelBuffer* buffer, float scale)
{
    for (int i = 0; i < (buffer->width * buffer->height); i++)
    {
        Vector2 p = BufferIndexToPosition(*buffer, i);

        // if (p.x == -1) // Something Very Wrong Has Happened, This Shouldn't Happen.

        int x = (int)p.x % 4;
        int y = (int)p.y % 4;

        float m = Dither_Matrix[x][y] / 16.0f - .5f;

        buffer->pixels[i].r += m * scale;
        buffer->pixels[i].r = CLAMP(buffer->pixels[i].r, 0, 255);

        buffer->pixels[i].g += m * scale;
        buffer->pixels[i].g = CLAMP(buffer->pixels[i].g, 0, 255);

        buffer->pixels[i].b += m * scale;
        buffer->pixels[i].b = CLAMP(buffer->pixels[i].b, 0, 255);
    }
}

// ------------ RENDERING FUNCTIONS ------------ //

// Shoutout To Ts Vid By Pikuma (luv his vids <3): https://youtu.be/k5wtuKWmV48?si=qunSZRoFOelywpJW
bool RasterizeTriangle(ScreenPixelBuffer* buffer, const TRIANGLE tri) 
{
    // Backface Culling:
    int x0 = tri.b.position.x - tri.a.position.x;
    int y0 = tri.b.position.y - tri.a.position.y;
    int x1 = tri.c.position.x - tri.a.position.x;
    int y1 = tri.c.position.y - tri.a.position.y;
    if (x0 * y1 - x1 * y0 < 0) return false;

    // Area Is Invalid.
    float area = VectorCross(tri.a.position, tri.b.position, tri.c.position);
    if (area <= 0) return false;

    int bottom = (int)(fmax(fmax(tri.a.position.y, tri.b.position.y), tri.c.position.y));
    int top    = (int)(fmin(fmin(tri.a.position.y, tri.b.position.y), tri.c.position.y));
    int right  = (int)(fmax(fmax(tri.a.position.x, tri.b.position.x), tri.c.position.x));
    int left   = (int)(fmin(fmin(tri.a.position.x, tri.b.position.x), tri.c.position.x));

    for (int y = top; y <= bottom; y++)
        for (int x = left; x <= right; x++)
        {
            Vector2 p = (Vector2){x, y}; // Current Pixel.
            
            int ind = PositionToBufferIndex(*buffer, p);
            if (ind < 0) continue;

            float c_ab = VectorCross(tri.a.position, tri.b.position, p);
            float c_bc = VectorCross(tri.b.position, tri.c.position, p);
            float c_ca = VectorCross(tri.c.position, tri.a.position, p);

            if (c_ab < 0.0f || c_bc < 0.0f || c_ca < 0.0f) continue;

            float alpha = c_ab / area;
            float beta  = c_bc / area;
            float gamma = c_ca / area;

            color_rgb interpCol = (color_rgb){
                .r = (uint8_t)(tri.a.color.r * beta + tri.b.color.r * gamma + tri.c.color.r * alpha),
                .g = (uint8_t)(tri.a.color.g * beta + tri.b.color.g * gamma + tri.c.color.g * alpha),
                .b = (uint8_t)(tri.a.color.b * beta + tri.b.color.b * gamma + tri.c.color.b * alpha),
            };

            // Deleted The Convertion To 16 Bit Color Prescision To Remove The PS1 Feel.
            buffer->pixels[ind] = interpCol; // Convert32To16(interpCol);
        }    

    return true;
}

void RenderZbuffer(ScreenPixelBuffer *buffer, const Scene scene, const camera cam, const TriangleBuffer zbuffer)
{
    TRI_VERT3 a, b, c;
    Vector2 proj_a, proj_b, proj_c;
    Vector2 dims = (Vector2){buffer->width, buffer->height};

    for (int i = zbuffer.tri_count-1; i >= 0; i--)
    { 
        mesh tri_mesh = scene.scene_objects[zbuffer.buffer_tris[i].mesh_index];
        tri tri_idx = tri_mesh.tris[zbuffer.buffer_tris[i].triangle_index];

        a = tri_mesh.verts[tri_idx.i-1];
        b = tri_mesh.verts[tri_idx.j-1];
        c = tri_mesh.verts[tri_idx.k-1];

        if (!Camera_To_Screen(cam, World_To_Camera(cam, Object_To_World(tri_mesh, a.position)), dims, &proj_a)) continue;
        if (!Camera_To_Screen(cam, World_To_Camera(cam, Object_To_World(tri_mesh, b.position)), dims, &proj_b)) continue;
        if (!Camera_To_Screen(cam, World_To_Camera(cam, Object_To_World(tri_mesh, c.position)), dims, &proj_c)) continue;

        RasterizeTriangle(buffer, (TRIANGLE) {
            (TRI_VERTEX){.position = proj_a, .color = a.color},
            (TRI_VERTEX){.position = proj_b, .color = b.color},
            (TRI_VERTEX){.position = proj_c, .color = c.color}
        });
    }
}

void RenderMesh(ScreenPixelBuffer* buffer, const camera cam, const mesh m)
{
    Vector2 proj_a, proj_b, proj_c;
    Vector2 dims = (Vector2){buffer->width, buffer->height};
    
    for (int i = 0; i < m.tri_size; i++)
    {
        TRI_VERT3 a = m.verts[m.tris[i].i-1];
        TRI_VERT3 b = m.verts[m.tris[i].j-1];
        TRI_VERT3 c = m.verts[m.tris[i].k-1];
        
        if (!Camera_To_Screen(cam, World_To_Camera(cam, Object_To_World(m, a.position)), dims, &proj_a)) continue;
        if (!Camera_To_Screen(cam, World_To_Camera(cam, Object_To_World(m, b.position)), dims, &proj_b)) continue;
        if (!Camera_To_Screen(cam, World_To_Camera(cam, Object_To_World(m, c.position)), dims, &proj_c)) continue;

        RasterizeTriangle(buffer, (TRIANGLE){
            .a = {.position = proj_a, .color = a.color},
            .b = {.position = proj_b, .color = b.color},
            .c = {.position = proj_c, .color = c.color}
        });
    }
}
