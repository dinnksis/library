#include "lib.hpp"
#include <algorithm>
#include "book.hpp"

//конструктор инициализации подключения к БД и загрузка данные
Library::Library(const std::string& db_path) : db_(db_path) {
    updateCache(); //згрузка книг из БД в кэш
}

//добавление книги с сохранением в бд
void Library::addBook(const Book& book) {
    if (db_.addBook(book)) {
        books_.push_back(book);
    }
}

//удаление по id
bool Library::deleteBook(int id) {
    if (db_.deleteBook(id)) {
        auto it = std::remove_if(books_.begin(), books_.end(), 
            [id](const Book& b) { return b.getId() == id; });
        
        if (it != books_.end()) {
            books_.erase(it, books_.end());
            return true;
        }
    }
    return false;
}


bool findId(const Book& b, int id) {
    return b.getId() == id;
}

bool findTitile(const Book& b, std::string title) {
    return b.getTitle() == title;
}

bool findAuthor(const Book& b, std::string author) {
    return b.getAuthor() == author;
}

//все книги из кэша
const std::vector<Book>& Library::getAllBooks() const {
    return books_;
}

//поиск книг по автору (работает с локальным кэшем)
std::vector<Book> Library::findBooksByAuthor(const std::string& author) const {
    std::vector<Book> result;
    for (const auto& book : books_) {
        if (book.getAuthor() == author) {
            result.push_back(book);
        }
    }
    return result;
}

//поиск книг по названию (работает с локальным кэшем)
std::vector<Book> Library::findBooksByTitle(const std::string& title) const {
    std::vector<Book> result;
    for (const auto& book : books_) {
        if (book.getTitle() == title) {
            result.push_back(book); 
        }
    }
    return result;
}

//взять книгу (обновляем и БД, и кэш)
bool Library::borrowBook(int id) {
    auto it = std::find_if(books_.begin(), books_.end(),
        [id](const Book& b) { return b.getId() == id; });

    if (it != books_.end() && !it->isBorrowed()) {
        if (db_.updateBookStatus(id, true)) {
            it->setBorrowed(true);
            return true;
        }
    }
    return false; 
}

//вернуть книгу (обновляем и БД, и кэш)
bool Library::returnBook(int id) {
    auto it = std::find_if(books_.begin(), books_.end(),
        [id](const Book& b) { return b.getId() == id; });

    if (it != books_.end() && it->isBorrowed()) {
        if (db_.updateBookStatus(id, false)) {
            it->setBorrowed(false);
            return true;
        }
    }
    return false;
}

bool isAvailable(const Book& book){
    return !book.isBorrowed();
}
size_t Library::getBookCount() const{
    return books_.size();
} 
size_t Library::getAvailableCount() const{
    return std::count_if(books_.begin(), books_.end(), isAvailable);
}


//обновление кэша из БД
void Library::updateCache() {
    books_ = db_.getAllBooks(); //получение актуальные данные из БД
}

