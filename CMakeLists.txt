cmake_minimum_required(VERSION 3.20)
project(LearnOpenGL C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(LearnOpenGL ${SOURCES} src/glad.c
        src/pipeline/VertexBuffer.h
        src/pipeline/VertexBuffer.cpp
        src/pipeline/VertexArray.h
        src/stb_image.cpp
        src/files/FileSystem.h
        src/graphics/Shader.cpp
        src/utils/Clock.cpp
        src/utils/Clock.h
        src/camera/Directions.h)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
target_include_directories(LearnOpenGL PRIVATE include src ${CMAKE_BINARY_DIR}/configuration)

# Prebuilt macOS (arm64) GLFW static library
target_link_libraries(LearnOpenGL PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/manual-link/glfw/libglfw3.a
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework QuartzCore"
        "-framework OpenGL"
)

# GLM is vendored as source (header-only); exposes the glm::glm target
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/manual-link/glm EXCLUDE_FROM_ALL)
target_link_libraries(LearnOpenGL PRIVATE glm::glm)

# main.cpp loads "./shaders/..." and "./assets/..." relative to the working directory
set_target_properties(LearnOpenGL PROPERTIES
        XCODE_GENERATE_SCHEME ON
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Make shaders/assets reachable from the binary's directory (CLion runs from the build dir)
add_custom_command(TARGET LearnOpenGL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:LearnOpenGL>/shaders
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:LearnOpenGL>/assets
)
