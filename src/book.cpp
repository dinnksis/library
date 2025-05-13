#include "book.hpp"


int Book::getId() const{
    return id_;
}
std::string Book::getTitle() const{
    return title_;
}
std::string Book::getAuthor() const{
    return author_;
}
int Book::getYear() const{
    return year_;
}

void Book::setBorrowed(bool borrow){
    Book::borrow_=borrow;
    if (borrow){
        std::cout<<"book is borrowed"<<std::endl;
    } else {
        std::cout<<"book returned"<<std::endl;
    }
    
}

bool Book::isBorrowed() const{
    return Book::borrow_;
}

