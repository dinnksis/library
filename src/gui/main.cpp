#include "lib.hpp"
#include "mainwindow.hpp"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {

    try {
        QApplication app(argc, argv); //создание qt приложения
        MainWindow window; 
        window.show();
        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "ошибка: " << e.what() << std::endl;
        return 1;
    }
}