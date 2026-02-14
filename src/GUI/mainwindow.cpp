#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include "core/Solver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// void MainWindow::renderBoard(const std::vector<std::vector<int>> &grid)
// {
//     if (ui->boardWidget->layout() != nullptr)
//     {
//         QLayoutItem *item;
//         while ((item = ui->boardWidget->layout()->takeAt(0)) != nullptr)
//         {
//             delete item->widget();
//             delete item;
//         }
//         delete ui->boardWidget->layout();
//     }

//     int n = grid.size();
//     QGridLayout *layout = new QGridLayout;
//     layout->setSpacing(0);

//     for (int row = 0; row < n; ++row)
//     {
//         for (int col = 0; col < n; ++col)
//         {

//             QLabel *cell = new QLabel;
//             cell->setFixedSize(40, 40);
//             cell->setAlignment(Qt::AlignCenter);

//             if ((row + col) % 2 == 0)
//                 cell->setStyleSheet("background-color: #EEE;");
//             else
//                 cell->setStyleSheet("background-color: #555; color: white;");

//             if (grid[row][col] == 1)
//                 cell->setText("♛");

//             layout->addWidget(cell, row, col);
//         }
//     }

//     ui->boardWidget->setLayout(layout);
// }

// void MainWindow::on_pushButton_solve_clicked()
// {
//     int n = ui->spinBoxN->value();

//     Solver solver(n);
//     solver.solve();

//     renderBoard(solver.getBoard().getGrid());
// }
