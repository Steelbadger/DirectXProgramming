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
	matrix invProj;
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
	float4 pixelPosition;
	float4 color;
	float fac = 0.3;

	float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
	float4 ambient = float4(0.0, 0.0, 0.0, 0.0);
	float4 specular = float4(0.0, 0.0, 0.0, 0.0);

    textureColor = shaderTextures[0].Sample(SampleType, input.tex);
	normal = shaderTextures[1].Sample(SampleType, input.tex);
	float specularity = normal.w;
	normal.w = 0.0f;
	normal = normalize((normal*2)-1);
//	normal = max(-normal, normal);
	float cdepth = shaderTextures[2].Sample(SampleType, input.tex).x;
	float depth = (cdepth);
	float4 projectionPosition = float4(input.tex.x*2-1, (1-input.tex.y)*2-1, depth, 1.0f);
	pixelPosition = mul(invProj, projectionPosition);
	pixelPosition = pixelPosition/pixelPosition.w;

	float4 col = textureColor * diffuseColor;


	// Invert the light direction for calculations.
	float3 fragToLight = normalize(lightDirection - (pixelPosition * lightDirection.w));
	fragToLight = ((lightDirection.w*2)-1)*fragToLight;

	float3 fragToView = normalize(-pixelPosition);

	float dist = distance(lightDirection.xyz, pixelPosition) * lightDirection.w;

	//ambient = textureColor * diffuseColor * fac * (1-lightDirection.w);	
	ambient = fac * (1-lightDirection.w);

	float diffuseContribution = max(0.0, dot(normal.xyz, fragToLight));

	float attenuation = (1/(1 + 0.1*dist + 0.01 * dist * dist));

	//diffuse = textureColor * diffuseColor * diffuseContribution * attenuation;
	diffuse = diffuseContribution * attenuation;

	float3 lightReflection = reflect(fragToLight, normal.xyz);

	float specularContribution = max(0.0, dot(-lightReflection, fragToView));

	specularContribution = pow(specularContribution, specularPower) * specularity;

	//specular = textureColor * diffuseColor * specularContribution * attenuation;
	specular = specularContribution * attenuation;

	//  Don't apply lighting to areas with no geometry (normal.w will be set to 1.0 in those areas)
	color = col * (ambient + diffuse + specular);
//	color = (projectionPosition+1)/2;
	color = pixelPosition/100.0f;
//	color = float4(dist, dist, dist, 1.0f)/100;
//	color = float4(diffuseContribution, diffuseContribution, diffuseContribution, 1.0f);
//	color = float4(fragToLight, 1.0f);
//	color = float4(fragToView, 1.0f);
//	color = textureColor;
//	color = float4(normal.xyz, 1.0f);
//	color = float4(normal.y, normal.y, normal.y, 1.0f);
//	color = float4(cdepth, cdepth, cdepth, 1.0f);

    return color;
}
