cmake_minimum_required(VERSION 3.10)

project(Introduction_to_Sockets)

find_package(fmt)

file(GLOB_RECURSE sources ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

foreach(target_file ${sources})
   string(REGEX REPLACE "^.*/" "" target_name ${target_file})
   string(REGEX REPLACE "\.cpp$" "" target_name ${target_name})
   add_executable(${target_name} ${target_file})   
   target_link_libraries(${target_name} PRIVATE fmt::fmt)
endforeach(target_file sources)
