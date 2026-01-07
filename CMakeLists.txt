cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW)

project(betatty VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Curses REQUIRED)

include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    ${CURSES_INCLUDE_DIR}
)

add_executable(
	betatty
	src/main.cpp
	src/chunk.cpp
	src/packet.cpp
	src/helper.cpp
	src/dataHandler.cpp
	src/global.cpp
	src/chat.cpp
	src/world.cpp
	src/lut.cpp
)

target_link_libraries(
	betatty
    deflate
    ${CURSES_LIBRARIES}
)
