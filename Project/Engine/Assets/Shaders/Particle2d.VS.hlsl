#include "Particle2d.hlsli"

struct Particle{
    float32_t4x4 wvp;
    float32_t4 color;
};
StructuredBuffer<Particle> gParticles : register(t0);

struct VertexShaderInput{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID) {
    VertexShaderOutput output;
    output.position = mul(input.position, gParticles[instanceId].wvp);
    output.texcoord = input.texcoord;
    output.color = gParticles[instanceId].color;

    return output;
}
