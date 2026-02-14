#pragma once
#include "Board.h"

class Solver
{
private:
    Board board;
    vector<Board> history;

    long long iterationCount = 0;
    int historyInterval = 100;

    bool solutionFound = false;
    bool efficientMode = false;

public:
    Solver(const vector<vector<int>> &color, bool efficientMode);

    void solve();

    const Board &getBoard() const;
    const vector<Board> &getHistory() const;
    long long getIterationCount() const;
};