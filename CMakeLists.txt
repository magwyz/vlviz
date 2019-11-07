cmake_minimum_required(VERSION 2.8)

project(vlviz)

find_package(OpenCV REQUIRED)

set(SOURCES src/main.cpp
            src/cameracapturer.cpp
            src/socket.cpp
            src/encoder/encoder.cpp
            src/sender.cpp
            src/receiver.cpp
            src/decoder/decoder.cpp
            src/codec/quantizedframe.cpp)

set(HEADERS include/cameracapturer.h
            include/errors.h
            include/log.h
            include/socket.h
            include/messagefifo.h
            include/encoder/encoder.h
            include/sender.h
            include/encoder/encoderevent.h
            include/receiver.h
            include/decoder/decoder.h
            include/messagehandler.h
            include/codec/definitions.h
            include/codec/quantizedframe.h
            include/codec/rawframe.h
            include/codec/helpers.h)

include_directories(include
                    include/encoder
                    include/decoder
                    include/codec)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(vlviz ${SOURCES} ${HEADERS})

target_link_libraries(vlviz ${OpenCV_LIBS})

