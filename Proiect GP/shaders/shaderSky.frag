#version 330 core
out vec4 FragColor;

void main() {
    float gradient = gl_FragCoord.y / 900.0; 
    vec3 topColor = vec3(0.05, 0.0, 0.1); // black purple
    vec3 bottomColor = vec3(0.0, 0.1, 1.0);  // bottom color (near-white)
    FragColor = vec4(mix(bottomColor, topColor, gradient), 1.0);
}
