cmake_minimum_required(VERSION 3.16.3)

project(bdb-test)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

###################################################################

set(TARGET_NAME mos)

add_executable(${TARGET_NAME})

target_include_directories(${TARGET_NAME} 
    PRIVATE include
)

target_sources(${TARGET_NAME} 
    PRIVATE src/main.cpp
    PRIVATE src/SequenceExample.cpp
)

target_link_libraries(${TARGET_NAME} 
    db_cxx
    spatialindex
)

set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)