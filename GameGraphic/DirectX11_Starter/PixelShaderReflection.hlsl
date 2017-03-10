//input struct
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD0;
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

//Textures and so on
Texture2D		diffuseTexture	: register(t0);
Texture2D		normalMap		: register(t1);
Texture2D		specTexture		: register(t2);
TextureCube		skyTexture		: register(t3);
SamplerState	trilinear		: register(s0);

//Entry point
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	//Sample the normal map
	float3 normalFromMap = normalMap.Sample(trilinear, input.uv).rgb;

	//Unpack the normal
	normalFromMap = normalFromMap * 2 - 1;

	//Calculate the TBN matrix to go from tangent-space to world-space
	float3 N = input.normal;
	//Gram - Schmidt orthogonalize
	float3 T = normalize(input.tangent - N * dot(input.tangent, N));
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);

	//new normal from normal map
	input.normal = normalize(mul(normalFromMap, TBN));

	//Directional light-------------------------------------------------
	
	//Light direction
	float3 dirToLight = normalize(-dirlight.Direction);

	float NdotL = saturate(dot(input.normal, dirToLight));

	//point light-------------------------------------------------------
	//diffuse
	float3 dirToPointLight = normalize(pointlight.Position - input.worldPos);
	float point_NdotL = saturate(dot(input.normal, dirToPointLight));

	//specular light
	float3 toCamera = normalize(cameraPosition - input.worldPos);
	float3 refl = reflect(-dirToPointLight, input.normal);
	float specularAmount = pow(max(dot(refl, toCamera), 0), 64);

	float4 surfaceColor = diffuseTexture.Sample(trilinear, input.uv);
	float4 specularTexture = specTexture.Sample(trilinear, input.uv);
	float4 reflectionColor = skyTexture.Sample(trilinear, reflect(-toCamera, input.normal));

	float4 finalLight = dirlight.AmbientColor	//ambient
		+ (dirlight.DiffuseColor * NdotL)	//diffuse
		+ (pointlight.Color * point_NdotL);	//diffuse

	surfaceColor = surfaceColor * finalLight + specularAmount * specularTexture * pointlight.Color;

	return	//reflectionColor / 8 + surfaceColor;
			lerp(reflectionColor, surfaceColor, 0.0f);
}