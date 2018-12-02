#version 300 es

in vec4 hPosition;
in vec3 wNormal;
in vec2 hTexcoord;

out vec4 colorOut;

void main() {
    colorOut = vec4(1.0, 0.0, 0.0, 1.0)*clamp(dot(wNormal, vec3(0,0,-1)), 0.0, 1.0);
}