#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL Java_petiaccja_mountainrange_MainActivity_getCompilerVersion(JNIEnv* env, jobject) {
    std::string compilerVersion = std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__);
    std::string hello = "Clang " + compilerVersion + ".";
    return env->NewStringUTF(hello.c_str());
}


