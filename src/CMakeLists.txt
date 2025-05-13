
add_library(book STATIC book.cpp book.hpp)
add_library(database STATIC database.cpp database.hpp)
add_library(lib STATIC lib.cpp lib.hpp)

#связывание библиотек
target_link_libraries(database PRIVATE sqlite)
target_link_libraries(lib PRIVATE book database)

#главное приложение с Qt
add_executable(main 
    main.cpp 
    mainwindow.cpp 
    mainwindow.hpp
)

add_executable(admin admin.cpp)
target_link_libraries(admin PRIVATE lib)

#Qt Widgets и Core
target_link_libraries(main PRIVATE 
    lib 
    Qt6::Widgets 
    Qt6::Core
)

#include-директории
foreach(target IN ITEMS book database lib main admin)
    target_include_directories(${target} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../sqlite
    )
endforeach()



