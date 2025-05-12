#ifndef LIBRARY
#define LIBRARY

#include <vector>
#include <optional>
#include "book.hpp"
#include "database.hpp"

class Library {
public:
    explicit Library(const std::string& db_path = "library.db"); //подключение к бд

    void addBook(const Book& book);
    bool deleteBook(int id);
    const std::vector<Book>& getAllBooks() const;
    
    std::vector<Book> findBooksByAuthor(const std::string& author) const;
    std::vector<Book> findBooksByTitle(const std::string& title) const;
    bool borrowBook(int id);   // взять книгу
    bool returnBook(int id);   // вернуть книгу

    //статистика 
    bool isAvalible(const Book& book) const;
    size_t getBookCount() const; //общее кол-во книг
    size_t getAvailableCount() const; //кол-во свободных книг

private:
    std::vector<Book> books_;
    DatabaseManager db_; 

    void updateCache(); //обновление кэша бд

     //поиск книги в кэше (вспомогательный метод)
    std::vector<Book>::iterator findBookIterator(int id);
    std::vector<Book>::const_iterator findBookIterator(int id) const;
};

#endif