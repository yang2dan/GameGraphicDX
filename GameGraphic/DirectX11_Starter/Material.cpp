#include "Material.h"



Material::Material(	SimpleVertexShader*			VS = NULL,
					SimplePixelShader*			PS = NULL,
					ID3D11ShaderResourceView*	TEXTURE = NULL,
					ID3D11ShaderResourceView*	NM = NULL,
					ID3D11SamplerState*			SS = NULL
			)
{
	vertexShader		= VS;
	pixelShader			= PS;
	texture				= TEXTURE;
	normalMap			= NM;
	samplerState		= SS;
}

Material::Material()
{
	vertexShader = NULL;
	pixelShader = NULL;
	texture = NULL;
	normalMap = NULL;
	samplerState = NULL;
}

Material::~Material()
{
	//assert(texture);
	//assert(normalMap);
	//assert(samplerState);
	if (texture)
	{
		texture->Release();
		texture = NULL;
	}
	
	if (normalMap)
	{
		normalMap->Release();
		normalMap = NULL;
	}

	
	if (samplerState)
	{
		samplerState->Release();
		samplerState = NULL;
	}
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

void Material::SetTexture(ID3D11ShaderResourceView* TEXTURE)
{
	texture = TEXTURE;
}

void Material::SetSamplerState(ID3D11SamplerState* SS)
{
	samplerState = SS;
}

ID3D11ShaderResourceView* Material::GetTexture()
{
	return texture;
}

ID3D11SamplerState* Material::GetSamplerState()
{
	return samplerState;
}