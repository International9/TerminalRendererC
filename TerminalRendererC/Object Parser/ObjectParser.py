# This Rushed Mess Takes A .obj And Turns It Into A C Function Fitting To The Renderer
# But Puts It Inside Of A File Named 'Object Output.txt'. Copy It's Contexts After
# Running This Script And Paste It Into 'mesh.c' While Also Declaring It In 'mesh.h'.

file = open("Friend.obj").read()

name = ''
malloc_verts = 0
malloc_tris  = 0
verts = []
tris  = []

# Parser:
for i in range(len(file)):
    if (file[i] == 'o' and file[i+1] == ' '):
        j = i+2

        while (file[j] != '\n'):
            name += file[j]
            j += 1

    cur_vec = []
    if (file[i] == 'v' and file[i+1] == ' '):
        cur = ""
        j = i+2

        for _ in range(3):
            while (file[j] != " " and file[j] != "\n"):
                cur += file[j]
                j += 1
            j += 1

            cur_vec.append(float(cur))
            cur = ""

        malloc_verts += 1
        verts.append(cur_vec)

    cur_tri = []

    if (file[i] == 'f'):
        j = i+2
        cur = ""
        t = True

        while (file[j] != '\n'):
            if (t == True):
                cur += file[j]
                j += 1

                if (file[j] == '/'):
                    cur_tri.append(int(cur))
                    cur = ""
                    t = False

            else:
                if (file[j] == " "):
                    t = True
                    j += 1 
                else:
                    j += 1
                
        malloc_tris += 1
        tris.append(cur_tri)

# Reverse From CCW To CW:
for i in range(len(tris)):
    var = tris[i][2]
    tris[i][2] = tris[i][1]
    tris[i][1] = var

name = name.lower()

# Puts Everything In 'Object Output.txt'
with(open("Object Output.txt", 'w') as file):
    file.write(f"mesh {name.capitalize()}()\n")
    file.write("{\n")
    file.write(f"    mesh {name};\n\n")
    file.write(f"    {name}.verts_size = {malloc_verts};\n")
    file.write(f"    {name}.tri_size = {malloc_tris};\n\n")

    file.write(f"    {name}.verts = malloc(sizeof(TRI_VERT3) * {malloc_verts});\n")
    file.write(f"    {name}.tris = malloc(sizeof(tri) * {malloc_tris});\n\n")

    for i in range(0, len(verts)):
        file.write(f"    {name}.verts[{i}] = (TRI_VERT3)" + "{.position = (Vector3){")
        file.write(str(verts[i][0]) + ", " + str(verts[i][1]) + ", " + str(verts[i][2]) + "}, .color = (color_rgb){255, 255, 255}};\n")

    file.write('\n')
    for i in range(0, len(tris)):
        file.write(f"    {name}.tris[{i}] = (tri)" + "{")
        file.write(str(tris[i][0]) + ", " + str(tris[i][1]) + ", " + str(tris[i][2]) + "};\n")

    file.write(f"\n    return {name};\n")

    file.write("}\n")
