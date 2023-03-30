cmake_minimum_required(VERSION 3.16.3)

project(modbs)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost  COMPONENTS filesystem system iostreams REQUIRED)

set_property(GLOBAL PROPERTY CXX_STANDARD 17)

include_directories(
    PRIVATE include
    PRIVATE ${Boost_INCLUDE_DIRS}
)

link_libraries(
    db_cxx
    spatialindex
    ${Boost_LIBRARIES}
)

###################################################################

set(TARGET_NAME leaf_plot)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE src/leaf_plot.cpp
)

###################################################################

set(TARGET_NAME BtRecExample)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE src/BtRecExample.cpp
)

###################################################################

set(TARGET_NAME SequenceExample)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE src/SequenceExample.cpp
)

###################################################################

set(TARGET_NAME AccessExample)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE src/AccessExample.cpp
)