#include "Particle.hlsli"

[maxvertexcount(3)]
void main(triangle VertexShaderOutput input[3] : SV_POSITION, inout TriangleStream<GeometryShaderOutput> output) {
    for (uint i = 0; i++ < 3; ){
        GeometryShaderOutput element;
        element.position = input[i].position;
        element.texcoord = input[i].texcoord;
        element.normal = input[i].normal;

        output.Append(element);
    }

}