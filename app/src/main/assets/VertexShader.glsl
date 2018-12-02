#version 300 es

uniform mat4 g_worldViewProj;
uniform mat4 g_world;

in vec3 lPosition;
in vec3 lNormal;
in vec2 lTexcoord;

out vec4 hPosition;
out vec3 wNormal;
out vec2 hTexcoord;


void main() {
	hPosition = vec4(lPosition, 1)*g_worldViewProj;
	wNormal = normalize(lNormal*mat3(g_world));
	hTexcoord = lTexcoord;
}