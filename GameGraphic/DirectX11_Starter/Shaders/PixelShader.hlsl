
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
	float3 worldPos		: POSITION;
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
	//Directional Light
	input.normal = normalize(input.normal);
	float3 dirToLight = normalize(-dirlight.Direction);
	float  NdotL = saturate(dot(input.normal, dirToLight));

	// POINT LIGHT -----------------------------
	// Diffuse light from point light
	float3 dirToPointLight = normalize(pointlight.Position - input.worldPos);
	float point_NdotL = saturate(dot(input.normal, dirToPointLight));

	// Specular light from point light
	float3 toCamera = normalize(cameraPosition - input.worldPos);
	float3 refl = reflect(-dirToPointLight, input.normal);
	float specularAmount = pow(max(dot(refl, toCamera), 0), 16);

	//return float4(input.normal, 1);
	
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	return dirlight.AmbientColor +
			(dirlight.DiffuseColor * NdotL)+
			(pointlight.Color * point_NdotL) + specularAmount;
}