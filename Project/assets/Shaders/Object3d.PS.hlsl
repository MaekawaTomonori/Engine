#include "Object3d.hlsli"

struct Material{
    float32_t4 color;
    uint32_t enableLighting;
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

struct PixelShaderOutput{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
    PixelShaderOutput output;
    float32_t4 texColor = gTexture.Sample(gSampler, input.texcoord);

    //Lambertian Reflectance
    if (gMaterial.enableLighting == 1) {
        float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        output.color = gMaterial.color * texColor * gDirectionalLight.color * gDirectionalLight.intensity * cos;
    } else if (gMaterial.enableLighting == 2) {
        float nDotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(nDotL * 0.5f + 0.5f, 2.0f);
        output.color = gMaterial.color * texColor * gDirectionalLight.color * gDirectionalLight.intensity * cos;
    } else {
        output.color = gMaterial.color * texColor;
    }

    return output;
}
