cmake_minimum_required(VERSION 3.0.0)
project(opengl_learn VERSION 0.1.0)
include_directories(./glad/include)
#include_directories(./glew-2.1.0/include)
include_directories(../linux/glfw/include)
include_directories(./opengl_learn/vendor/stb)
include_directories(./opengl_learn/vendor/)
#add_executable(opengl_learn opengl_learn/opengltriangles.cpp)
#add_executable(opengl_learn opengl_learn/openglframebuffer.cpp)
add_executable(opengl_learn
    glad/src/glad.c
    opengl_learn/opengldispatchcomputer1.cpp
)
#opengl_learn/opengldispatchcomputer1.cpp
#add_executable(opengl_learn opengl_learn/opengreadbufferpixel.cpp)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
# Link against libglfw3.a
target_link_libraries(opengl_learn /root/umd/openglworkspace/linux/glfw/build/src/libglfw3.a)

target_link_libraries(opengl_learn GLEW)
target_link_libraries(opengl_learn /usr/local/lib/x86_64-linux-gnu/libGL.so)
# target_link_libraries(opengl_learn ${OPENGL_LIBRARIES})
