cmake_minimum_required(VERSION 3.20)
project(LearnOpenGL C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(LearnOpenGL ${SOURCES} src/glad.c)

target_include_directories(LearnOpenGL PRIVATE include src)

# Prebuilt macOS (arm64) GLFW static library
target_link_libraries(LearnOpenGL PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/manual-link/libglfw3.a
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreFoundation"
    "-framework QuartzCore"
    "-framework OpenGL"
)

# main.cpp loads "./shaders/..." and "./assets/..." relative to the working directory
set_target_properties(LearnOpenGL PROPERTIES
    XCODE_GENERATE_SCHEME ON
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
