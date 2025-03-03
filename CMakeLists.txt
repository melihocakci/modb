cmake_minimum_required(VERSION 3.16.3)

project(modb LANGUAGES CXX)

add_compile_options(-Wall -Werror)

set(CMAKE_CXX_STANDARD 20)


find_package(Boost REQUIRED COMPONENTS serialization iostreams filesystem)
include_directories(BEFORE SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(BEFORE ${Boost_LIBRARIES})


add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        db_cxx
        spatialindex
        boost_serialization
        boost_iostreams
        boost_filesystem
)


option(BUILD_EXAMPLES "build examples for modb" OFF)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
