cmake_minimum_required(VERSION 4.0)
project(Candriver CXX)

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

target_include_directories(candriver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# test

add_executable(test_candriver_listen test/test_candriver_listen.cpp)
target_link_libraries(test_candriver_listen PRIVATE candriver)


add_executable(test_candriver_send test/test_candriver_send.cpp)
target_link_libraries(test_candriver_send PRIVATE candriver)

add_executable(test_candriver_dmJ4310 test/test_candriver_dm-J4310.cpp)
target_link_libraries(test_candriver_dmJ4310 PRIVATE candriver)

add_executable(test_candriver_dmJ8090 test/test_candriver_dm-J8090.cpp)
target_link_libraries(test_candriver_dmJ8090 PRIVATE candriver)

add_executable(test_candriver_sendM3508 test/test_candriver_sendM3508.cpp)
target_link_libraries(test_candriver_sendM3508 PRIVATE candriver)

add_executable(test_candriver_sendM2006 test/test_candriver_sendM2006.cpp)
target_link_libraries(test_candriver_sendM2006 PRIVATE candriver)


install(TARGETS candriver
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

# 输出 ${PROJECT_NAME} 变量
message("PROJECT_NAME:" ${PROJECT_NAME})

install(TARGETS
  candriver
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS
    test_candriver_listen test_candriver_send test_candriver_dmJ4310 test_candriver_dmJ8090
    DESTINATION test/${PROJECT_NAME}
)

install(DIRECTORY include DESTINATION include/${PROJECT_NAME})