aux_source_directory(. TEST_SRC_LIST)
foreach(TEST_SRC ${TEST_SRC_LIST})
    STRING(REGEX REPLACE "^\\./|\\.c[a-zA-Z0-9_]*$" "" TEST_EXE_NAME ${TEST_SRC})
    message(STATUS "添加测试程序:${TEST_EXE_NAME}")
    add_executable(${TEST_EXE_NAME} ${TEST_SRC})

    if(WIN32)
        target_link_libraries(${TEST_EXE_NAME} ${CMAKE_PROJECT_NAME}_shared ${LINK_LIB_LIST})
    else()
        target_link_libraries(${TEST_EXE_NAME} ${CMAKE_PROJECT_NAME}_shared ${LINK_LIB_LIST} pthread)
    endif()

endforeach(TEST_SRC ${TEST_SRC_LIST})