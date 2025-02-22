add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        db_cxx
        spatialindex
        boost_serialization
        boost_iostreams
        boost_filesystem
)
