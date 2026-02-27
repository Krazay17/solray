cmake_minimum_required(VERSION 3.20)
project(solray)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_C_STANDARD 11)
add_subdirectory(vendored/RAYLIB EXCLUDE_FROM_ALL)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(solray ${SOURCES})
target_include_directories(solray PRIVATE
    "src"
)
target_link_libraries(solray PRIVATE
    raylib
)

if(WIN32)
    target_link_options(solray PRIVATE -mwindows)
endif()