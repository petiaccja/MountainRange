#include <jni.h>
#include <string>

#include <chrono>
#include <cmath>
#include <random>

#include "Rendering/Renderer.hpp"
#include "Rendering/Scene.hpp"
#include "Rendering/TileEntity.hpp"
#include "Rendering/PerspectiveCamera.hpp"

#include <android/asset_manager_jni.h>


using namespace mrange;


static std::unique_ptr<PlatformHelper> g_platformHelper;
static std::unique_ptr<Renderer> g_renderer;
static std::unique_ptr<Scene> g_scene;
static std::unique_ptr<TileEntity> g_testTile;
static PerspectiveCamera g_camera;


extern "C"
JNIEXPORT jstring JNICALL Java_petiaccja_mountainrange_MainActivity_getCompilerVersion(JNIEnv* env, jobject) {
    std::string compilerVersion = std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__);
    std::string hello = "Clang " + compilerVersion + ".";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL Java_petiaccja_mountainrange_JniBridge_Init(JNIEnv* env, jobject object, jobject activity, jobject assetManager) {
    // Create objects.
    AAssetManager* nativeAssetManager = AAssetManager_fromJava(env, assetManager);
    g_platformHelper = std::make_unique<PlatformHelper>(nativeAssetManager);
    g_renderer = std::make_unique<Renderer>(g_platformHelper.get());
    g_scene = std::make_unique<Scene>();
    g_testTile = std::make_unique<TileEntity>();


    g_camera.SetPosition({1.4f, 0.6f, 0.7f});
    g_camera.SetFOVAspect(Rad2Deg(60.f), 1.0f/1.8f);
    g_camera.SetTarget({0.0f, 0.0f, 0.0f});
    g_camera.SetFarPlane(5.0f);
    g_camera.SetNearPlane(0.05f);

    // Set up test scene.
    g_scene->AddEntity(g_testTile.get());

    GeometryTile tile(64);
    Image<float> image(8, 8);
    std::mt19937 rne;
    std::uniform_real_distribution<float> rng(0.0f, 0.1f);
    for (int i=0; i<8; ++i) {
        for (int j=0; j<8; ++j) {
            image(i,j) = rng(rne);
        }
    }
    tile.ApplyHeightmap(image);
    tile.RecomputeNormals();
    g_testTile->SetTile(tile);
}



extern "C"
JNIEXPORT void JNICALL Java_petiaccja_mountainrange_JniBridge_OnDrawFrame() {
    std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());

    float blue = std::sin(time.count()*0.005f)/2.f + 0.5f;
    float red = std::cos(time.count()*0.005f)/2.f + 0.5f;

    glClearColor(red, 0.5f, blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    g_renderer->DrawScene(*g_scene, g_camera);
}
