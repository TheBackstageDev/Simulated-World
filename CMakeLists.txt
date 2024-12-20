cmake_minimum_required(VERSION 3.10)

project(COSMIC_PROJECT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE SOURCES "${SOURCE_FOLDER}/*.cpp")

add_executable(Main ${SOURCES})

target_link_libraries(Main "${LIB_DIR}/libraylibdll.a")  

set_target_properties(Main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})