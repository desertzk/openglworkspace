cmake_minimum_required(VERSION 3.0.0)
project(opengl_learn VERSION 0.1.0)
include_directories(./glew-2.1.0/include)
add_executable(opengl_learn opengl_learn/opengltriangles.cpp)

target_link_libraries(opengl_learn glfw)
target_link_libraries(opengl_learn GLEW)
target_link_libraries(opengl_learn GL)
# target_link_libraries(opengl_learn ${OPENGL_LIBRARIES})