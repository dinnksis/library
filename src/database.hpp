#pragma once
#include <../sqlite/sqlite3.h>
#include <string>
#include <vector>
#include "book.hpp"

class DatabaseManager {
public:
    DatabaseManager(const std::string& db_path);
    ~DatabaseManager();
    
    // Книги
    bool addBook(const Book& book);
    bool deleteBook(int id);
    std::vector<Book> getAllBooks();
    bool updateBookStatus(int id, bool borrowed);

private:
    sqlite3* db_;
    
    bool execute(const std::string& sql);
    static int bookCallback(void* data, int argc, char** argv, char** colNames);
};