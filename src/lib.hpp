#ifndef LIBRARY
#define LIBRARY

#include <vector>
#include <optional>
#include "book.hpp"

class Library {
public:
    void addBook(const Book& book);
    bool deleteBook(int id);
    const std::vector<Book>& getAllBooks() const;
    
    std::vector<Book> findBooksByAuthor(const std::string& author) const;
    std::vector<Book> findBooksByTitle(const std::string& title) const;
    bool borrowBook(int id);   // взять книгу
    bool returnBook(int id);   // вернуть книгу

private:
    std::vector<Book> books_;
};

#endif