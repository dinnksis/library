find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(library_gui
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
)

target_link_libraries(library_gui PRIVATE
    core_lib
    Qt6::Widgets
    Qt6::Core
)