#include "Screen.hlsli"

struct BlurOutput{
    float4 color : SV_TARGET2;
};

BlurOutput main(VertexShaderOutput input) {
    BlurOutput output;

    float w, h, levels;
    gTexture.GetDimensions(0, w, h, levels);
    float dx = 1.f / w;
    float dy = 1.f / h;

    output.color = Get5x5GaussianBlur(gRootColor, gSampler, input.texcoord, float2(dx, dy), float4(0, 0, 1, 1));
    return output;
}
