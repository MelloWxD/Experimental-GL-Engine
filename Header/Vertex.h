#pragma once
#include"Constants.h"

struct Vertex
{
	Vertex()
	{
		position = v3(0);
		normal = v3(0);
		uv = v2(0);
	}
	v3 position;
	v3 normal;
	v2 uv;
};