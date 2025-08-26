cmake_minimum_required(VERSION 3.24)
project(engine)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

#add glad
add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC headers)

#add the buffer manager
add_library(buffers STATIC src/buffers.cpp)
target_include_directories(buffers PUBLIC headers)

#add shaders
add_library(shaders STATIC src/Shader.cpp)
target_include_directories(shaders PUBLIC headers)

#add camera class
add_library(camera STATIC src/Camera.cpp)
target_include_directories(camera PUBLIC headers)

#stb image
add_library(stb_image STATIC src/stb_image.cpp)
target_include_directories(stb_image PUBLIC headers)

#mesh class
add_library(mesh STATIC src/Mesh.cpp)
target_include_directories(mesh PUBLIC headers)

#model
add_library(model STATIC src/Model.cpp)
target_include_directories(model PUBLIC headers ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(model PUBLIC mesh assimp)

#add_library(model STATIC src/Model.cpp)
#target_include_directories(model PUBLIC headers)
#target_link_libraries(model PUBLIC mesh assimp
##target_include_directories(model PUBLIC headers ${ASSIMP_INCLUDE_DIRS})


#main program
add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE headers)
target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw OpenGL::GL glm::glm  assimp stb_image buffers shaders camera mesh model)
