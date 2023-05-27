cmake_minimum_required(VERSION 3.25)
project(sloth_raytracer)

set(CMAKE_CXX_STANDARD 11)

# Include SDL2
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/sdl2/2.26.5/include/")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include(FetchContent)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)

FetchContent_GetProperties(json)
if(NOT json_POPULATED) # Have we downloaded json yet?
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR})
endif()

add_executable(sloth_raytracer main.cpp image.h include/stb_image_write.h image.cpp geometry.h scene.h scene.cpp)

target_link_libraries(sloth_raytracer ${SDL2_LIBRARIES} nlohmann_json::nlohmann_json)
