#pragma once
#include "SimpleShader.h"
class Material
{
public:
	Material(SimpleVertexShader*		VS, 
			 SimplePixelShader*			PS,
			 ID3D11ShaderResourceView*	SRV,
			 ID3D11SamplerState*		SS
	);
	Material();
	~Material();
	void SetVertexShader(SimpleVertexShader* VS);
	void SetPixelShader(SimplePixelShader* PS);
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();

	void SetSRV(ID3D11ShaderResourceView* SRV);
	void SetSamplerState(ID3D11SamplerState* SS);
	ID3D11ShaderResourceView* GetSRV();
	ID3D11SamplerState*		  GetSamplerState();

	ID3D11ShaderResourceView*	shaderResourceView;
	ID3D11SamplerState*			samplerState;

private:
	SimpleVertexShader*			vertexShader;
	SimplePixelShader*			pixelShader;
};

