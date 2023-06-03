link_libraries(modb)

###################################################################

add_executable(load_data
    load_data.cpp
)

add_executable(analyze_data
    analyze_data.cpp
)

###################################################################

add_executable(query_exhaustive 
    query_exhaustive.cpp
)

add_executable(query_modb
    query_modb.cpp
)

###################################################################

add_executable(asio_test
    asio_test.cpp
)
