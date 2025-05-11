#include <iostream>
#include "lib.hpp"

int main() {
    Library lib;
    
    //добавление книг
    lib.addBook(Book(1, "Poltava", "Alexander Pushkin", 1829));
    lib.addBook(Book(2, "Poet", "Michail Lermontow", 1838));
    
    //поиск 
    auto books = lib.findBooksByAuthor("Alexander Pushkin");
    for (const auto& book : books) {
        std::cout << book.getTitle() << " (" << book.getYear() << ")\n";
    }

    return 0;
}