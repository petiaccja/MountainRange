#pragma once

#include "Platform.hpp"

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>


namespace mrange {

class Scene;


class Renderer {
public:
	Renderer(const PlatformHelper* platformHelper);
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	~Renderer();

	void DrawScene(const Scene& scene);

private:
	void LoadShaders();
	void LoadVao();

private:
	const PlatformHelper* m_platformHelper = nullptr;
	GLuint m_shaderProgram = 0;
	GLuint m_vao = 0;

	GLuint m_cbTransform;

	bool m_isGlBullshitInit = false;
};



} // namespace mrange