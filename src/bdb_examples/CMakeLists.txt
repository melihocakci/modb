###################################################################

set(TARGET_NAME bdb_example_load)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE excxx_example_database_load.cpp
    PRIVATE MyDb.cpp
)

###################################################################

set(TARGET_NAME bdb_example_read)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE excxx_example_database_read.cpp
    PRIVATE MyDb.cpp
)
