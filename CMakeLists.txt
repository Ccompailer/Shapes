cmake_minimum_required(VERSION 3.25)
project(untitled2)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled2 main.cpp
        Shapes.h
        Point.h
        Vector3D.h)
find_package(Poco REQUIRED Net)
target_link_libraries(untitled2 PRIVATE ${Poco_LIBRARIES})
