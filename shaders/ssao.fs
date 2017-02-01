#version 330

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D gPositionMap;
uniform float gSampleRad;
uniform mat4 gProj;

const int MAX_KERNEL_SIZE = 128;
uniform vec3 gKernel[MAX_KERNEL_SIZE];

void main()
{
    vec3 Pos = texture(gPositionMap, TexCoord).xyz;

    float AO = 0.0;

    for (int i = 0 ; i < MAX_KERNEL_SIZE ; i++) {
        vec3 samplePos = Pos + gKernel[i]; // generate a random point
        vec4 offset = vec4(samplePos, 1.0); // make it a 4-vector
        offset = gProj * offset; // project on the near clipping plane
        offset.xy /= offset.w; // perform perspective divide
        offset.xy = offset.xy * 0.5 + vec2(0.5); // transform to (0,1) range

        float sampleDepth = texture(gPositionMap, offset.xy).b;

        if (abs(Pos.z - sampleDepth) < gSampleRad) {
            AO += step(sampleDepth,samplePos.z);
        }
    }

    AO = 1.0 - AO/128.0;

    FragColor = vec4(pow(AO, 2.0));
}