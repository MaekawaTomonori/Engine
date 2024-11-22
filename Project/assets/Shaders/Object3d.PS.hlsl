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

struct PixelShaderOutput{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
    PixelShaderOutput output;
    float32_t4 texColor = gTexture.Sample(gSampler, input.texcoord);

    if (texColor.a == 0.f) {discard;}

	float32_t3 rgb = gMaterial.color.rgb * texColor.rgb;
    float32_t a = gMaterial.color.a * texColor.a;

	float nDotL = dot(normalize(input.normal), -gDirectionalLight.direction);
    float cos = pow(nDotL * 0.5f + 0.5f, 2.0f);
    
    //Lambertian Reflectance
    if (gMaterial.enableLighting == 1) {
        cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        output.color.rgb = rgb * gDirectionalLight.color.rgb * gDirectionalLight.intensity * cos;
    }else if (gMaterial.enableLighting == 2) {
        
        output.color.rgb = rgb * gDirectionalLight.color.rgb * gDirectionalLight.intensity * cos;
    }else if (gMaterial.enableLighting == 3 || gMaterial.enableLighting == 4) {
        
        float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));

        float specularPow;

        if(gMaterial.enableLighting == 4){
            float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
            float nDotH = saturate(dot(normalize(input.normal), halfVector));
            specularPow = pow(saturate(nDotH), gMaterial.shininess);
        }else {
	        float rDotE = dot(reflectLight, toEye);
			specularPow = pow(saturate(rDotE), gMaterial.shininess);
        }

        float32_t3 diffuse = rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.f, 1.f, 1.f);
        output.color.rgb = diffuse + specular;
    }else{
        output.color.rgb = rgb;
    }

    output.color.a = a;

    if (output.color.a == 0.f){discard;}

    return output;
}
