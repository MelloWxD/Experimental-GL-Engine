#include "..\Header\Model.h"
#include "..\Header\Vertex.h"
#include "..\Header\Mesh.h"
#include "..\Header\Texture.h"
#include "..\Header\ShaderModule.h"

void Model::Draw(ShaderModule* shader)
{
    for (unsigned int i = 0; i < _meshes.size(); i++)
        _meshes[i].Draw(shader);
}

void Model::loadFromFile(std::string file_path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("ASSIMP ERROR :: %s \n", importer.GetErrorString());
        return;
    }
    directory = file_path.substr(0, file_path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}


std::vector<Texture*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene)
{
    std::vector<Texture*> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        std::string abspath = directory + "/" + str.C_Str();
    
        
        for (auto x = 0; x < _loaded_Textures.size(); ++x)
        {
            if (std::strcmp(_loaded_Textures[x]->_path.data(), abspath.c_str()) == 0)
            {
                skip = true;
                textures.push_back(_loaded_Textures[x]);
                break;
            }
        }
        if (!skip)
        {
            if (auto tex = scene->GetEmbeddedTexture(str.C_Str()))// check if embedded
            {
              
            
                Texture* texture = new Texture(tex, i, GL_UNSIGNED_BYTE, abspath.c_str());    
                texture->_type = type;
                textures.push_back(texture);
                _loaded_Textures.push_back(texture);
                printf("Embedded texture found");// embedded texture
            }
            else
            {
                Texture* texture = new Texture(abspath.c_str(), i, GL_UNSIGNED_BYTE);
                texture->_type = type;
                /* texture.id = TextureFromFile(str.C_Str(), directory);

                 texture.path = str;*/
                textures.push_back(texture);
                _loaded_Textures.push_back(texture);
            }
        }
    }
    return textures;
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {        
        // process vertex positions, normals and texture coordinates
        Vertex v;
        v3 pos;
        pos.x = mesh->mVertices[i].x;
        pos.y = mesh->mVertices[i].y;
        pos.z = mesh->mVertices[i].z;
        v.position = pos;

        v3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;

        v.normal = normal;

        if (mesh->mTextureCoords[0]) // Check the mesh has textures otherwise set them to zero to prevent artefacting
        {
            v2 Uvs;
            Uvs.x = mesh->mTextureCoords[0][i].x;
            Uvs.y = mesh->mTextureCoords[0][i].y;
            v.uv = Uvs;
        }
        else
        {
            v.uv = v2(0.f, 0.f);
        }
        vertices.push_back(v);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }  




    // process material & textures
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture*> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse", scene);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture*> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular", scene);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end()); 
        
       /* std::vector<Texture*> emissionMaps = loadMaterialTextures(material,
            aiTextureType_EMISSIVE, "texture_emission");
        textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());*/
    }


    return Mesh(vertices, indices, textures);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        system("cls");
        printf("Model Nodes Remaining - %i", node->mNumChildren - i -1);
        processNode(node->mChildren[i], scene);
    }
}