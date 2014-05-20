cmake_minimum_required(VERSION 2.8.3)
project(sinetest)

add_executable(sintest sintest.cpp)
target_link_libraries(sintest rt)
