cmake_minimum_required (VERSION 2.8.11)
project (Knapsack-Problem)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable (brute_force ../src/brute_force.cc
                            ../src/instances.cc)

add_executable (heuristic ../src/heuristic.cc
                            ../src/instances.cc)
