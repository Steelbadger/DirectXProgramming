////////////////////////////////////////////////////////////////////////////////
// Filename: normalmappixelshader.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////

Texture2D shaderTextures[3];
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 diffuseColor;
    float4 lightDirection;
    float specularPower;
	float3 lightPadding;
};

cbuffer CameraBuffer
{
	float3 position;
	float padding;
}

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
	float4 normal;
	float3 pixelPosition;
	float4 color;
	float fac = 0.3;

	float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
	float4 ambient = float4(0.0, 0.0, 0.0, 0.0);
	float4 specular = float4(0.0, 0.0, 0.0, 0.0);

    textureColor = shaderTextures[0].Sample(SampleType, input.tex);
	normal = shaderTextures[1].Sample(SampleType, input.tex);

	pixelPosition = shaderTextures[2].Sample(SampleType, input.tex).xyz;
	float specularity = shaderTextures[2].Sample(SampleType, input.tex).w;


	// Invert the light direction for calculations.
	float3 fragToLight = normalize(lightDirection - (pixelPosition * lightDirection.w));
	fragToLight = ((lightDirection.w*2)-1)*fragToLight;

	float3 fragToView = normalize(position - pixelPosition);

	float dist = distance(lightDirection.xyz, position) * lightDirection.w;

	ambient = textureColor * diffuseColor * fac * (1-lightDirection.w);	

	float diffuseContribution = max(0.0, dot(normal.xyz, fragToLight));

	float attenuation = (1/(1 + 0.01*dist + 0.0001 * dist * dist));

	diffuse = textureColor * diffuseColor * diffuseContribution * attenuation;

	float3 lightReflection = reflect(fragToLight, normal.xyz);

	float specularContribution = max(0.0, dot(-lightReflection, fragToView));

	specularContribution = pow(specularContribution, specularPower) * specularity;

	specular = textureColor * diffuseColor * specularContribution * attenuation;

	//  Don't apply lighting to areas with no geometry (normal.w will be set to 1.0 in those areas)
	color = (ambient + diffuse + specular) * (1-normal.w) + textureColor * normal.w;

    return color;
}
