find_package(Boost  COMPONENTS filesystem system iostreams REQUIRED)
# include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../lib/gnuplot-iostream") # if library is external 
include_directories(../include)

set(CMAKE_BUILD_TYPE Debug)

set(INSTALL_DIR /usr/local/bin)

set (DIR geometry)
set (SOURCES
        Intersection
        )

foreach (test ${SOURCES})
    add_executable(test-${DIR}-${test} ${DIR}/${test}.cc)
    target_link_libraries(test-${DIR}-${test} ${SIDX_LIB_NAME})
    install(TARGETS test-${DIR}-${test} DESTINATION ${INSTALL_DIR})

endforeach()

set (DIR rtree)
set (SOURCES
        Exhaustive
        Generator
        RTreeBulkLoad
        GeneratorNormalLocation
        RTreeLoad
        RTreeQuery)


if(Boost_FOUND)
    foreach (test ${SOURCES})
        include_directories(${Boost_INCLUDE_DIRS}) 
        add_executable(test-${DIR}-${test} ${DIR}/${test}.cc)
        target_link_libraries(test-${DIR}-${test} ${SIDX_LIB_NAME} ${Boost_LIBRARIES})
        install(TARGETS test-${DIR}-${test} DESTINATION ${INSTALL_DIR})

    endforeach()
endif()

set (DIR mvrtree)
set (SOURCES
        Exhaustive
        Generator
        MVRTreeLoad
        MVRTreeQuery
        )


foreach (test ${SOURCES})
    add_executable(test-${DIR}-${test} ${DIR}/${test}.cc)
    target_link_libraries(test-${DIR}-${test} ${SIDX_LIB_NAME})
    install(TARGETS test-${DIR}-${test} DESTINATION ${INSTALL_DIR})

endforeach()

set (DIR tprtree)
set (SOURCES
        Exhaustive
        Generator
        TPRTreeLoad
        TPRTreeQuery
        )


foreach (test ${SOURCES})
    add_executable(test-${DIR}-${test} ${DIR}/${test}.cc ${DIR}/RandomGenerator.cc)
    target_link_libraries(test-${DIR}-${test} ${SIDX_LIB_NAME})
    install(TARGETS test-${DIR}-${test} DESTINATION ${INSTALL_DIR})
endforeach()




add_subdirectory(gtest)
