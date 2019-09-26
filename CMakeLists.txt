cmake_minimum_required(VERSION 2.8)

project(vlviz)

find_package(OpenCV REQUIRED)

set(SOURCES src/main.cpp
            src/cameracapturer.cpp)

set(HEADERS include/cameracapturer.h
            include/errors.h
            include/log.h)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(vlviz ${SOURCES} ${HEADERS})

target_link_libraries(vlviz ${OpenCV_LIBS})

