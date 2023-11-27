cmake_minimum_required(VERSION 3.10)

project(asio)

set(CMAKE_BUILD_TYPE Debug)

set(CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)


add_subdirectory(src/Basic_Skills)

add_subdirectory(src/Introduction_to_Sockets)

