cmake_minimum_required(VERSION 3.23)
project(project_2)

set(CMAKE_CXX_STANDARD 14)


add_executable(project_2 spaceTravel.cpp  )

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(project_2 PRIVATE
        OpenGL::GL
        GLUT::GLUT
        GLEW::GLEW
        ${OPENGL_LIBRARIES}  # Link against the OpenGL libraries directly
        glu32  # Link against GLU library directly
        )
