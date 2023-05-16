add_library(common OBJECT 
    Object.cpp
    Point.cpp
    Region.cpp
    DatabaseResource.cpp
    IndexService.cpp
    IndexVisitor.cpp
    Common.cpp
)

link_libraries(common)

###################################################################

set(TARGET_NAME modb)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE main.cpp
)

###################################################################

set(TARGET_NAME test_exhaustive)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE test_exhaustive.cpp
)

###################################################################

set(TARGET_NAME test_modb)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE test_modb.cpp
)

###################################################################
###################### MAIN PROJECT ###############################
# There is need to test environment above code

# set(TARGET_NAME bdb-test)

# add_executable(${TARGET_NAME})

# target_sources(${TARGET_NAME} 
#     PRIVATE bdb-test.cpp
# )

