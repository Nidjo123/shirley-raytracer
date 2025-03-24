cmake_minimum_required(VERSION 3.28)

project(shirley-raytracer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp src/util.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE sfml-graphics)

add_executable(interactive src/interactive.cpp src/util.cpp)
target_compile_features(interactive PRIVATE cxx_std_17)
target_link_libraries(interactive PRIVATE sfml-graphics)
