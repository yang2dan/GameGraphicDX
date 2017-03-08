#pragma once
#include "SimpleShader.h"
class Material
{
public:
	Material(	SimpleVertexShader*		VS,
				SimplePixelShader*			PS,
				ID3D11ShaderResourceView*	TEXTURE,
				ID3D11ShaderResourceView*	NM,
				ID3D11SamplerState*		SS
	);
	Material();
	~Material();
	void SetVertexShader(SimpleVertexShader* VS);
	void SetPixelShader(SimplePixelShader* PS);
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();

	void SetTexture(ID3D11ShaderResourceView* TEXTURE);
	void SetSamplerState(ID3D11SamplerState* SS);
	ID3D11ShaderResourceView* GetTexture();
	ID3D11SamplerState*		  GetSamplerState();

	ID3D11ShaderResourceView*	texture;
	ID3D11ShaderResourceView*	normalMap;
	ID3D11ShaderResourceView*	specTexture;
	ID3D11SamplerState*			samplerState;

private:
	SimpleVertexShader*			vertexShader;
	SimplePixelShader*			pixelShader;
};

