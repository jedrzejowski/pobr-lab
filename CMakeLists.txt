cmake_minimum_required(VERSION 3.17)
project(pobr_lab)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(pobr_lab main.cpp
        types.h
        asserts.h
        lib.h
        lab2.h
        lab3.h
)

target_link_libraries(pobr_lab PRIVATE ${OpenCV_LIBS})
