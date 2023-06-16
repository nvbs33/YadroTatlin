cmake_minimum_required(VERSION 3.20)
project(TapeTest)

# Add all the source files in the test directory
file(GLOB TEST_SOURCES "*.cpp")

# Include the headers from the include directory
include_directories(../include)

# Create an executable from the test files
add_executable(TapeTest ${TEST_SOURCES})

# Link the test executable with the project library
target_link_libraries(TapeTest Tape)