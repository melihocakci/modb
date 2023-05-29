link_libraries(modb)

###################################################################

add_executable(load_file
    load_file.cpp
)

###################################################################

add_executable(test_exhaustive 
    test_exhaustive.cpp
)

###################################################################

add_executable(test_modb
    test_modb.cpp
)

###################################################################
###################### MAIN PROJECT ###############################
# There is need to test environment above code

# set(TARGET_NAME bdb-test)

# add_executable(${TARGET_NAME})

# target_sources(${TARGET_NAME} 
#     PRIVATE bdb-test.cpp
# )

###################################################################

add_executable(asio_test
    asio_test.cpp
)