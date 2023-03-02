cmake_minimum_required(VERSION 3.16.3)

project(bdb-test)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(BDB_DIR /usr/local/BerkeleyDB.18.1)

###################################################################

set(TARGET test)

add_executable(${TARGET})

target_include_directories(${TARGET} PRIVATE include)

target_sources(${TARGET} 
    PRIVATE src/test.c
)

target_link_libraries(${TARGET} 
    ${BDB_DIR}/lib/libdb.so
)