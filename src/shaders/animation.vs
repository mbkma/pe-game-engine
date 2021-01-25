#version 420 core

layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 5) in ivec4 BoneIDs;
layout (location = 6) in vec4 Weights;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

const int MAX_BONES = 100;

uniform mat4 gBones[MAX_BONES];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    Normal = mat3(transpose(inverse(model))) * aNormal;  // do this on the cpu
    TexCoords = aTexCoords;

    mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
    BoneTransform += gBones[BoneIDs[1]] * Weights[1];
    BoneTransform += gBones[BoneIDs[2]] * Weights[2];
    BoneTransform += gBones[BoneIDs[3]] * Weights[3];

    vec4 PosL = BoneTransform * vec4(aPos, 1.0);

    FragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * PosL;


}

