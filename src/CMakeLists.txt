# ###################################################################

# set(TARGET_NAME bdb_example_load)

# add_executable(${TARGET_NAME})

# target_sources(${TARGET_NAME} 
#     PRIVATE bdb_examples/excxx_example_database_load.cpp
#     PRIVATE bdb_examples/MyDb.cpp
# )

# ###################################################################

# set(TARGET_NAME bdb_example_read)

# add_executable(${TARGET_NAME})

# target_sources(${TARGET_NAME} 
#     PRIVATE bdb_examples/excxx_example_database_read.cpp
#     PRIVATE bdb_examples/MyDb.cpp
# )

# ###################################################################

set(TARGET_NAME modb)

add_executable(${TARGET_NAME})


target_sources(${TARGET_NAME} 
    PRIVATE main.cpp
    PRIVATE Object.cpp
    PRIVATE Point.cpp
    PRIVATE Region.cpp
    PRIVATE DatabaseResource.cpp
)


####################################################################

set(TARGET_NAME pistache)
set(ENV{PKG_CONFIG_PATH} "/home/onur/moving-object-db-system/lib/pistache/build/meson-private/libpistache.pc")
set(ENV{PKG_CONFIG_LIBDIR} "/home/onur/moving-object-db-system/lib/pistache/build/meson-private/")

# find_package(libpistache REQUIRED)

find_package(PkgConfig)
# pkg_search_module(OPENSSL REQUIRED openssl)
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)

pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)
# add_executable(${TARGET_NAME} FooServer.cpp)
# target_link_libraries(${TARGET_NAME} PkgConfig::Pistache)

###################################################################
###################### MAIN PROJECT ###############################
# There is need to test environment above code
set(TARGET_NAME bdb-test)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE bdb-test.cpp
    PRIVATE Object.cpp
    PRIVATE Region.cpp
    PRIVATE Point.cpp
    PRIVATE DatabaseResource.cpp
    PRIVATE IndexService.cpp
    PRIVATE MyVisitor.cpp
)

