

add_library(book STATIC book.cpp book.hpp)
add_library(database STATIC database.cpp database.hpp)
add_library(lib STATIC lib.cpp lib.hpp)

# связывание библиотек
target_link_libraries(database PRIVATE sqlite)
target_link_libraries(lib PRIVATE book database)


add_executable(main main.cpp)
target_link_libraries(main PRIVATE lib)

foreach(target IN ITEMS main book database lib)
    target_include_directories(${target} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../sqlite
    )
endforeach()