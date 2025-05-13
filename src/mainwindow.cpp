#include "mainwindow.hpp"
#include <QHeaderView>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), lib("library.db") {
    setupUI();
    refreshBookList();
}

void MainWindow::setupUI() {
    setWindowTitle("Библиотека");
    resize(800, 600);

    //центральный виджет
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    //поиск
    QHBoxLayout* searchLayout = new QHBoxLayout();
    
    authorSearch = new QLineEdit(this);
    authorSearch->setPlaceholderText("поиск по автору");
    QPushButton* authorSearchBtn = new QPushButton("найти по автору", this);
    connect(authorSearchBtn, &QPushButton::clicked, this, &MainWindow::searchByAuthor);
    
    titleSearch = new QLineEdit(this);
    titleSearch->setPlaceholderText("поиск по названию");
    QPushButton* titleSearchBtn = new QPushButton("найти по названию", this);
    connect(titleSearchBtn, &QPushButton::clicked, this, &MainWindow::searchByTitle);
    
    QPushButton* showAllBtn = new QPushButton("показать все", this);
    connect(showAllBtn, &QPushButton::clicked, this, &MainWindow::showAllBooks);

    searchLayout->addWidget(authorSearch);
    searchLayout->addWidget(authorSearchBtn);
    searchLayout->addWidget(titleSearch);
    searchLayout->addWidget(titleSearchBtn);
    searchLayout->addWidget(showAllBtn);
    mainLayout->addLayout(searchLayout);

    //таблица книг
    bookTable = new QTableWidget(this);
    bookTable->setColumnCount(5);
    bookTable->setHorizontalHeaderLabels({"ID", "Название", "Автор", "Год", "Статус"});
    bookTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bookTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(bookTable);

    //кнопки действий
    QHBoxLayout* actionLayout = new QHBoxLayout();
    borrowButton = new QPushButton("взять книгу", this);
    returnButton = new QPushButton("вернуть книгу", this);
    
    connect(borrowButton, &QPushButton::clicked, this, &MainWindow::borrowSelectedBook);
    connect(returnButton, &QPushButton::clicked, this, &MainWindow::returnSelectedBook);
    
    actionLayout->addWidget(borrowButton);
    actionLayout->addWidget(returnButton);
    mainLayout->addLayout(actionLayout);
}

void MainWindow::updateBookList(const std::vector<Book>& books) {
    bookTable->setRowCount(0);
    for (const auto& book : books) {
        int row = bookTable->rowCount();
        bookTable->insertRow(row);
        
        bookTable->setItem(row, 0, new QTableWidgetItem(QString::number(book.getId())));
        bookTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book.getTitle())));
        bookTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(book.getAuthor())));
        bookTable->setItem(row, 3, new QTableWidgetItem(QString::number(book.getYear())));
        bookTable->setItem(row, 4, new QTableWidgetItem(book.isBorrowed() ? "взята" : "доступна"));
    }
}

void MainWindow::refreshBookList() {
    updateBookList(lib.getAllBooks());
}

void MainWindow::searchByAuthor() {
    QString author = authorSearch->text();
    if (!author.isEmpty()) {
        updateBookList(lib.findBooksByAuthor(author.toStdString()));
    }
}

void MainWindow::searchByTitle() {
    QString title = titleSearch->text();
    if (!title.isEmpty()) {
        updateBookList(lib.findBooksByTitle(title.toStdString()));
    }
}

void MainWindow::showAllBooks() {
    refreshBookList();
}

void MainWindow::borrowSelectedBook() {
    QList<QTableWidgetItem*> selectedItems = bookTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "предупреждение", "выберите книгу");
        return;
    }

    int row = selectedItems.first()->row();
    int id = bookTable->item(row, 0)->text().toInt();
    
    if (lib.borrowBook(id)) {
        QMessageBox::information(this, "успех", "книга успешно взята");
        refreshBookList();
    } else {
        QMessageBox::warning(this, "ошибка", "не удалось взять книгу");
    }
}

void MainWindow::returnSelectedBook() {
    QList<QTableWidgetItem*> selectedItems = bookTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "предупреждение", "выберите книгу");
        return;
    }

    int row = selectedItems.first()->row();
    int id = bookTable->item(row, 0)->text().toInt();
    
    if (lib.returnBook(id)) {
        QMessageBox::information(this, "успех", "книга успешно возвращена");
        refreshBookList();
    } else {
        QMessageBox::warning(this, "ошибка", "не удалось вернуть книгу");
    }
} 