#include "database.hpp"
#include <iostream>
#include <stdexcept>

DatabaseManager::DatabaseManager(const std::string& db_path) {
    if (sqlite3_open(db_path.c_str(), &db_) != SQLITE_OK) {
        throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db_)));
    }
    
    execute("CREATE TABLE IF NOT EXISTS books ("
           "id INTEGER PRIMARY KEY, "
           "title TEXT NOT NULL, "
           "author TEXT NOT NULL, "
           "year INTEGER, "
           "borrowed BOOLEAN DEFAULT 0)");
}

DatabaseManager::~DatabaseManager() {
    if (db_) {
        sqlite3_close(db_);
    }
}

bool DatabaseManager::addBook(const Book& book) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO books (id, title, author, year) VALUES (?, ?, ?, ?)";
    
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, book.getId());
    sqlite3_bind_text(stmt, 2, book.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, book.getAuthor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, book.getYear());
    
    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    
    if (!success) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db_) << std::endl;
    }
    
    return success;
}

bool DatabaseManager::deleteBook(int id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM books WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    
    return success;
}

std::vector<Book> DatabaseManager::getAllBooks() {
    std::vector<Book> books;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, title, author, year, borrowed FROM books";
    
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db_) << std::endl;
        return books;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        books.emplace_back(
            sqlite3_column_int(stmt, 0),  // id
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),  // title
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),  // author
            sqlite3_column_int(stmt, 3),  // year
            sqlite3_column_int(stmt, 4)   // borrowed
        );
    }
    
    sqlite3_finalize(stmt);
    return books;
}

bool DatabaseManager::updateBookStatus(int id, bool borrowed) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE books SET borrowed = ? WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, borrowed ? 1 : 0);
    sqlite3_bind_int(stmt, 2, id);
    
    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    
    return success;
}


bool DatabaseManager::execute(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

int DatabaseManager::bookCallback(void* data, int argc, char** argv, char** colNames) {
    auto* books = static_cast<std::vector<Book>*>(data);
    books->emplace_back(
        std::stoi(argv[0]),  // id
        argv[1],             // title
        argv[2],             // author
        std::stoi(argv[3]),  // year
        std::stoi(argv[4])   // borrowed
    );
    return 0;
}