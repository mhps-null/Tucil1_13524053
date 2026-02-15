#pragma once
#include <algorithm>
#include <QObject>
#include "Board.h"

class Solver : public QObject
{
    Q_OBJECT
private:
    Board board;
    vector<Board> history;

    long long iterationCount = 0;
    int historyInterval = 100;

    bool solutionFound = false;
    bool efficientMode = false;

public:
    explicit Solver(const Board &board, int historyInterval, bool efficientMode);

    const Board &getBoard() const;
    const vector<Board> &getHistory() const;
    long long getIterationCount() const;
    bool getSolFound() const;
    void recordIteration();

public slots:
    void solve();

signals:
    void finished(const Board result, long long iterationCount, bool solutionFound);
    void progress(long long iteration);
};