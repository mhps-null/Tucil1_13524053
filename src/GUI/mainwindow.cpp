#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QColor>
#include <QThread>

#include <fstream>
#include <vector>
#include <string>
#include <cctype>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::renderBoard()
{
    if (!currentBoard.has_value())
        return;

    const Board &board = *currentBoard;

    if (ui->boardWidget->layout() != nullptr)
    {
        QLayoutItem *item;
        while ((item = ui->boardWidget->layout()->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }
        delete ui->boardWidget->layout();
    }

    int n = board.getSize();

    const std::vector<std::vector<int>> &grid = board.getGrid();
    const std::vector<std::vector<int>> &color = board.getColor();

    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    for (int row = 0; row < n; ++row)
    {
        for (int col = 0; col < n; ++col)
        {
            QLabel *cell = new QLabel;
            cell->setFixedSize(40, 40);
            cell->setAlignment(Qt::AlignCenter);
            cell->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            int colorId = color[row][col];
            QColor cellColor = QColor::fromHsv((colorId * 40) % 360, 160, 220);

            cell->setStyleSheet(
                QString("background-color: %1;"
                        "border-right: 1px solid black;"
                        "border-bottom: 1px solid black;")
                    .arg(cellColor.name()));

            if (grid[row][col] == 1)
                cell->setText("♛");
            cell->setStyleSheet(
                QString("background-color: %1;"
                        "border-right: 1px solid black;"
                        "border-bottom: 1px solid black;"
                        "font-size: 24px;"
                        "font-weight: bold;"
                        "color: black;")
                    .arg(cellColor.name()));

            layout->addWidget(cell, row, col);
        }
    }

    ui->boardWidget->setLayout(layout);
}

void MainWindow::updateStatus(long long iteration, bool solved)
{
    ui->labelIteration->setText(QString::number(iteration));

    ui->labelState->setText(
        solved ? "Solved" : "No solution");
}

void MainWindow::on_pushButtonSolve_clicked()
{
    solverActive = true;

    if (!currentBoard.has_value())
        return;

    if (solverThread != nullptr)
        return;

    int interval = ui->spinBoxInterval->value();
    bool effMode = ui->checkBoxEff->isChecked();

    solverThread = new QThread;
    solverWorker = new Solver(*currentBoard, interval, effMode);

    solverWorker->moveToThread(solverThread);

    connect(solverThread, &QThread::started,
            solverWorker, &Solver::solve);

    connect(solverWorker, &Solver::finished,
            this, &MainWindow::onSolveFinished);

    connect(solverWorker, &Solver::progress,
            this, [this](long long iter)
            { if (!solverActive) return; ui->labelIteration->setText(QString::number(iter)); });

    connect(solverWorker, &Solver::finished,
            solverThread, &QThread::quit);

    connect(solverThread, &QThread::finished,
            solverWorker, &QObject::deleteLater);

    connect(solverThread, &QThread::finished,
            solverThread, &QObject::deleteLater);

    connect(solverThread, &QThread::finished,
            this, [this]()
            {
                solverThread = nullptr;
                solverWorker = nullptr; });

    solverThread->start();
}

void MainWindow::on_pushButtonReset_clicked()
{
    solverActive = false;

    if (solverWorker != nullptr)
        solverWorker->requestStop();

    if (solverThread != nullptr)
    {
        solverThread->quit();
        solverThread->wait();
    }

    currentBoard.reset();

    if (ui->boardWidget->layout() != nullptr)
    {
        QLayoutItem *item;
        while ((item = ui->boardWidget->layout()->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }
        delete ui->boardWidget->layout();
    }

    ui->labelIteration->setText("0");
    ui->labelState->setText("Idle");
    ui->lineEditPath->clear();
}

void MainWindow::on_pushButtonBrowse_clicked()
{
    QString defaultPath = QString(PROJECT_ROOT) + "/test";

    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select Test Case",
        defaultPath,
        "Text Files (*.txt);;All Files (*)");

    if (!fileName.isEmpty())
    {
        ui->lineEditPath->setText(fileName);
    }
}

void MainWindow::on_pushButtonImport_clicked()
{
    QString path = ui->lineEditPath->text();

    if (path.isEmpty())
    {
        ui->labelState->setText("No file selected");
        return;
    }

    std::ifstream file(path.toStdString());

    if (!file.is_open())
    {
        ui->labelState->setText("Failed to open file");
        return;
    }

    std::vector<std::vector<int>> color;
    std::string line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        std::vector<int> row;

        for (char c : line)
        {
            if (!isalpha(c))
            {
                ui->labelState->setText("Invalid character");
                return;
            }

            row.push_back(c - 'A');
        }

        color.push_back(row);
    }

    int n = color.size();

    if (n == 0)
    {
        ui->labelState->setText("Empty file");
        return;
    }

    for (const auto &row : color)
    {
        if ((int)row.size() != n)
        {
            ui->labelState->setText("File is not NxN");
            return;
        }
    }

    currentBoard = Board(n, color);

    renderBoard();
}

void MainWindow::onSolveFinished(const Board &result, long long iteration, bool solved)
{
    if (!solverActive)
        return;

    currentBoard = result;

    if (solved)
        renderBoard();

    updateStatus(iteration, solved);

    solverActive = false;
}
