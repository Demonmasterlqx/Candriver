project(Candriver CXX)
cmake_minimum_required(VERSION 3.0)

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE -Ofast)
 
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O2  -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2  -Wall")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/test)

include_directories(include)

# lib

add_library(candriver 
    SHARED src/candriver.cpp)

# test

add_executable(test_candriver_listen test/test_candriver_listen.cpp)
target_link_libraries(test_candriver_listen PRIVATE candriver)


add_executable(test_candriver_send test/test_candriver_send.cpp)
target_link_libraries(test_candriver_send PRIVATE candriver)