#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

out VS_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLight;
    mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

uniform vec3 lightPos;
uniform vec3 viewPos;
void main()
{    

    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.FragPosLight = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    vs_out.TBN = transpose(mat3(T, B, N));    
    vs_out.TangentLightPos = vs_out.TBN * lightPos;
    vs_out.TangentViewPos  = vs_out.TBN * viewPos;
    vs_out.TangentFragPos  = vs_out.TBN * vs_out.FragPos;
    vs_out.TexCoords = aTexCoords;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}