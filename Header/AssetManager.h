#pragma once
#include"Constants.h"


struct Texture;

class AssetManager
{
public:
	AssetManager();
	
	
	Texture* texture1;
	Texture* texture2;
	Texture* diffuseMap;
	Texture* specularMap;
	Texture* emissionMap;
};

