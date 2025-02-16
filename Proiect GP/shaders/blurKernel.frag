#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D intermediateTexture;
uniform vec2 texelSize;

void main() {
    // Gaussian blur kernel (5x5)
    float kernel[25] = float[](
        1.0 / 256,  4.0 / 256,  6.0 / 256,  4.0 / 256,  1.0 / 256,
        4.0 / 256, 16.0 / 256, 24.0 / 256, 16.0 / 256,  4.0 / 256,
        6.0 / 256, 24.0 / 256, 36.0 / 256, 24.0 / 256,  6.0 / 256,
        4.0 / 256, 16.0 / 256, 24.0 / 256, 16.0 / 256,  4.0 / 256,
        1.0 / 256,  4.0 / 256,  6.0 / 256,  4.0 / 256,  1.0 / 256
    );

    // Offsets for neighboring texels (5x5)
    vec2 offsets[25] = vec2[](
        vec2(-2, -2), vec2(-1, -2), vec2( 0, -2), vec2( 1, -2), vec2( 2, -2),
        vec2(-2, -1), vec2(-1, -1), vec2( 0, -1), vec2( 1, -1), vec2( 2, -1),
        vec2(-2,  0), vec2(-1,  0), vec2( 0,  0), vec2( 1,  0), vec2( 2,  0),
        vec2(-2,  1), vec2(-1,  1), vec2( 0,  1), vec2( 1,  1), vec2( 2,  1),
        vec2(-2,  2), vec2(-1,  2), vec2( 0,  2), vec2( 1,  2), vec2( 2,  2)
    );

    vec3 result = vec3(0.0);

    for (int i = 0; i < 25; i++) {
        vec3 color = texture(intermediateTexture, TexCoords + offsets[i] * texelSize).rgb;
        result += color * kernel[i];
    }

    FragColor = vec4(result, 1.0);
}
