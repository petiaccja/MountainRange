#version 300 es

uniform mat4 g_worldViewProj;
uniform mat4 g_world;

layout(location = 0) in vec3 lPosition;
layout(location = 1) in vec3 lNormal;
layout(location = 2) in vec2 lTexcoord;

out vec4 hPosition;
out vec3 wNormal;
out vec2 hTexcoord;


void main() {
	hPosition = vec4(lPosition, 1)*g_worldViewProj;
	wNormal = normalize(lNormal*mat3(g_world));
	hTexcoord = lTexcoord;

	gl_Position = hPosition;
}