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

set(TARGET_NAME modb)

add_executable(${TARGET_NAME})


target_sources(${TARGET_NAME} 
    PRIVATE main.cpp
    PRIVATE Plane.cpp
    PRIVATE Point.cpp
    PRIVATE Rectangle.cpp
    PRIVATE DatabaseResource.Plane.cpp
)

###################################################################
###################### MAIN PROJECT ###############################
# There is need to test environment above code
set(TARGET_NAME bdb-test)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE bdb-test.cpp
    PRIVATE Plane.cpp
    PRIVATE Rectangle.cpp
    PRIVATE Point.cpp
    PRIVATE DatabaseResource.Plane.cpp
)

