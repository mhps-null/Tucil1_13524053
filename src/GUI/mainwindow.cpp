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
#include <QMouseEvent>

#include <vector>
#include <cctype>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidgetColor->setMouseTracking(true);
    ui->tableWidgetColor->viewport()->installEventFilter(this);

    QString defaultPath = QString(PROJECT_ROOT) + "/test";
    ui->lineEditPath->setText(defaultPath);
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

    QGridLayout *layout =
        static_cast<QGridLayout *>(ui->boardWidget->layout());

    while (QLayoutItem *item = layout->takeAt(0))
    {
        delete item->widget();
        delete item;
    }

    int n = board.getSize();

    const std::vector<std::vector<int>> &grid = board.getGrid();
    const std::vector<std::vector<int>> &color = board.getColor();

    for (int row = 0; row < n; ++row)
    {
        for (int col = 0; col < n; ++col)
        {
            QLabel *cell = new QLabel;
            cell->setAlignment(Qt::AlignCenter);
            cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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
}

void MainWindow::updateStatus(long long iteration, bool solved, qint64 time)
{
    ui->labelIteration->setText(QString::number(iteration));

    ui->labelState->setText(
        solved ? "Solved" : "No solution");

    ui->labelTime->setText(QString::number(time) + " ms");
}

void MainWindow::on_pushButtonSolve_clicked()
{

    if (!currentBoard.has_value())
    {
        ui->labelState->setText("No board available");
        return;
    }

    if (ui->stackedWidget->currentWidget() != ui->boardWidget)
    {
        ui->labelState->setText("Build or import board first");
        return;
    }

    if (solverThread != nullptr)
        return;

    solverActive = true;
    bool effMode = ui->checkBoxEff->isChecked();

    solverThread = new QThread;
    solverWorker = new Solver(*currentBoard, effMode);

    solverWorker->moveToThread(solverThread);

    connect(solverThread, &QThread::started,
            solverWorker, &Solver::solve);

    connect(solverWorker, &Solver::finished,
            this, &MainWindow::onSolveFinished);

    connect(solverWorker, &Solver::boardUpdated,
            this,
            [this](const Board &snapshot, long long iter)
            {
                if (!solverActive)
                    return;

                currentBoard = snapshot;
                renderBoard();
                ui->labelIteration->setText(QString::number(iter));
            });

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

void MainWindow::on_pushButtonStop_clicked()
{
    solverActive = false;

    if (solverWorker != nullptr)
        solverWorker->requestStop();

    if (solverThread != nullptr)
    {
        solverThread->quit();
        solverThread->wait();
    }

    ui->labelIteration->setText("0");
    ui->labelState->setText("Idle");
    ui->labelTime->setText("-");
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

    std::vector<std::vector<int>> color;
    QString error;

    if (!FileManager::load(path, color, error))
    {
        ui->labelState->setText(error);
        return;
    }

    if (!validateBoard(color))
        return;

    int n = color.size();

    currentBoard = Board(n, color);
    renderBoard();
    ui->stackedWidget->setCurrentWidget(ui->boardWidget);
}

void MainWindow::on_pushButtonImportImage_clicked()
{
    QString path = QFileDialog::getOpenFileName(
        this,
        "Open Image",
        "",
        "Images (*.png *.jpg *.bmp)");

    if (path.isEmpty())
        return;

    QImage img(path);

    if (img.isNull())
        return;

    ui->imageWidget->setImage(img);
    ui->stackedWidget->setCurrentWidget(ui->imagePage);
}

void MainWindow::on_pushButtonGenerate_clicked()
{
    int n = ui->spinBoxN->value();

    ui->tableWidgetColor->clear();
    ui->tableWidgetColor->setRowCount(n);
    ui->tableWidgetColor->setColumnCount(n);

    ui->tableWidgetColor->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetColor->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetColor->setItem(i, j, item);
        }
    }

    ui->stackedWidget->setCurrentWidget(ui->inputPage);
}

