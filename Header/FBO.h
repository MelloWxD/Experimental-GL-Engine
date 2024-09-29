#pragma once
#include"Constants.h"
class Texture;
//Class Wrapper for Framebuffer Objects
class FBO
{
public:
	FBO(unsigned t);
	~FBO();

	bool checkComplete();
	void Bind();
	void Unbind();

	unsigned _fbotype = FBO_DEFAULT;
	enum fboType : unsigned int
	{
		FBO_DEFAULT = 0,
		FBO_SHADOWPASS = 1
	};

	unsigned int _framebuffer;
	unsigned int _rbo;
	unsigned int _tex;
	//Texture* _tex;

};