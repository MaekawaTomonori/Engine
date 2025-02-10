#include "Screen.hlsli"

struct PSOutput{
    float4 color : SV_TARGET0;
};

PSOutput main(VertexShaderOutput input) {
    PSOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);

    output.color += gBloom.Sample(gSampler, input.texcoord);
    output.color = pow(output.color, 2.2);

    return output;
}