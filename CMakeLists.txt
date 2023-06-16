cmake_minimum_required(VERSION 3.20)
project(Tape)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system unit_test_framework)

include_directories(${Boost_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.cpp")

add_executable(Tape ${SOURCES} main.cpp)

add_executable(TapeTest ${SOURCES} test/test.cpp)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

#add_test(${PROJECT_NAME} my_test)
