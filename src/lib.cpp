#include "lib.hpp"
#include <algorithm>
#include "book.hpp"

void Library::addBook(const Book& book) {
    books_.push_back(book);
}

bool Library::deleteBook(int id) {
    auto it = std::remove_if(books_.begin(), books_.end(), 
        [id](const Book& b) { return b.getId() == id; });
    
    if (it != books_.end()) {
        books_.erase(it, books_.end());
        return true;
    }
    return false;
}

//проверка что есть такая книга в библиотеке
bool findId(const Book& b,int id){
    return b.getId()==id;
}
bool findTitile(const Book& b,std::string title){
    return b.getTitle()==title;
}
bool findAuthor(const Book& b,std::string author){
    return b.getAuthor()==author;
}


const std::vector<Book>& Library::getAllBooks() const {
    return books_;
}


std::vector<Book> Library::findBooksByAuthor(const std::string& author) const {
    std::vector<Book> result;
    for (const auto& book : books_) {
        if (book.getAuthor() == author) {
            result.push_back(book);
        }
    }
    return result;
}

std::vector<Book> Library::findBooksByTitle(const std::string& title) const {
    std::vector<Book> result;
    for (const auto& book : books_) {
        if (book.getTitle() == title) {
            result.push_back(book); 
        }
    }
    return result;
}


bool Library::borrowBook(int id) {
    auto it = std::find_if(books_.begin(), books_.end(),
        [id](const Book& b) { return b.getId() == id; });

    if (it != books_.end() && !it->isBorrowed()) {
        it->setBorrowed(true);
        return true; //успешно взяли
    }
    
    return false; 
}

bool Library::returnBook(int id) {
    auto it = std::find_if(books_.begin(), books_.end(),
        [id](const Book& b) { return b.getId() == id; });

    if (it != books_.end() && it->isBorrowed()) {
        it->setBorrowed(false); 
        return true; // возврат
    }
    
    return false; //книги нет такой или уже вернули
}



