//Constant buffer for C++ data being passed in
cbuffer externalData : register(b0)
{
	matrix view;
	matrix projection;
}

//Describes individual vertex data
struct VertexShaderInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
};

//Defines the output data of vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uvw			: TEXCOORD0;
};

//entry point
VertexToPixel main(VertexShaderInput input)
{
	//Output
	VertexToPixel output;

	//Adjust the view matrix (remove translation)
	matrix viewNoMovement = view;
	viewNoMovement._41 = 0;
	viewNoMovement._42 = 0;
	viewNoMovement._43 = 0;

	//Combine view anad projection
	matrix vp = mul(viewNoMovement, projection);
	output.position = mul(float4(input.position, 1), vp);

	//skybox vertex has a depth of 1.0
	output.position.z = output.position.w;

	//use input position as sampling direction in cubemap in Pixel Shader
	output.uvw = input.position;
	
	return output;
}