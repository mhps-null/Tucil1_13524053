#include "Solver.h"

Solver::Solver(const vector<vector<int>> &color, bool efficientMode)
    : board(color.size(), color), efficientMode(efficientMode) {}

void Solver::solve()
{
    int n = board.getSize();
    int totalSize = n * n;

    if (!efficientMode)
    {
        while (true)
        {
            iterationCount++;

            if (iterationCount % historyInterval == 0)
            {
                history.push_back(board);
            }

            if (board.isValidWholeBoard())
            {
                solutionFound = true;
                return;
            }

            bool keepGoing = true;

            for (int k = totalSize - 1; k >= 0 && keepGoing; k--)
            {
                int row = k / n;
                int col = k % n;

                if (board.getGrid()[row][col] == 0)
                {
                    board.placeQueen(row, col);
                    keepGoing = false;
                }
                else
                {
                    board.removeQueen(row, col);
                }
            }

            if (keepGoing)
            {
                break;
            }
        }
    }
    else
    {
    }
}

const Board &Solver::getBoard() const
{
    return board;
};

const vector<Board> &Solver::getHistory() const
{
    return history;
};

long long Solver::getIterationCount() const
{
    return iterationCount;
}