#pragma once
#include <QMainWindow>
#include <optional>

#include "core/Board.h"
#include "core/Solver.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonSolve_clicked();
    void on_pushButtonBrowse_clicked();
    void on_pushButtonImport_clicked();
    void onSolveFinished(const Board &result, long long iteration, bool solved);

private:
    Ui::MainWindow *ui;

    std::optional<Board> currentBoard;

    void renderBoard();
    void updateStatus(long long iteration, bool solved);
};
