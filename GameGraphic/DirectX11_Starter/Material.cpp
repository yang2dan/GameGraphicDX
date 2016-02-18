#include "Material.h"



Material::Material(SimpleVertexShader* VS = NULL, SimplePixelShader* PS = NULL)
{
	vertexShader = VS;
	pixelShader  = PS;
}

Material::Material()
{
	vertexShader = NULL;
	pixelShader = NULL;
}

Material::~Material()
{
}

void Material::SetVertexShader(SimpleVertexShader* VS)
{
	vertexShader = VS;
}
void Material::SetPixelShader(SimplePixelShader* PS)
{
	pixelShader = PS;
}
SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}
SimplePixelShader* Material::GetPixelShader()
{
	return pixelShader;
}