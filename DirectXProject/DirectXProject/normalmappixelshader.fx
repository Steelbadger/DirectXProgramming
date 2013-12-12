////////////////////////////////////////////////////////////////////////////////
// Filename: normalmappixelshader.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////

Texture2D shaderTextures[2];
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
	float specularPower;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
	float3 worldPosition : TEXCOORD1;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 NormalMapPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float3 fragToLight;
	float3 fragToView;
    float4 color;
	float fac = 0.6;

	float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
	float4 ambient = float4(0.0, 0.0, 0.0, 0.0);
	float4 specular = float4(0.0, 0.0, 0.0, 0.0);
	float4 lightcolour = float4(0.0, 0.0, 0.0, 0.0);

    // Sample the texture pixel at this location.
    textureColor = shaderTextures[0].Sample(SampleType, input.tex);
	//textureColor = shaderTextures[1].Sample(SampleType, input.tex);
	color = diffuseColor;

	ambient = ambient + (textureColor * diffuseColor * fac);
		
    // Sample the pixel in the bump map.
    bumpMap = shaderTextures[1].Sample(SampleType, input.tex);

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;

    // Calculate the normal from the data in the bump map.
    bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal;
	
    // Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);

    // Invert the light direction for calculations.
    fragToLight = -lightDirection;
	fragToView = normalize(-input.worldPosition);

	float diffuseContribution = max(0.0, dot(bumpNormal, fragToLight));
	diffuseContribution = max(0.0, dot(bumpNormal, fragToLight));

	float attenuation = 1;

	diffuse = diffuse + (textureColor * diffuseColor * diffuseContribution * attenuation);

	float3 lightReflection = reflect(fragToLight, bumpNormal);

	float specularContribution = max(0.0, dot(-lightReflection, fragToView));

	specularContribution = pow(specularContribution, specularPower);

	specular = specular + (textureColor * diffuseColor * specularContribution * attenuation);

	color = ambient + diffuse + specular;

 //   // Calculate the amount of light on this pixel based on the bump map normal value.
 //   lightIntensity = saturate(dot(bumpNormal, lightDir));

	//if(lightIntensity > 0.0f)
	//{
	//	// Add diffuse and light intensity to colour value (if greater than zero
	//	color += (diffuseColor * lightIntensity);

	//	// Saturate the ambient and diffuse color.
	//	color = saturate(color);

	//	// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
	//	reflection = normalize(2 * lightIntensity * input.normal - lightDir); 

	//	 // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
	//	specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
	//}

 //   // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
 //   color = saturate(color);

 //   // Combine the final bump light color with the texture color.
 //   color = color * textureColor;
	//// Add the specular component last to the output color.
	//color = saturate(color + specular);
	
    return color;
}