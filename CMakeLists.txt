cmake_minimum_required(VERSION 3.16)
project(shm_array)

set(CMAKE_CXX_STANDARD 11)

FILE(GLOB SMFILES "./*.cpp")

link_libraries(pthread rt)
add_executable(creator test/creator.cpp ${SMFILES})
add_executable(destroyer test/destroyer.cpp ${SMFILES})
add_executable(first_hand test/first_hand.cpp ${SMFILES})
add_executable(second_hand test/second_hand.cpp ${SMFILES})

