add_library(core_lib
    book.cpp
    book.hpp
    database.cpp
    database.hpp
    lib.cpp
    lib.hpp
)

target_link_libraries(core_lib PRIVATE sqlite3)

target_include_directories(core_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/sqlite
)