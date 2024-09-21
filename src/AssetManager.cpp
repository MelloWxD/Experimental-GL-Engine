#include "..\Header\AssetManager.h"
#include "..\Header\Texture.h"

AssetManager::AssetManager()
{
	diffuseMap = new Texture("Assets/container2.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	specularMap = new Texture("Assets/container2_specular.png", GL_TEXTURE_2D, 1, GL_RGBA, GL_UNSIGNED_BYTE);
	emissionMap = new Texture("Assets/matrix.jpg", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE);
	/*LoadTexture("Assets/container.jpg", texture1, GL_RGB, false);
	LoadTexture("Assets/awesomeface.png", texture2, GL_RGBA, true);
	LoadTexture("Assets/container2.png", diffuseMap, GL_RGBA, true);
	LoadTexture("Assets/container2_specular.png", specularMap, GL_RGBA, true);*/
}

