cmake_minimum_required (VERSION 3.16)
project(astar)

set(CMAKE_CXX_STANDARD 20)
set(SRCDIR "${PROJECT_SOURCE_DIR}/src")
set(INCDIR "${PROJECT_SOURCE_DIR}/src")

set(EXNAME "astar")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${SRCDIR}/*.cpp" "${SRCDIR}/*.h")
add_executable (${EXNAME} ${SRC_FILES})

set_target_properties(
    ${EXNAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_include_directories(${EXNAME} PUBLIC ${INCDIR})