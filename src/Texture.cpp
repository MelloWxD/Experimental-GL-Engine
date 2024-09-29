#include "..\Header\Texture.h"
#include "..\Header\ShaderModule.h"
#define STB_IMAGE_IMPLEMENTATION

#include"..\External\include/stb_image.h"
#include<filesystem>

Texture::Texture(const char* img_path, GLuint slot, GLenum pixelType) // TYPE IS ASSUMED TO BE GL_TEXTURE_2D
{
	// Assigns the type of the texture ot the texture object

	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(img_path, &widthImg, &heightImg, &numColCh, 0);
	if (bytes)
	{
		GLenum format;
		if (numColCh == 1)
			format = GL_RED;
		else if (numColCh == 3)
			format = GL_RGB;
		else if (numColCh == 4)
			format = GL_RGBA;

		// Generates an OpenGL texture object
		glGenTextures(1, &_id);
		// Assigns the texture to a Texture Unit
		glActiveTexture(GL_TEXTURE0 + slot);
		_unit = slot;
		glBindTexture(GL_TEXTURE_2D, _id);

		// Configures the type of algorithm that is used to make the image smaller or bigger
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Configures the way the texture repeats (if it does at all)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
		// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
		// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

		// Assigns the image to the OpenGL Texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
		// Generates MipMaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// Deletes the image data as it is already in the OpenGL Texture object
		stbi_image_free(bytes);

		// Unbinds the OpenGL Texture object so that it can't accidentally be modified
		glBindTexture(GL_TEXTURE_2D, 0);
		_path = std::string(img_path);

	}
	else
	{
		printf("Failed to Load texture %s\n", img_path);
		stbi_image_free(bytes);
	}
}

Texture::Texture(std::string img_path, GLuint slot, GLenum pixelType)
{// Assigns the type of the texture ot the texture object

	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(img_path.c_str(), &widthImg, &heightImg, &numColCh, 0);
	if (bytes)
	{
		GLenum format;
		if (numColCh == 1)
			format = GL_RED;
		else if (numColCh == 3)
			format = GL_RGB;
		else if (numColCh == 4)
			format = GL_RGBA;

		// Generates an OpenGL texture object
		glGenTextures(1, &_id);
		// Assigns the texture to a Texture Unit
		glActiveTexture(GL_TEXTURE0 + slot);
		_unit = slot;
		glBindTexture(GL_TEXTURE_2D, _id);

		// Configures the type of algorithm that is used to make the image smaller or bigger
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Configures the way the texture repeats (if it does at all)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
		// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
		// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

		// Assigns the image to the OpenGL Texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
		// Generates MipMaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// Deletes the image data as it is already in the OpenGL Texture object
		stbi_image_free(bytes);

		// Unbinds the OpenGL Texture object so that it can't accidentally be modified
		glBindTexture(GL_TEXTURE_2D, 0);
		_path = std::string(img_path);

	}
	else
	{
		printf("Failed to Load texture %s\n", img_path);
		stbi_image_free(bytes);
	}
}

Texture::Texture(const aiTexture* tex_ptr, GLuint slot, GLenum pixelType, std::string path)
{
	// Assigns the type of the texture ot the texture object
	if (tex_ptr->mHeight == 0)
	{
		// Compressed texture data
		const unsigned char* textureData = reinterpret_cast<const unsigned char*>(tex_ptr->pcData);


		// Stores the width, height, and the number of color channels of the image
		int widthImg, heightImg, numColCh;
		// Flips the image so it appears right side up
		stbi_set_flip_vertically_on_load(true);
		// Load texture using stb_image from memory
		unsigned char* image = stbi_load_from_memory(textureData, tex_ptr->mWidth, &widthImg, &heightImg, &numColCh, 0);

		if (image)
		{
			GLenum format;
			if (numColCh == 1)
				format = GL_RED;
			else if (numColCh == 3)
				format = GL_RGB;
			else if (numColCh == 4)
				format = GL_RGBA;

			// Generates an OpenGL texture object
			glGenTextures(1, &_id);
			// Assigns the texture to a Texture Unit
			glActiveTexture(GL_TEXTURE0 + slot);
			_unit = slot;
			glBindTexture(GL_TEXTURE_2D, _id);

			// Configures the type of algorithm that is used to make the image smaller or bigger
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			// Configures the way the texture repeats (if it does at all)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
			// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

			// Assigns the image to the OpenGL Texture object
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, image);
			// Generates MipMaps
			glGenerateMipmap(GL_TEXTURE_2D);

			// Deletes the image data as it is already in the OpenGL Texture object
			stbi_image_free(image);

			// Unbinds the OpenGL Texture object so that it can't accidentally be modified
			glBindTexture(GL_TEXTURE_2D, 0);
			_path = std::string(path);

		}
		else
		{
			printf("Failed to Load Embedded texture %s\n", path);
			stbi_image_free(image);
		}
	}
}
Texture::Texture(int x, int y, GLuint slot = 0, GLenum texture_type = GL_TEXTURE_2D, GLenum filter_mode = GL_NEAREST, unsigned clampMode = GL_REPEAT)
{
	// SHADOW / depth mpa Tex generation
	_type = texture_type;
	glGenTextures(1, &_id);
	glActiveTexture(GL_TEXTURE0 + slot);
	_unit = slot;
	glBindTexture(texture_type, _id);
	glTexImage2D(texture_type, 0, GL_DEPTH_COMPONENT,
		x, y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, filter_mode);
	glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, filter_mode);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, clampMode);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, clampMode);
	_unit = slot;
}
Texture::~Texture()
{
	glDeleteTextures(1, &_id);

}

void Texture::texUnit(ShaderModule& shaderM, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform buffer
	GLuint texUni = glGetUniformLocation(shaderM.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shaderM.Use();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + _unit);
	glBindTexture(_type, _id);
}

void Texture::Unbind()
{
	glBindTexture(_type, 0);

}

CubeMap::CubeMap(std::string img_path)
{
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

	int width, height, nrChannels;	

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		auto fn = std::string(img_path + "/");
		(fn += faces[i]).c_str();
		stbi_set_flip_vertically_on_load(false);

		unsigned char* data = stbi_load(fn.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

CubeMap::~CubeMap()
{
}

void CubeMap::texUnit(ShaderModule& shaderM, const char* uniform, GLuint unit)
{
}

void CubeMap::Bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

}

void CubeMap::Unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}
