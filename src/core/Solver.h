#pragma once
#include "Board.h"

class Solver
{
private:
    Board board;
    long long allCount = 0;
    bool efficientMode = false;

public:
    Solver(const vector<vector<int>> &color, bool efficientMode);

    long long getAllCount() const;
    void solve();

    const Board &getBoard() const;
};