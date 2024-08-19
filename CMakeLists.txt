cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(myproject VERSION 1.0)

# Include the directories for GLAD and your project
include_directories(/usr/local/include/)
link_directories(/usr/local/lib/)

set(CMAKE_CXX_STANDARD 17)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")
set(CMAKE_BUILD_TYPE Debug)

# Explicitly set the output directory for the executable to the main directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(minipad main.cpp)

find_package(Freetype REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)

# Link libraries including macOS frameworks and required compression libraries
target_link_libraries(minipad PRIVATE 
    glad 
    glfw 
    glm::glm 
    Freetype::Freetype 
    ${OPENGL_LIBRARIES} 
    "-framework QuartzCore -framework Cocoa -framework OpenGL -framework IOKit"
    -lbz2
    -lz
)
