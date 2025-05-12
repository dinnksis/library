cmake_minimum_required(VERSION 3.20)
project(library)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)

#SQLite библиотека
add_library(sqlite STATIC sqlite/sqlite3.c)
target_include_directories(sqlite PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


add_subdirectory(src)