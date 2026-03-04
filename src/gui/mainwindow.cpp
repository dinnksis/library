#include "mainwindow.hpp"
#include <QHeaderView>
#include <QMessageBox>
#include <QPalette>
#include <QFont>
#include <QBrush>
#include <QColor>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), lib("library.db") {
    setWindowTitle("Библиотека");
    resize(1000, 700);
    
    // Устанавливаем стиль для всего приложения
    setStyleSheet(
        "QMainWindow {"
        "   background-color: #f5f5f5;"
        "}"
        "QPushButton {"
        "   background-color: #4a6fa5;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 5px;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3a5a80;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2b4560;"
        "}"
        "QLineEdit {"
        "   padding: 8px;"
        "   border: 2px solid #ddd;"
        "   border-radius: 5px;"
        "   font-size: 12px;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #4a6fa5;"
        "}"
        "QTableWidget {"
        "   gridline-color: #e0e0e0;"
        "   background-color: white;"
        "   selection-background-color: #4a6fa5;"
        "   selection-color: white;"
        "}"
        "QTableWidget::item {"
        "   padding: 8px;"
        "}"
        "QHeaderView::section {"
        "   background-color: #4a6fa5;"
        "   color: white;"
        "   padding: 8px;"
        "   border: none;"
        "   font-weight: bold;"
        "}"
        "QLabel {"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   color: #333;"
        "}"
    );
    
    setupUI();
    refreshBookList();
    updateStats();
}

void MainWindow::setupUI() {
    // Центральный виджет с отступами
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Заголовок
    QLabel* titleLabel = new QLabel("Каталог книг", this);
    titleLabel->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: #2c3e50;"
        "padding: 10px;"
        "background-color: white;"
        "border-radius: 10px;"
    );
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Статистика
    QHBoxLayout* statsLayout = new QHBoxLayout();
    
    totalBooksLabel = new QLabel("Всего книг: 0", this);
    availableBooksLabel = new QLabel("Доступно: 0", this);
    borrowedBooksLabel = new QLabel("Выдано: 0", this);
    
    QString statsStyle = 
        "QLabel {"
        "   background-color: white;"
        "   padding: 15px;"
        "   border-radius: 8px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border: 1px solid #ddd;"
        "}";
    
    totalBooksLabel->setStyleSheet(statsStyle + "color: #2c3e50;");
    availableBooksLabel->setStyleSheet(statsStyle + "color: #27ae60;");
    borrowedBooksLabel->setStyleSheet(statsStyle + "color: #e74c3c;");
    
    statsLayout->addWidget(totalBooksLabel);
    statsLayout->addWidget(availableBooksLabel);
    statsLayout->addWidget(borrowedBooksLabel);
    mainLayout->addLayout(statsLayout);

    // Панель поиска
    QGroupBox* searchGroup = new QGroupBox("Поиск книг", this);
    searchGroup->setStyleSheet(
        "QGroupBox {"
        "   font-weight: bold;"
        "   border: 2px solid #4a6fa5;"
        "   border-radius: 8px;"
        "   margin-top: 10px;"
        "   padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 5px;"
        "}"
    );
    
    QGridLayout* searchLayout = new QGridLayout(searchGroup);
    
    // Поиск по автору
    QLabel* authorLabel = new QLabel("Автор:", this);
    authorLabel->setStyleSheet("font-weight: bold; color: #333;");
    authorSearch = new QLineEdit(this);
    authorSearch->setPlaceholderText("Введите автора");
    QPushButton* authorSearchBtn = new QPushButton("Найти", this);
    authorSearchBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #27ae60;"
        "}"
        "QPushButton:hover {"
        "   background-color: #219a52;"
        "}"
    );
    connect(authorSearchBtn, &QPushButton::clicked, this, &MainWindow::searchByAuthor);
    
    // Поиск по названию
    QLabel* titleSearchLabel = new QLabel("Название:", this);
    titleSearchLabel->setStyleSheet("font-weight: bold; color: #333;");
    titleSearch = new QLineEdit(this);
    titleSearch->setPlaceholderText("Введите название...");
    QPushButton* titleSearchBtn = new QPushButton("Найти", this);
    titleSearchBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #27ae60;"
        "}"
        "QPushButton:hover {"
        "   background-color: #219a52;"
        "}"
    );
    connect(titleSearchBtn, &QPushButton::clicked, this, &MainWindow::searchByTitle);
    
    // Кнопка показа всех
    QPushButton* showAllBtn = new QPushButton("Показать все", this);
    showAllBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #e67e22;"
        "   grid-column: span 2;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d35400;"
        "}"
    );
    connect(showAllBtn, &QPushButton::clicked, this, &MainWindow::showAllBooks);
    
    // Добавляем виджеты в сетку
    searchLayout->addWidget(authorLabel, 0, 0);
    searchLayout->addWidget(authorSearch, 0, 1);
    searchLayout->addWidget(authorSearchBtn, 0, 2);
    
    searchLayout->addWidget(titleSearchLabel, 1, 0);
    searchLayout->addWidget(titleSearch, 1, 1);
    searchLayout->addWidget(titleSearchBtn, 1, 2);
    
    searchLayout->addWidget(showAllBtn, 2, 0, 1, 3);
    
    searchLayout->setColumnStretch(1, 1);
    mainLayout->addWidget(searchGroup);

    // Таблица книг
    bookTable = new QTableWidget(this);
    bookTable->setColumnCount(5);
    bookTable->setHorizontalHeaderLabels({"ID", "Название", "Автор", "Год", "Статус"});
    bookTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bookTable->setSelectionMode(QAbstractItemView::SingleSelection);
    bookTable->setAlternatingRowColors(true);
    bookTable->setStyleSheet(
        "QTableWidget::item:alternate {"
        "   background-color: #f9f9f9;"
        "}"
    );
    mainLayout->addWidget(bookTable);

    // Панель действий
    QGroupBox* actionGroup = new QGroupBox("Действия с книгой", this);
    actionGroup->setStyleSheet(
        "QGroupBox {"
        "   font-weight: bold;"
        "   border: 2px solid #4a6fa5;"
        "   border-radius: 8px;"
        "   margin-top: 10px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 5px;"
        "}"
    );
    
    QHBoxLayout* actionLayout = new QHBoxLayout(actionGroup);
    
    borrowButton = new QPushButton("Взять книгу", this);
    borrowButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   font-size: 14px;"
        "   padding: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
    );
    
    returnButton = new QPushButton("Вернуть книгу", this);
    returnButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #e67e22;"
        "   font-size: 14px;"
        "   padding: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d35400;"
        "}"
    );
    
    connect(borrowButton, &QPushButton::clicked, this, &MainWindow::borrowSelectedBook);
    connect(returnButton, &QPushButton::clicked, this, &MainWindow::returnSelectedBook);
    
    actionLayout->addWidget(borrowButton);
    actionLayout->addWidget(returnButton);
    mainLayout->addWidget(actionGroup);
}

