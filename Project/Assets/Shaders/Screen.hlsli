Texture2D gTexture : register(t0);
Texture2D gRootColor : register(t1);
Texture2D gBlur : register(t2);
Texture2D gBloom : register(t3);
SamplerState gSampler : register(s0);

float4 Get5x5GaussianBlur(Texture2D texture, SamplerState smp, float2 uv, float2 d, float4 rect);

struct VertexShaderOutput{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};
float4 Get5x5GaussianBlur(Texture2D texture, SamplerState smp,float2 uv, float2 d, float4 rect) {
    float4 r = texture.Sample(smp, uv);
    float4 blur = float4(0.f, 0.f, 0.f, 0.f);

    float weights[5][5] = {
        { 1 / 273.0, 4 / 273.0, 7 / 273.0, 4 / 273.0, 1 / 273.0 },
        { 4 / 273.0, 16 / 273.0, 26 / 273.0, 16 / 273.0, 4 / 273.0 },
        { 7 / 273.0, 26 / 273.0, 41 / 273.0, 26 / 273.0, 7 / 273.0 },
        { 4 / 273.0, 16 / 273.0, 26 / 273.0, 16 / 273.0, 4 / 273.0 },
        { 1 / 273.0, 4 / 273.0, 7 / 273.0, 4 / 273.0, 1 / 273.0 }
    };

    float offsets[5] = { -2.0f, -1.0f, 0.0f, 1.0f, 2.0f };

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            float2 offset = float2(offsets[i] * d.x, offsets[j] * d.y);
            float2 sampleUV = uv + offset;

            sampleUV.x = clamp(sampleUV.x, rect.x + d.x * 0.5, rect.z - d.x * 0.5);
            sampleUV.y = clamp(sampleUV.y, rect.y + d.y * 0.5, rect.w - d.y * 0.5);

            blur += texture.Sample(smp, sampleUV) * weights[i][j];
        }
    }

    return float4(blur.rgb, r.a);
}