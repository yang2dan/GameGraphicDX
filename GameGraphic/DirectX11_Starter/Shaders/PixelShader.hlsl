
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD0;
	//float4 color		: COLOR;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

struct PointLight
{
	float4 Color;
	float3 Position;
};

cbuffer externalData : register(b0)
{
	DirectionalLight dirlight;
	PointLight		 pointlight;
	float3			 cameraPosition;
}

Texture2D		diffuseTexture	: register(t0);
Texture2D		normalMap		: register(t1);
Texture2D		specTexture		: register(t2);
SamplerState	trilinear		: register(s0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	//normal map normal calculate
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	float3 normalFromMap = normalMap.Sample(trilinear, input.uv).rgb;
	//Unpack the normal
	normalFromMap = normalFromMap * 2 - 1;
	//Calculate the TBN matrix to go from tangent-space to world-space
	float3 N = input.normal;	
	float3 T = input.tangent;//normalize(input.tangent - N * dot(input.tangent, N));
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);
	//change the existing normal
	input.normal = normalize(mul(normalFromMap, TBN));
	//Directional Light
	float3 dirToLight = normalize(-dirlight.Direction);
	float  NdotL = saturate(dot(input.normal, dirToLight));

	// POINT LIGHT -----------------------------
	// Diffuse light from point light
	float3 dirToPointLight = normalize(pointlight.Position - input.worldPos);
	float point_NdotL = saturate(dot(input.normal, dirToPointLight));

	// Specular light from point light
	float3 toCamera = normalize(cameraPosition - input.worldPos);
	float3 refl = reflect(-dirToPointLight, input.normal);
	float specularAmount = pow(max(dot(refl, toCamera), 0), 64);

	//return float4(input.normal, 1);
	float4  surfaceColor = diffuseTexture.Sample(trilinear, input.uv);

	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	float4 finalLight = dirlight.AmbientColor
		+ (dirlight.DiffuseColor * NdotL)
		+ (pointlight.Color * point_NdotL);
	
	float4 finalColor = surfaceColor * finalLight + specularAmount * pointlight.Color;
	return	float4(finalColor.rgb, 0.2f);
}