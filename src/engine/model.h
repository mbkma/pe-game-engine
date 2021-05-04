#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
public:
    // model data
    vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(string const &path, bool gamma = false);

    // draws the model, and thus all its meshes
    void Draw(Shader &shader);

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path);

    Assimp::Importer* m_importer;
    const aiScene* m_pScene;

#define NUM_BONES_PER_VERTEX 4

    struct BoneInfo
    {
        glm::mat4 BoneOffset;
        glm::mat4 FinalTransformation;

        BoneInfo()
        {
            BoneOffset = glm::mat4(0.0f);
            FinalTransformation = glm::mat4(0.0f);
        }
    };

    std::map<std::string, unsigned int> m_BoneMapping; // maps a bone name to its index
    unsigned int m_NumBones;
    std::vector<BoneInfo> m_BoneInfo;

    /* duration of the animation, can be changed if frames are not present in all interval */
    double animDuration;

    unsigned int currentAnimation;

    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);

    unsigned int numBones() const
    {
        return m_NumBones;
    }

    void boneTransform(float timeInSeconds, std::vector<glm::mat4>& Transforms);
    void setBoneTransformations(GLuint shaderProgram, GLfloat currentTime);
    unsigned int getNumAnimations();
    void setAnimation(unsigned int a);

private:

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node);

    Mesh processMesh(aiMesh *mesh);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

    unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
};

#endif
