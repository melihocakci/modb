cmake_minimum_required(VERSION 3.16.3)

project(modb LANGUAGES CXX)


############################## configurations ##############################

add_compile_options(-Wall -Werror)

set(CMAKE_CXX_STANDARD 20)


############################## libraries ##############################

find_package(Boost REQUIRED COMPONENTS serialization iostreams filesystem)
include_directories(BEFORE SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(BEFORE ${Boost_LIBRARIES})


############################## options ##############################

option(BUILD_EXAMPLES "build examples for modb" OFF)


############################## targets ##############################

add_subdirectory(modb)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
