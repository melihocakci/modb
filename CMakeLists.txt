cmake_minimum_required(VERSION 3.16.3)

project(modb LANGUAGES CXX)


############################## configurations ##############################

add_compile_options(-Wall -Werror)

add_link_options(-pie)

set(CMAKE_CXX_STANDARD 20)


############################## libraries ##############################

find_package(Boost REQUIRED COMPONENTS serialization iostreams filesystem)

include_directories(
    ${Boost_INCLUDE_DIRS}
)

link_libraries(
    ${Boost_LIBRARIES}
)

add_library(json INTERFACE)
target_include_directories(json INTERFACE lib/json/single_include)
add_library(nlohmann::json ALIAS json)


############################## options ##############################

option(BUILD_EXAMPLES "build examples for modb" OFF)


############################## targets ##############################

add_subdirectory(modb)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
