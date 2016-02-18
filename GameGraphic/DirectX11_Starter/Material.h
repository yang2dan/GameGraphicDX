#pragma once
#include "SimpleShader.h"
class Material
{
public:
	Material(SimpleVertexShader* VS, SimplePixelShader* PS);
	Material();
	~Material();
	void SetVertexShader(SimpleVertexShader* VS);
	void SetPixelShader(SimplePixelShader* PS);
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader*	pixelShader;
};

