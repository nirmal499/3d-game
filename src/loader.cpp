#include <component/loader.hpp>
#include <component/raw_model.hpp>
#include <util/common.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::unique_ptr<RawModel> Loader::LoadToVao(const std::vector<float>& positions, const std::vector<float>& texCoords, const std::vector<float>& normals, const std::vector<unsigned int>& indices)
{
    GLuint VaoID;
    GLuint VboID[3];
    GLuint EboID;

    glGenVertexArrays(1, &VaoID);
    _vaos.push_back(VaoID);

    glGenBuffers(3, VboID);
    _vbos.push_back(VboID[0]);
    _vbos.push_back(VboID[1]);
    _vbos.push_back(VboID[2]);

    glGenBuffers(1, &EboID);
    _vbos.push_back(EboID);

    glBindVertexArray(VaoID);

        glBindBuffer(GL_ARRAY_BUFFER, VboID[0]);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VboID[1]);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, VboID[2]);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(2);
        
        /*  It is generally unnecessary to unbind the element array buffer (EBO) when unbinding the vertex array 
            object (VAO) because the binding of the EBO is stored within the VAO. The state of the 
            EBO is automatically tracked by the VAO, so it is sufficient to unbind only the VAO.
        */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), indices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->UnBindVao(); /* glBindVertexArray(0) */

    return std::make_unique<RawModel>(VaoID, indices.size());

    /* Logger::log(1, "%s: VAO and VBO initialized\n", __FUNCTION__); */
}

GLuint Loader::LoadTexture(const char* fileName)
{
    int width, height, noofchannels;
    /* stbi_set_flip_vertically_on_load(true); */
    unsigned char *textureData = stbi_load(fileName, &width, &height, &noofchannels, 0);
	if (!textureData)
	{
		std::cout << "Can't load the texture file " << std::endl;
    	stbi_image_free(textureData);
	}

    GLuint textureID;
    
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		/* It defines the format and content of a texture that can be applied to 3D objects for rendering */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, noofchannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

  	glBindTexture(GL_TEXTURE_2D, 0);

  	stbi_image_free(textureData);

    _textures.push_back(textureID);
    return textureID;
}

void Loader::UnBindVao()
{
    glBindVertexArray(0);
}

void Loader::CleanUp()
{
    for(GLuint id: _vaos)
    {
        glDeleteVertexArrays(1, &id);
    }

    for(GLuint id: _vbos)
    {
        glDeleteBuffers(1, &id);
    }

    for(GLuint id: _textures)
    {
        glDeleteTextures(1, &id);
    }
}

