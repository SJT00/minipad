cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(myproject VERSION 1.0)

# Include the directories for GLAD and your project
include_directories(/usr/local/include/)
link_directories(/usr/local/lib/)

set(CMAKE_CXX_STANDARD 17)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")
set(CMAKE_BUILD_TYPE Debug)

# Explicitly set the output directory for the executable to the main directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(minipad main.cpp)

find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)

target_include_directories(minipad PUBLIC ${OPENGL_INCLUDE_DIR})

target_link_libraries(minipad "-framework QuartzCore")
target_link_libraries(minipad "-framework Cocoa")
target_link_libraries(minipad "-framework OpenGL")
target_link_libraries(minipad "-framework IOKit")
target_link_libraries(minipad glad glfw ${OPENGL_LIBRARIES})