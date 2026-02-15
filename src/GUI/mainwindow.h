#pragma once
#include <QMainWindow>
#include <QThread>
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
    void on_pushButtonReset_clicked();
    void onSolveFinished(const Board &result, long long iteration, bool solved, qint64 time);

private:
    Ui::MainWindow *ui;

    bool solverActive = false;
    std::optional<Board> currentBoard;
    QThread *solverThread = nullptr;
    Solver *solverWorker = nullptr;

    void renderBoard();
    void updateStatus(long long iteration, bool solved, qint64 time);
};
