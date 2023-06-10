link_libraries(modb)

###################################################################

add_executable(load_data
    load_data.cpp
)

add_executable(analyze_data
    analyze_data.cpp
)

###################################################################

add_executable(intersection_exhaustive 
    intersection_exhaustive.cpp
)

add_executable(intersection_query
    intersection_query.cpp
)

###################################################################

add_executable(asio_test
    asio_test.cpp
)

###################################################################

add_executable(load_query
    load_query.cpp
)

###################################################################

add_executable(intersection_plot
    intersection_plot.cpp
)
