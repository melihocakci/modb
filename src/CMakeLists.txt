###################################################################

set(TARGET_NAME bdb_example_load)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE bdb_examples/excxx_example_database_load.cpp
    PRIVATE bdb_examples/MyDb.cpp
)

###################################################################

set(TARGET_NAME bdb_example_read)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE bdb_examples/excxx_example_database_read.cpp
    PRIVATE bdb_examples/MyDb.cpp
)
