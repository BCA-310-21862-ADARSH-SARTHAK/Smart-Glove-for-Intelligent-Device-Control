cmake_minimum_required(VERSION 3.5)

project(GloveRobotController)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

include_directories(
    /usr/include/eigen3
)

add_executable(glove_robot_control
    glove_robot_control.cpp
)

add_executable(read_command_test
    read_command_test.cpp
)

target_link_libraries(glove_robot_control
    Threads::Threads
)

target_link_libraries(read_command_test
    Threads::Threads
)