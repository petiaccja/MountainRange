

cmake_minimum_required(VERSION 3.6.0)


include_directories(libs/include)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(sources
        src/main/cpp/native-lib.cpp
		)

set(resources
        src/main/cpp/Resources/GeometryTile.cpp
        src/main/cpp/Resources/GeometryTile.hpp
        src/main/cpp/Resources/Image.cpp
        src/main/cpp/Resources/Image.hpp
        src/main/cpp/Resources/Vertex.hpp
        )

set(platform
		src/main/cpp/Platform/Platform.cpp
		src/main/cpp/Platform/Platform.hpp
		)

set(rendering
		src/main/cpp/Rendering/BasicCamera.hpp
		src/main/cpp/Rendering/BasicCamera.cpp
		src/main/cpp/Rendering/PerspectiveCamera.hpp
		src/main/cpp/Rendering/PerspectiveCamera.cpp
        src/main/cpp/Rendering/TileEntity.cpp
        src/main/cpp/Rendering/TileEntity.hpp
        src/main/cpp/Rendering/Scene.cpp
        src/main/cpp/Rendering/Scene.hpp
        src/main/cpp/Rendering/Renderer.cpp
        src/main/cpp/Rendering/Renderer.hpp
		)

add_library(native-lib SHARED ${sources} ${resources} ${platform} ${rendering})


find_library(log-lib log )

target_link_libraries(native-lib
        ${log-lib} -lGLESv3 -landroid)