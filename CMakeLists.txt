cmake_minimum_required(VERSION 3.10)

project(BambooBrain)
set(CMAKE_C_STANDARD 99)

# LIB SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# LIB SDL2 IMAGES
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2IMAGE REQUIRED SDL2_image)

file(GLOB SOURCES "src/*.c" "src/*.h" "src/dynamic/*.c" "src/dynamic/*.h")

# EXECUTABLE
add_executable(bamboo ${SOURCES})
target_link_libraries(bamboo PRIVATE m ${SDL2_LIBRARIES}  ${SDL2IMAGE_LIBRARIES})
file(COPY datasets DESTINATION ${CMAKE_BINARY_DIR})
