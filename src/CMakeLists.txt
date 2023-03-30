###################################################################

set(TARGET_NAME leaf_plot)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE leaf_plot.cpp
)

###################################################################

set(TARGET_NAME BtRecExample)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE BtRecExample.cpp
)

###################################################################

set(TARGET_NAME SequenceExample)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE SequenceExample.cpp
)

###################################################################

set(TARGET_NAME AccessExample)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} 
    PRIVATE AccessExample.cpp
)
