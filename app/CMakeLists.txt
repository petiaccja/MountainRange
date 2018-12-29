

cmake_minimum_required(VERSION 3.6.0)


include_directories(libs/include)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(sources
        src/main/cpp/native-lib.cpp
        src/main/cpp/GeometryTile.cpp
        src/main/cpp/GeometryTile.hpp
        src/main/cpp/Image.cpp
        src/main/cpp/Image.hpp
        src/main/cpp/Vertex.hpp
        )

set(extra
		src/main/cpp/BasicCamera.hpp
		src/main/cpp/BasicCamera.cpp
		src/main/cpp/PerspectiveCamera.hpp
		src/main/cpp/PerspectiveCamera.cpp
		)

set(rendering
        src/main/cpp/TileEntity.cpp
        src/main/cpp/TileEntity.hpp
        src/main/cpp/Scene.cpp
        src/main/cpp/Scene.hpp
        src/main/cpp/Renderer.cpp
        src/main/cpp/Renderer.hpp
		src/main/cpp/Platform.cpp
		src/main/cpp/Platform.hpp
		)

add_library(native-lib SHARED ${sources} ${rendering} ${extra})


find_library(log-lib log )

target_link_libraries(native-lib
        ${log-lib} -lGLESv3 -landroid)