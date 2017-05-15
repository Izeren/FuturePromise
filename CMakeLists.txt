cmake_minimum_required(VERSION 3.6)
project(FuturePromise)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        MyExceptions.cpp
        )
add_executable(FuturePromise ${SOURCE_FILES})
target_link_libraries (FuturePromise pthread)
