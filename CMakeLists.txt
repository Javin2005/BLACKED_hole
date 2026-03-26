cmake_minimum_required(VERSION 3.10)
project(Blacked_Hole)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Skapa compile_commands.json (bra för autocomplete i t.ex. VS Code)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Tipsa CMake om var Raylib kan finnas på olika system
if(APPLE)
    # För Mac (Homebrew)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
else()
    # För Linux (Linuxbrew eller system-install)
    list(APPEND CMAKE_PREFIX_PATH "/home/linuxbrew/.linuxbrew")
endif()

# Nu hittar find_package oftast rätt automatiskt
find_package(raylib REQUIRED)

# Hämta alla källkodsfiler
file(GLOB SOURCES "src/3d/*.cpp")

add_executable(Blacked_Hole ${SOURCES})

# Inkludera dina egna headers
target_include_directories(Blacked_Hole PRIVATE include/3d)

# Länka raylib
target_link_libraries(Blacked_Hole raylib)

# Optimeringsflaggor för Release-läge
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(Blacked_Hole PRIVATE -O3)
else()
    target_compile_options(Blacked_Hole PRIVATE -g) # Debug-info
endif()