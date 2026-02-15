#pragma once
#include <algorithm>
#include <QElapsedTimer>
#include <QObject>
#include "Board.h"

class Solver : public QObject
{
    Q_OBJECT
private:
    Board board;
    QElapsedTimer frameTimer;

    long long iterationCount = 0;

    std::atomic<bool> stopRequested{false};

    bool solutionFound = false;
    bool efficientMode = false;

public:
    explicit Solver(const Board &board, bool efficientMode);

    const Board &getBoard() const;
    long long getIterationCount() const;
    bool getSolFound() const;
    void recordIteration();

    void requestStop() { stopRequested = true; }

public slots:
    void solve();

signals:
    void boardUpdated(const Board &snapshot, long long iteration);
    void finished(const Board result, long long iterationCount, bool solutionFound, qint64 time);
    void progress(long long iteration);
};