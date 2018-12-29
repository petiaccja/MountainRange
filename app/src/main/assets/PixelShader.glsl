#version 300 es

in vec4 hPosition;
in vec3 wNormal;
in vec2 hTexcoord;

out vec4 outColor;

void main() {
	vec3 normal = normalize(wNormal);

	vec3 sunDir = normalize(vec3(-0.2, -0.3, -0.7));
	vec3 zenithDir = vec3(0,0,1);
	vec3 sunColor = vec3(0.9, 0.8, 0.7);
	vec3 skyColor = vec3(0.15, 0.23, 0.3);

	float sunCoeff = clamp(-dot(normal, sunDir), 0.0, 1.0);
	float skyCoeff = clamp(dot(normal, zenithDir), 0.0, 1.0);

    outColor = vec4(sunColor*sunCoeff + skyColor*skyCoeff, 1.0);
}