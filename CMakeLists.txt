cmake_minimum_required(VERSION 3.16.3)

project(modb)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Werror )

set(Boost_DEBUG ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost COMPONENTS serialization)

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

add_subdirectory(src)
