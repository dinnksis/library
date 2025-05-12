#include "lib.hpp"
#include <iostream>
#include <iomanip>

//вывод списка книг
void printBooks(const std::vector<Book>& books) {
    if (books.empty()) {
        std::cout << "No books found\n";
        return;
    }

    //заголовок таблицы
    std::cout << "\n| ID  | Title                     | Author               | Year  | Status     |\n";
    std::cout << "|-----|---------------------------|----------------------|-------|------------|\n";

    for (const auto& book : books) {
        std::cout << "| " << std::setw(3) << book.getId() << " "
                  << "| " << std::setw(25) << std::left << book.getTitle() << std::right << " "
                  << "| " << std::setw(20) << std::left << book.getAuthor() << std::right << " "
                  << "| " << std::setw(5) << book.getYear() << " "
                  << "| " << std::setw(10) << (book.isBorrowed() ? "Borrowed" : "Available") << " |\n";
    }
    std::cout << std::endl;
}

int main() {
    try {
        //инициализация библиотеки с подключением к БД
        Library lib("library.db");
        
        std::cout << "=== test ===\n";

        //добавление книг
        std::cout << "\ndobavlenie\n";
        lib.addBook(Book(1, "Mumu", "Ivan Turgenev", 1949));
        lib.addBook(Book(2, "Master and Margarita", "Michail Bulgakov", 1967));
        lib.addBook(Book(3, "Prestuplenie and Nakazanie", "Fedor Dostoevski", 1866));
        lib.addBook(Book(4, "Mumu", "Ivan Turgenev", 1949)); // Дубликат названия

        //вывод всех книг
        printBooks(lib.getAllBooks());

        //поиск книг
        std::cout << "\npoisk Turgenev\n";
        printBooks(lib.findBooksByAuthor("Ivan Turgenev"));

        std::cout << "\npoisk 1984\n";
        printBooks(lib.findBooksByTitle("Mumu"));

        //взять
        std::cout << "\nborrow id 2\n";
        if (lib.borrowBook(2)) {
            std::cout << "vzali!\n";
        } else {
            std::cout << "ne vzali\n";
        }

        // статус
        std::cout << "\nbook ststus:\n";
        printBooks(lib.getAllBooks());

        //попытка взять занятую
        std::cout << "\n  borrow ID=2:\n";
        if (!lib.borrowBook(2)) {
            std::cout << "zanato\n";
        }

        //возврат
        std::cout << "\nreturn ID=2:\n";
        if (lib.returnBook(2)) {
            std::cout << "vernuli\n";
        }

        //удаление 
        std::cout << "\ndelete book ID=3:\n";
        if (lib.deleteBook(3)) {
            std::cout << "udalili\n";
        }

        //список книг
        std::cout << "\nbook list\n";
        printBooks(lib.getAllBooks());

        //статистика
        std::cout << "\nlib statistic\n";
        std::cout << "book count " << lib.getBookCount() << "\n";
        std::cout << "avaliable book count" << lib.getAvailableCount() << "\n";

    } catch (const std::exception& e) {
        std::cerr << "err: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}