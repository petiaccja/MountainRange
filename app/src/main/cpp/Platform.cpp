#include "Platform.hpp"

#include <jni.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <chrono>
#include <cmath>


#include "Renderer.hpp"
#include "Scene.hpp"
#include "TileEntity.hpp"
#include <optional>

#include <android/asset_manager_jni.h>


using namespace mrange;



static std::unique_ptr<PlatformHelper> g_platformHelper;
static std::unique_ptr<Renderer> g_renderer;
static std::unique_ptr<Scene> g_scene;
static std::unique_ptr<TileEntity> g_testTile;



extern "C"
JNIEXPORT void JNICALL Java_petiaccja_mountainrange_JniBridge_Init(JNIEnv* env, jobject object, jobject activity, jobject assetManager) {
	// Create objects.
	AAssetManager* nativeAssetManager = AAssetManager_fromJava(env, assetManager);
	g_platformHelper = std::make_unique<PlatformHelper>(nativeAssetManager);
	g_renderer = std::make_unique<Renderer>(g_platformHelper.get());
	g_scene = std::make_unique<Scene>();
	g_testTile = std::make_unique<TileEntity>();

	// Set up test scene.
	g_scene->AddEntity(g_testTile.get());

	GeometryTile tile(16);
	g_testTile->SetTile(tile);
}



extern "C"
JNIEXPORT void JNICALL Java_petiaccja_mountainrange_JniBridge_OnDrawFrame() {
	std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());

	float blue = std::sin(time.count()*0.005f)/2.f + 0.5f;
	float red = std::cos(time.count()*0.005f)/2.f + 0.5f;

	glClearColor(red, 0.5f, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	g_renderer->DrawScene(*g_scene.get());
}


namespace mrange {


PlatformHelper::PlatformHelper(AAssetManager* assetManager) : m_assetManager(assetManager) {
}


std::string PlatformHelper::LoadShaderFile(std::string_view fileName) const {
	AAsset* asset = AAssetManager_open(m_assetManager, fileName.data(), AASSET_MODE_STREAMING);

	std::array<char, 64> buffer;
	std::string content;

	int bytesRead = 0;
	while ((bytesRead = AAsset_read(asset, buffer.data(), buffer.size())) > 0) {
		content.insert(content.end(), buffer.begin(), buffer.begin() + bytesRead);
	}

	AAsset_close(asset);

	return content;
}


}