#include "Particle.hlsli"

struct Material{
    float32_t4 color;
    uint32_t enableLighting;
};
ConstantBuffer<Material> gMaterial : register(b0);

Texture2D gTexture : register(t0);
SamplerState gSampler : register(t0);

struct PixelShaderOutput{
    float32_t4 color : SV_TARGET0;
};

//PixelShaderOutput main(GeometryShaderOutput input) {
PixelShaderOutput main(VertexShaderOutput input) {
    PixelShaderOutput output;
    float32_t4 texColor = gTexture.Sample(gSampler, input.texcoord);

    if (texColor.a == 0.f) {discard;}

    output.color = gMaterial.color * texColor * input.color;

    if (output.color.a == 0.f){discard;}

    return output;
}
