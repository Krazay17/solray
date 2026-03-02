cmake_minimum_required(VERSION 3.20)
project(solray)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_C_STANDARD 11)
add_subdirectory(vendored/RAYLIB EXCLUDE_FROM_ALL)

file(GLOB_RECURSE SOURCES "src/*.c")
set(RESOURCE_FILE "resource/resources.rc")

#---- Client App ----
add_executable(solray ${SOURCES} ${RESOURCE_FILE})
target_include_directories(solray PRIVATE
    "src"
    "vendored/enet/include"
)
if(WIN32)
    target_link_libraries(solray PRIVATE
        raylib
    
        ws2_32
        winmm
    )
else()
    target_link_libraries(solray PRIVATE
        raylib
    
        GL
        rt
    )
endif()

if(WIN32)
    #target_link_options(solray PRIVATE -mwindows)
    target_compile_definitions(solray PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

#---- Server App ----
add_executable(solray_server server/main.c)

target_include_directories(solray_server PRIVATE 
    "src" 
    "vendored/enet/include"
)

if(WIN32)
    target_link_libraries(solray_server PRIVATE ws2_32 winmm)
endif()
