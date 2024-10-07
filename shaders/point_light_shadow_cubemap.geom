#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos;
in vec3 WorldPos[]; // Input world position

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        // Sets the face we are currently working on
        gl_Layer = face;
        for(int i = 0; i < 3; i++)
        {
            // Make transformed vertex
            FragPos = vec4(WorldPos[i], 1.0);  
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}