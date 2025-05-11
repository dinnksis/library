add_library (book book.cpp book.hpp)
add_library (lib lib.cpp lib.hpp)

add_executable (main main.cpp lib.cpp book.cpp)