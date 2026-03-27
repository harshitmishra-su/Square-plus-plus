cmake_minimum_required(VERSION 3.31)

if(TARGET)
    set(CMAKE_C_COMPILER "zig;cc;-target;${TARGET}")
    set(CMAKE_CXX_COMPILER "zig;c++;-target;${TARGET}")

    if(${TARGET} MATCHES "windows")
        set(CMAKE_SYSTEM_NAME Windows)
    elseif(${TARGET} MATCHES "linux")
        set(CMAKE_SYSTEM_NAME Linux)
    endif()
endif()

project(squarepp)

include(FetchContent)
set(RAYLIB_VERSION 5.5)
set(BUILD_SHARED_LIB OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

set(SOURCE src/squarepp.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} raylib)

if(WIN32)
    # Zig/MinGW needs these linked explicitly for raylib
    target_link_libraries(${PROJECT_NAME} winmm gdi32 shell32 user32)
    # Prevent a console window from popping up on Windows
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
