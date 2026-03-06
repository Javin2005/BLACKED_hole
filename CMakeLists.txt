cmake_minimum_required(VERSION 3.10)
project(Blacked_hole)

set(CMAKE_CXX_STANDARD 17)

# Berätta för CMake att leta i Homebrew-mappen på Mac
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

find_package(raylib REQUIRED)

# Inkludera mappar
include_directories(include)
include_directories(/opt/homebrew/include) # Extra säkerhet för Mac

file(GLOB SOURCES "src/*.cpp")

add_executable(Blacked_Hole ${SOURCES})

# Länka biblioteket
target_link_libraries(Blacked_Hole raylib)