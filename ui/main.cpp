#include <QApplication>
#include <cstring>
#include <iostream>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--cli") == 0) {
        std::cout << "The Lab Cli" << std::endl;
        return 0;
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
