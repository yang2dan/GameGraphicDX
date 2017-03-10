//Pixel Shader Input
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uvw			: TEXCOORD0;
};

//Textures
TextureCube skyTexture	: register(t0);
SamplerState trilinear	: register(s0);

//entry point of pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	return skyTexture.Sample(trilinear, input.uvw);
}