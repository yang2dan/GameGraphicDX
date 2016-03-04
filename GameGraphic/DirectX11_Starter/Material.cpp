#include "Material.h"



Material::Material(	SimpleVertexShader*			VS = NULL,
					SimplePixelShader*			PS = NULL,
					ID3D11ShaderResourceView*	SRV = NULL,
					ID3D11SamplerState*			SS = NULL
			)
{
	vertexShader		= VS;
	pixelShader			= PS;
	shaderResourceView	= SRV;
	samplerState		= SS;
}

Material::Material()
{
	vertexShader = NULL;
	pixelShader = NULL;
	shaderResourceView = NULL;
	samplerState = NULL;
}

Material::~Material()
{
	shaderResourceView->Release();
	samplerState->Release();
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

void Material::SetSRV(ID3D11ShaderResourceView* SRV)
{
	shaderResourceView = SRV;
}

void Material::SetSamplerState(ID3D11SamplerState* SS)
{
	samplerState = SS;
}

ID3D11ShaderResourceView* Material::GetSRV()
{
	return shaderResourceView;
}

ID3D11SamplerState* Material::GetSamplerState()
{
	return samplerState;
}