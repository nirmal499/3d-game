#include <component/obj_loader.hpp>
#include <component/loader.hpp>
#include <component/raw_model.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::unique_ptr<RawModel> OBJLoader::LoadObjModel(const char* filePath, Loader* loader)
{
	std::vector<float> vertices;
    std::vector<float> textureCoords;
    std::vector<float> normals;
    std::vector<unsigned int> indices;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
        std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
        std::abort();
    }

    // Process each mesh in the scene
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
        aiMesh* mesh = scene->mMeshes[i];

        // Process vertices
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
		{
            aiVector3D vertex = mesh->mVertices[j];
            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);

            /*
                Normals are used for lighting calculations, and each vertex in a mesh can have only one normal vector.
                Therefore, Assimp does not need an index to specify which set of normals you are checking for.
            */
            if (mesh->HasNormals()) 
            {
                aiVector3D normal = mesh->mNormals[j];
                normals.push_back(normal.x);
                normals.push_back(normal.y);
                normals.push_back(normal.z);
            }

            /*
                Assimp supports multiple sets of texture coordinates (also known as UV channels) for a single mesh. This is useful for more
                complex materials that might need different textures for different purposes (e.g., diffuse map, normal map, light map, etc.).
                The function HasTextureCoords takes an index to specify which set of texture coordinates you are checking for.
            */
            if (mesh->HasTextureCoords(0))
			{
                /* Process the first set of texture coordinates */
                aiVector3D texCoord = mesh->mTextureCoords[0][j];
                textureCoords.push_back(texCoord.x);
                textureCoords.push_back(texCoord.y);
            } else
			{
                // If no texture coordinates, you may set default values
                textureCoords.push_back(0.0f);
                textureCoords.push_back(0.0f);
            }
        }

        // Process faces (indices)
        for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
		{
            aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k)
			{
                indices.push_back(face.mIndices[k]);
            }
        }
    }

    return loader->LoadToVao(vertices, textureCoords, normals, indices);
}

