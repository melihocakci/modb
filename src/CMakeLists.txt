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

###################################################################

set(TARGET_NAME bdb-test)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE bdb-test.cpp
    PRIVATE DatabaseResource.cpp
)

###################################################################

set(TARGET_NAME modb)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE main.cpp
    PRIVATE Plane.cpp
    PRIVATE Point.cpp

)
