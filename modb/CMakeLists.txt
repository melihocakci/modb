add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/Object.cpp
        src/Point.cpp
        src/Region.cpp
        src/DatabaseManager.cpp
        src/IndexManager.cpp
        src/IndexVisitor.cpp
        src/Common.cpp
        src/SendDataWS.cpp
        src/IntersectionSendOption.cpp
        src/Timer.cpp   
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        db_cxx
        spatialindex
        nlohmann::json
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -fpic
)
