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

    if (texColor.a == 0.f) {discard;}

    float32_t3 rgb = gMaterial.color.rgb * texColor.rgb;
    float32_t a = gMaterial.color.a * texColor.a;

	output.color.rgb = rgb;
    output.color.a = a;

    if (output.color.a == 0.f){discard;}

    return output;
}
