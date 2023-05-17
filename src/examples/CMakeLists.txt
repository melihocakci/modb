link_libraries(modb)

###################################################################

set(TARGET_NAME load_data)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE load_data.cpp
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
