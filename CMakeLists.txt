cmake_minimum_required(VERSION 3.10)

project(BambooBrain)
set(CMAKE_C_STANDARD 99)

# LIB SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.c" "src/*.h")

# EXECUTABLE
add_executable(bamboo ${SOURCES})
target_link_libraries(bamboo ${SDL2_LIBRARIES})
file(COPY datasets DESTINATION ${CMAKE_BINARY_DIR})

# PLAYGROUND
add_executable(test1 playground/test1.c ${SOURCES})
target_link_libraries(test1 ${SDL2_LIBRARIES})
file(COPY datasets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(test2 playground/test2.c ${SOURCES})
target_link_libraries(test2 ${SDL2_LIBRARIES})
file(COPY datasets DESTINATION ${CMAKE_BINARY_DIR})
