#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include "lib.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots: //методы обработки событий
    void searchByAuthor();
    void searchByTitle();
    void showAllBooks();
    void borrowSelectedBook();
    void returnSelectedBook();
    void updateBookList(const std::vector<Book>& books);

private:
    Library lib;
    QTableWidget* bookTable;
    QLineEdit* authorSearch;
    QLineEdit* titleSearch;
    QPushButton* borrowButton;
    QPushButton* returnButton;
    
    void setupUI(); //создание виджетов
    void refreshBookList();
};

#endif 