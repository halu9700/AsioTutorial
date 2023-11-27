cmake_minimum_required(VERSION 3.10)

project(Basic_Skills)

set(CXX_STANDARD 20)


file(GLOB_RECURSE sources CONFIGURE_DEPEND ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.hpp)



find_package(fmt)

foreach(target ${sources})
    string(REGEX REPLACE "^.*/" "" target_name ${target})
    string(REGEX REPLACE "\.cpp$" "" target_name ${target_name})
    add_executable(${target_name} ${target})
    target_link_libraries(${target_name} PRIVATE fmt::fmt)
endforeach()