void MainWindow::updateBookList(const std::vector<Book>& books) {
    bookTable->setRowCount(0);
    
    for (const auto& book : books) {
        int row = bookTable->rowCount();
        bookTable->insertRow(row);
        
        // ID
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(book.getId()));
        idItem->setTextAlignment(Qt::AlignCenter);
        bookTable->setItem(row, 0, idItem);
        
        // Название
        QTableWidgetItem* titleItem = new QTableWidgetItem(QString::fromStdString(book.getTitle()));
        titleItem->setFont(QFont("Arial", 10, QFont::Bold));
        bookTable->setItem(row, 1, titleItem);
        
        // Автор
        bookTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(book.getAuthor())));
        
        // Год
        QTableWidgetItem* yearItem = new QTableWidgetItem(QString::number(book.getYear()));
        yearItem->setTextAlignment(Qt::AlignCenter);
        bookTable->setItem(row, 3, yearItem);
        
        
        QTableWidgetItem* statusItem = new QTableWidgetItem(book.isBorrowed() ? "Взята" : "Доступна");
        statusItem->setTextAlignment(Qt::AlignCenter);
        
        if (book.isBorrowed()) {
            statusItem->setForeground(QBrush(QColor("#e74c3c")));
            statusItem->setFont(QFont("Arial", 10, QFont::Bold));
        } else {
            statusItem->setForeground(QBrush(QColor("#27ae60")));
            statusItem->setFont(QFont("Arial", 10, QFont::Bold));
        }
        
        bookTable->setItem(row, 4, statusItem);
    }
    
    updateStats();
}

void MainWindow::updateStats() {
    size_t total = lib.getBookCount();
    size_t available = lib.getAvailableCount();
    size_t borrowed = total - available;
    
    totalBooksLabel->setText(QString("Всего книг: %1").arg(total));
    availableBooksLabel->setText(QString("Доступно: %1").arg(available));
    borrowedBooksLabel->setText(QString("Выдано: %1").arg(borrowed));
}

void MainWindow::refreshBookList() {
    updateBookList(lib.getAllBooks());
}

void MainWindow::searchByAuthor() {
    QString author = authorSearch->text().trimmed();
    if (!author.isEmpty()) {
        auto results = lib.findBooksByAuthor(author.toStdString());
        if (results.empty()) {
            QMessageBox::information(this, "Результаты поиска", 
                QString("Книг автора '%1' не найдено").arg(author));
        }
        updateBookList(results);
    }
}

void MainWindow::searchByTitle() {
    QString title = titleSearch->text().trimmed();
    if (!title.isEmpty()) {
        auto results = lib.findBooksByTitle(title.toStdString());
        if (results.empty()) {
            QMessageBox::information(this, "Результаты поиска", 
                QString("Книг с названием '%1' не найдено").arg(title));
        }
        updateBookList(results);
    }
}

void MainWindow::showAllBooks() {
    authorSearch->clear();
    titleSearch->clear();
    refreshBookList();
}

void MainWindow::borrowSelectedBook() {
    QList<QTableWidgetItem*> selectedItems = bookTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, выберите книгу из списка");
        return;
    }

    int row = selectedItems.first()->row();
    int id = bookTable->item(row, 0)->text().toInt();
    QString title = bookTable->item(row, 1)->text();
    
    if (lib.borrowBook(id)) {
        QMessageBox::information(this, "Успех", 
            QString("Книга '%1' успешно выдана").arg(title));
        refreshBookList();
    } else {
        QMessageBox::warning(this, "Ошибка", 
            QString("Не удалось выдать книгу '%1'. Возможно, она уже выдана").arg(title));
    }
}

void MainWindow::returnSelectedBook() {
    QList<QTableWidgetItem*> selectedItems = bookTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, выберите книгу из списка");
        return;
    }

    int row = selectedItems.first()->row();
    int id = bookTable->item(row, 0)->text().toInt();
    QString title = bookTable->item(row, 1)->text();
    
    if (lib.returnBook(id)) {
        QMessageBox::information(this, "Успех", 
            QString("Книга '%1' успешно возвращена").arg(title));
        refreshBookList();
    } else {
        QMessageBox::warning(this, "Ошибка", 
            QString("Не удалось вернуть книгу '%1'. Возможно, она не была выдана").arg(title));
    }
}