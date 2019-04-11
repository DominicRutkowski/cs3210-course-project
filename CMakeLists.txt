cmake_minimum_required(VERSION 3.13)
project(cs3210_course_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(cs3210_course_project main.cpp Environment.cpp Environment.hpp Unit.hpp Obstacle.cpp Obstacle.hpp ViableUnit.cpp ViableUnit.hpp Plant.cpp Plant.hpp Animal.cpp Animal.hpp Organism.hpp Organism.cpp)
