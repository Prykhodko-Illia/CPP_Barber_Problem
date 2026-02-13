cmake_minimum_required(VERSION 4.1)
project(BarberProblem)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES "src/*.cpp")

add_executable(BarberProblem ${SOURCES})
