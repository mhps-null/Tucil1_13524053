#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show(); // ← WAJIB ADA

    return app.exec(); // ← WAJIB ADA
}