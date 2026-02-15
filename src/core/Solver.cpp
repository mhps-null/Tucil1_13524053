#include "Solver.h"

Solver::Solver(const Board &board, int historyInterval, bool efficientMode)
    : board(board), historyInterval(historyInterval > 0 ? historyInterval : 1), efficientMode(efficientMode) {}

void Solver::solve()
{
    int n = board.getSize();

    if (!efficientMode)
    {
        int totalSize = n * n;
        while (true)
        {
            recordIteration();

            if (board.isValidWholeBoard())
            {
                solutionFound = true;
                break;
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
        vector<int> permutation(n);
        for (int i = 0; i < n; i++)
        {
            permutation[i] = i;
        }

        do
        {
            recordIteration();

            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    board.removeQueen(i, j);
                }
            }

            for (int row = 0; row < n; row++)
            {
                board.placeQueen(row, permutation[row]);
            }

            if (board.isValidWholeBoard())
            {
                solutionFound = true;
                break;
            }
        } while (next_permutation(permutation.begin(), permutation.end()));
    }
    emit finished(board, iterationCount, solutionFound);
}

void Solver::recordIteration()
{
    iterationCount++;

    if (iterationCount % historyInterval == 0 && historyInterval > 0)
    {
        history.push_back(board);
    }

    emit progress(iterationCount);
};

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

bool Solver::getSolFound() const
{
    return solutionFound;
}