void MainWindow::on_pushButtonBuildBoard_clicked()
{
    int n = ui->tableWidgetColor->rowCount();
    if (n == 0)
        return;

    std::vector<std::vector<int>> color(n, std::vector<int>(n));

    QMap<QString, int> colorMap;
    int nextId = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            QTableWidgetItem *item = ui->tableWidgetColor->item(i, j);

            if (!item)
            {
                ui->labelState->setText("Incomplete input");
                return;
            }

            QVariant data = item->data(Qt::UserRole);

            if (!data.isValid())
            {
                ui->labelState->setText("All cells must be colored");
                return;
            }

            QColor cellColor = data.value<QColor>();
            QString key = cellColor.name();

            if (!colorMap.contains(key))
            {
                colorMap[key] = nextId++;
            }

            color[i][j] = colorMap[key];
        }
    }

    if (!validateBoard(color))
        return;

    currentBoard = Board(n, color);
    renderBoard();
    ui->stackedWidget->setCurrentWidget(ui->boardWidget);
    ui->labelState->setText("Board created from painted colors");
}

void MainWindow::on_pushButtonChangeColor_clicked()
{
    int hue = QRandomGenerator::global()->bounded(360);
    currentPaintColor = QColor::fromHsv(hue, 160, 220);
}

void MainWindow::on_pushButtonSave_clicked()
{
    if (!currentBoard.has_value())
    {
        ui->labelState->setText("No board to save");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Board",
        "",
        "Text Files (*.txt)");

    if (fileName.isEmpty())
        return;

    if (!FileManager::save(*currentBoard, fileName))
    {
        ui->labelState->setText("Failed to save file");
        return;
    }

    ui->labelState->setText("Board saved successfully");
}

void MainWindow::onSolveFinished(const Board &result, long long iteration, bool solved, qint64 time)
{
    if (!solverActive)
        return;

    currentBoard = result;

    renderBoard();

    updateStatus(iteration, solved, time);

    solverActive = false;
}

bool MainWindow::validateBoard(const std::vector<std::vector<int>> &color)
{
    int n = color.size();

    if (n == 0)
    {
        ui->labelState->setText("Board size invalid");
        return false;
    }

    for (const auto &row : color)
    {
        if ((int)row.size() != n)
        {
            ui->labelState->setText("Board must be NxN");
            return false;
        }
    }

    std::set<int> uniqueColors;

    for (const auto &row : color)
        for (int c : row)
            uniqueColors.insert(c);

    if ((int)uniqueColors.size() != n)
    {
        ui->labelState->setText("Number of colors must be exactly N");
        return false;
    }

    std::vector<int> count(n, 0);

    for (const auto &row : color)
        for (int c : row)
            count[c]++;

    for (int i = 0; i < n; i++)
    {
        if (count[i] == 0)
        {
            ui->labelState->setText("Each color must appear at least once");
            return false;
        }
    }

    return true;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tableWidgetColor->viewport())
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            mousePressed = true;

            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            QModelIndex index =
                ui->tableWidgetColor->indexAt(mouseEvent->pos());

            if (index.isValid())
            {
                QTableWidgetItem *item =
                    ui->tableWidgetColor->item(index.row(), index.column());

                if (!item)
                {
                    item = new QTableWidgetItem;
                    ui->tableWidgetColor->setItem(index.row(), index.column(), item);
                }

                item->setBackground(currentPaintColor);
                item->setData(Qt::UserRole, currentPaintColor);
            }
        }

        if (event->type() == QEvent::MouseMove && mousePressed)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            QModelIndex index =
                ui->tableWidgetColor->indexAt(mouseEvent->pos());

            if (index.isValid())
            {
                QTableWidgetItem *item =
                    ui->tableWidgetColor->item(index.row(), index.column());

                if (!item)
                {
                    item = new QTableWidgetItem;
                    ui->tableWidgetColor->setItem(index.row(), index.column(), item);
                }

                item->setBackground(currentPaintColor);
                item->setData(Qt::UserRole, currentPaintColor);
            }
        }

        if (event->type() == QEvent::MouseButtonRelease)
            mousePressed = false;
    }

    return QMainWindow::eventFilter(obj, event);
}
