#ifndef BOOK
#define BOOK

#include <string>
#include <iostream>

class Book {
public:
    
    Book(int id , const std::string& title, const std::string& author, int year ):id_(id),title_(title),author_(author),year_(year),borrow_(0){};//инициализация
    Book()=default;
    Book(const Book&)=default;
    ~Book()=default;

    
    int getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    int getYear() const;

    bool isBorrowed() const; //показывает, взята или нет
    void setBorrowed(bool borrow);//берем книгу
    
private:
    int id_=0;
    std::string title_;
    std::string author_;
    int year_;
    bool borrow_=0;
};

#endif