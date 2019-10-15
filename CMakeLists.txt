cmake_minimum_required(VERSION 2.8)

project(vlviz)

find_package(OpenCV REQUIRED)

set(SOURCES src/main.cpp
            src/cameracapturer.cpp
            src/socket.cpp
            src/encoder/encoder.cpp
            src/sender.cpp)

set(HEADERS include/cameracapturer.h
            include/errors.h
            include/log.h
            include/socket.h
            include/messagefifo.h
            include/encoder/encoder.h
            include/sender.h
            include/encoder/encoderevent.h)

include_directories(include include/encoder)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(vlviz ${SOURCES} ${HEADERS})

target_link_libraries(vlviz ${OpenCV_LIBS})

