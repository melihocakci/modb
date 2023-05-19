cmake_minimum_required(VERSION 3.16.3)

project(modb)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Werror)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS serialization)

set(Boost_DEBUG ON)

include_directories(
    PRIVATE include
    PRIVATE ${Boost_INCLUDE_DIRS}
)

link_libraries(
    db_cxx
    spatialindex
    ${Boost_LIBRARIES}
)

add_subdirectory(src/modb)

add_subdirectory(src/examples)

################### BACKEND PISTACHE #################################
####################################################################

# set(TARGET_NAME pistache)
# set(ENV{PKG_CONFIG_PATH} "/home/onur/moving-object-db-system/lib/pistache/build/meson-private/libpistache.pc")
# set(ENV{PKG_CONFIG_LIBDIR} "/home/onur/moving-object-db-system/lib/pistache/build/meson-private/")

# find_package(libpistache REQUIRED)

# cmake_minimum_required(VERSION 3.12)
# project(PistacheExample)

# set(CMAKE_CXX_STANDARD 17)

# ############################
# ##      SOURCE FILES      ##
# ############################
# add_executable(${PROJECT_NAME} src/FooServer.cpp)

# #####################################
# ##      HEADERS SEARCH PATHS       ##
# ##################################### 
# set(PROJECT_INCLUDE_DIR "src/include")
# set(PISTACHE_INCLUDE_DIR "lib/pistache/include")

# set(HEADER_SEARCH_PATHS ${PROJECT_INCLUDE_DIR} ${PISTACHE_INCLUDE_DIR})

# #####################################
# ##      LIBRARY SEARCH PATHS       ##
# #####################################
# set(PISTACHE_LIBRARY "${PROJECT_SOURCE_DIR}/lib/pistache/build/src/libpistache.so")
# set(EXTRA_LIBRARY "-pthread -lssl")

# set(LIBRARIES_SEARCH_PATHS ${PISTACHE_LIBRARY} ${EXTRA_LIBRARY})

# #######################################
# ##      ADDING HEADERS LIBRARY       ##
# #######################################
# include_directories(${HEADER_SEARCH_PATHS})
# target_link_libraries(${PROJECT_NAME} ${LIBRARIES_SEARCH_PATHS})

# # find_package(PkgConfig)
# # # pkg_search_module(OPENSSL REQUIRED openssl)
# # # find_package(OpenSSL REQUIRED PRIVATE COMPONENTS SSL Crypto)

# # pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)
# # # add_executable(${TARGET_NAME} FooServer.cpp)
# # # target_link_libraries(${TARGET_NAME} PkgConfig::Pistache)
