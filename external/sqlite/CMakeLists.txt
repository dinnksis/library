add_library(sqlite3 STATIC sqlite3.c)

target_include_directories(sqlite3 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Не показывать предупреждения SQLite
target_compile_options(sqlite3 PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W0>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-w>
)