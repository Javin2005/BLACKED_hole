cmake_minimum_required(VERSION 3.10)
project(Blacked_Hole)

set(CMAKE_CXX_STANDARD 17)

# --- DETTA GÖR ATT DET FUNKAR FÖR BÅDA ---
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
elseif(WIN32)
    # Detta pekar på mappen som skapades i Steg 1
    list(APPEND CMAKE_PREFIX_PATH "C:/raylib/raylib/lib/cmake/raylib")
endif()

find_package(raylib REQUIRED)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(Blacked_Hole ${SOURCES})
target_link_libraries(Blacked_Hole raylib)