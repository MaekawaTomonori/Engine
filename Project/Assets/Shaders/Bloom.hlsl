#include "Screen.hlsli"

float4 main(VertexShaderOutput input) : SV_TARGET3 {
    float w, h, levels;
    gRootColor.GetDimensions(0, w, h, levels);

    float2 d = float2(1.f / w, 1.f / h);

    float4 bloomAcc = float4(0, 0, 0, 0);
    float2 uvSize = float2(1.f, 0.5f);
    float2 uvOffset = float2(0, 0);

    for (int i = 0; i < 16; ++i) {
        bloomAcc += Get5x5GaussianBlur(gBlur, gSampler, input.texcoord * uvSize + uvOffset, d, float4(uvOffset, uvOffset + uvSize));
        uvOffset.y += uvSize.y;
        uvSize *= 0.5f;
    }

	float4 color = Get5x5GaussianBlur(gRootColor, gSampler, input.texcoord, d, float4(0,0,1,1) + saturate(bloomAcc));
    return color;
}
