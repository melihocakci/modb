cmake_minimum_required(VERSION 3.16.3)

project(modbs)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost  COMPONENTS filesystem system iostreams REQUIRED)

set_property(GLOBAL PROPERTY CXX_STANDARD 17)

###################################################################

set(TARGET_NAME leaf_plot)

add_executable(${TARGET_NAME})

target_include_directories(${TARGET_NAME}
    PRIVATE include
    PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_NAME} 
    db_cxx
    spatialindex
    ${Boost_LIBRARIES}
)

target_sources(${TARGET_NAME} 
    PRIVATE src/leaf_plot.cpp
)