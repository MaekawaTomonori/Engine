#include "Object3d.hlsli"

struct Material{
    float32_t4 color;
    uint32_t enableLighting;
    float32_t shininess;
};
ConstantBuffer<Material> gMaterial : register(b0);

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct DirectionalLight{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

struct Camera{
    float32_t3 worldPosition;
};
ConstantBuffer<Camera> gCamera : register(b2);

struct PointLight{
    float32_t4 color;
    float32_t3 position;
    float intensity;
    float radius;
    float decay;
};
ConstantBuffer<PointLight> gPointLight : register(b3);

struct SpotLight{
    float32_t4 color;
    float32_t3 position;
    float32_t intensity;
    float32_t3 direction;
    float32_t distance;
    float32_t decay;
    float32_t cosAngle;
};
ConstantBuffer<SpotLight> gSpotLight : register(b4);

struct PixelShaderOutput{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
    PixelShaderOutput output;
    float32_t4 texColor = gTexture.Sample(gSampler, input.texcoord);

    if (texColor.a == 0.f) {discard;}

	float32_t3 rgb = gMaterial.color.rgb * texColor.rgb;
    float32_t3 finalRGB = rgb;
    float32_t a = gMaterial.color.a * texColor.a;

    //Lambertian Reflectance
    if (gMaterial.enableLighting == 0){
        output.color = float32_t4(rgb, a);
        return output;
    }

	//directional
	float nDotL = dot(normalize(input.normal), -gDirectionalLight.direction);
	float cos = pow(nDotL * 0.5f + 0.5f, 2.0f);
    
    float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));

    float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
    float nDotH = saturate(dot(normalize(input.normal), halfVector));
    float specularPow = pow(saturate(nDotH), gMaterial.shininess);

    float32_t3 diffuse = rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
    float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.f, 1.f, 1.f);
    finalRGB = diffuse + specular;
    
    //point
    float32_t3 pointDirection = normalize(input.worldPosition - gPointLight.position);

    float32_t distance = length(gPointLight.position - input.worldPosition);
    float32_t factor = pow(saturate(-distance / gPointLight.radius + 1.0), gPointLight.decay);

    float cosP = pow(dot(normalize(input.normal), -pointDirection) * 0.5f + 0.5f, 2.0f);

    float32_t3 halfVectorP = normalize(-pointDirection + toEye);
    float specularPowP = pow(saturate(dot(normalize(input.normal), halfVector)), gMaterial.shininess);

    float32_t3 pointDiffuse = rgb * gPointLight.color.rgb * cosP * gPointLight.intensity * factor;
    float32_t3 pointSpecular = gPointLight.color.rgb * gPointLight.intensity * factor * specularPowP * float32_t3(1.f, 1.f, 1.f);

    finalRGB += pointDiffuse + pointSpecular;

    //spot
    float32_t3 spotDirection = normalize(input.worldPosition - gSpotLight.position);
    float32_t cosAngle = dot(spotDirection, gSpotLight.direction);
    float32_t falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (1.f - gSpotLight.cosAngle));

    float32_t attenuationFactor = 1.f / (1.f + gSpotLight.decay * pow(distance / gSpotLight.distance, 2.f));

    finalRGB += rgb * gSpotLight.color.rgb * gSpotLight.intensity * attenuationFactor * falloffFactor;


    //final set
    output.color = float32_t4(finalRGB, a);

    if (output.color.a == 0.f){discard;}

    return output;
}
