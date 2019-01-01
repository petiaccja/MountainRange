#include "Renderer.hpp"
#include "Scene.hpp"
#include "TileEntity.hpp"

#include <GLES3/gl31.h>

#include <iostream>


using Mat44_PackedGL = mathter::Matrix<float, 4, 4,mathter::eMatrixOrder::FOLLOW_VECTOR, mathter::eMatrixLayout::COLUMN_MAJOR, true>;


namespace mrange {


struct CbTransform {
	Mat44_PackedGL worldViewProj;
	Mat44_PackedGL world;
};


Renderer::Renderer(const PlatformHelper* platformHelper)
	: m_platformHelper(platformHelper)
{
	assert(platformHelper != nullptr);
}


Renderer::~Renderer() {

}


void Renderer::DrawScene(const Scene& scene, const BasicCamera& camera) {
	if (!m_isGlBullshitInit) {
		LoadShaders();
		LoadVao();
		m_isGlBullshitInit = true;
	}

	SetDrawingState();

	// Get camera matrices.
	Mat44 view = camera.GetViewMatrix();
	Mat44 proj = camera.GetProjectionMatrix();


	CbTransform cbTransform;

	// Render all entities.
	for (auto& tileEntity : scene) {
		Mat44 world = Mat44::Identity();

		// Set cbuffer.
		cbTransform.worldViewProj = world*view*proj;
		cbTransform.world = world;

		assert(glGetError() == GL_NO_ERROR);
		auto worldViewProjLoc = glGetUniformLocation(m_shaderProgram, "g_worldViewProj");
		auto worldLoc = glGetUniformLocation(m_shaderProgram, "g_world");
		assert(glGetError() == GL_NO_ERROR);
		glUniformMatrix4fv(worldViewProjLoc, 1, false, (float*)&cbTransform.worldViewProj);
		glUniformMatrix4fv(worldLoc, 1, false, (float*)&cbTransform.world);
		assert(glGetError() == GL_NO_ERROR);

		// Bind vertex buffer.
		glBindVertexBuffer(0, tileEntity->GetVertexBuffer(), 0, sizeof(VertexPNT_Packed));
		glVertexBindingDivisor(0, 0);
		assert(glGetError() == GL_NO_ERROR);

		// Bind index buffer. Completely clear from these retarded arbitrary OpenGL names, right?
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tileEntity->GetIndexBuffer());
		assert(glGetError() == GL_NO_ERROR);

		// Of course specifying offset into the index buffer is the most fucking retarded bullshit ever
		// because fuck this goddamn useless bullshit crap "array" of retards writing this fucking mess
		// they call the bullshitGL specification. Fuck them all.
		glDrawElements(GL_TRIANGLES, tileEntity->GetIndexCount(), GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));
		assert(glGetError() == GL_NO_ERROR);
	}
}


void Renderer::LoadShaders() {
	std::string vertexShaderSource = m_platformHelper->LoadShaderFile("VertexShader.glsl");
	std::string pixelShaderSource = m_platformHelper->LoadShaderFile("PixelShader.glsl");

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	assert(glGetError() == GL_NO_ERROR);
	GLuint pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	assert(glGetError() == GL_NO_ERROR);

	const char* shaderSource = nullptr;
	GLint shaderSourceSize = 0;

	shaderSource = vertexShaderSource.data();
	shaderSourceSize = GLint(vertexShaderSource.size());
	glShaderSource(vertexShader, 1, &shaderSource, &shaderSourceSize);
	assert(glGetError() == GL_NO_ERROR);

	shaderSource = pixelShaderSource.data();
	shaderSourceSize = GLint(pixelShaderSource.size());
	glShaderSource(pixelShader, 1, &shaderSource, &shaderSourceSize);
	assert(glGetError() == GL_NO_ERROR);


	glCompileShader(vertexShader);
	glCompileShader(pixelShader);
	GLint compileStatus = GL_FALSE;
	glGetShaderiv(vertexShader,  GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		GLint logSize = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
		std::string log;
		log.resize(logSize);
		glGetShaderInfoLog(pixelShader, logSize, nullptr, log.data());
		assert(false);
	}
	glGetShaderiv(pixelShader,  GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		GLint logSize = 0;
		glGetShaderiv(pixelShader, GL_INFO_LOG_LENGTH, &logSize);
		std::string log;
		log.resize(logSize);
		glGetShaderInfoLog(pixelShader, logSize, nullptr, log.data());
		assert(false);
	}


	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, pixelShader);
	glLinkProgram(shaderProgram);
	assert(glGetError() == GL_NO_ERROR);

	m_shaderProgram = shaderProgram;
}

void Renderer::LoadVao() {
	glGenVertexArrays(1, &m_vao);
	assert(glGetError() == GL_NO_ERROR);
}

void Renderer::SetDrawingState() {
	// Set state
	glClearDepthf(1.0f);
	glDisable(GL_DEPTH_TEST);
	glDepthRangef(-1.0f, 1.0f);
	glUseProgram(m_shaderProgram);
	assert(glGetError() == GL_NO_ERROR);

	// Set vertex format.
	glBindVertexArray(m_vao);

	glEnableVertexAttribArray(0);
	assert(glGetError() == GL_NO_ERROR);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	assert(glGetError() == GL_NO_ERROR);
	glVertexAttribBinding(0, 0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NO_ERROR);
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPNT_Packed, normal));
	assert(glGetError() == GL_NO_ERROR);
	glVertexAttribBinding(1, 0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(2);
	assert(glGetError() == GL_NO_ERROR);
	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(VertexPNT_Packed, texcoord));
	assert(glGetError() == GL_NO_ERROR);
	glVertexAttribBinding(2, 0);
	assert(glGetError() == GL_NO_ERROR);
}


} // namespace mrange