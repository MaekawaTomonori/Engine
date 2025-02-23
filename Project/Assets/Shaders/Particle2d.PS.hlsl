#include "Particle2d.hlsli"

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput{
    float4 color : SV_TARGET0;
    float4 rootColor : SV_TARGET1;
};

PixelShaderOutput main(VertexShaderOutput input) {
    PixelShaderOutput output;

    float4 color = gTexture.Sample(gSampler, input.texcoord);

    if (color.a < 0.1f) {discard;}

    output.color = color;
    output.rootColor = color;

	return output;
}
