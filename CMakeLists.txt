cmake_minimum_required(VERSION 3.0.0)
project(lab_db VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE PROJ_FILES
        src/*.c
        src/*.h
        )

add_executable(lab_db ${PROJ_FILES})
        
include_directories(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
