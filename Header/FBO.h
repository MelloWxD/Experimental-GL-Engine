#pragma once
#include"Constants.h"
class Texture;
class ShaderModule;
class PointLight;
//Class Wrapper for Framebuffer Objects
class FBO
{
public:
	FBO(PointLight* pl, unsigned t = FBO_DEFAULT);
	FBO(unsigned t);
	~FBO();

	bool checkComplete();
	void Bind();
	void Unbind();

	unsigned _fbotype = FBO_DEFAULT;
	enum fboType : unsigned int
	{
		FBO_DEFAULT = 0,
		FBO_SHADOWPASS = 1,
		FBO_POINTLIGHT_SHADOWPASS = 2
	};

	unsigned int _framebuffer;
	unsigned int _rbo;
	unsigned int _tex;
	unsigned int _depthCubemap;
	//Texture* _tex;

};