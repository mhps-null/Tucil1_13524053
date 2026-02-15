#include "Solver.h"

Solver::Solver(const Board &board, bool efficientMode)
    : board(board), efficientMode(efficientMode) {}

void Solver::solve()
{
    QElapsedTimer timer;
    timer.start();

    frameTimer.start();

    int n = board.getSize();

    if (!efficientMode)
    {
        int totalSize = n * n;
        while (true)
        {
            if (stopRequested)
                break;

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
            if (stopRequested)
                break;

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
    qint64 time = timer.elapsed();
    emit finished(board, iterationCount, solutionFound, time);
}

void Solver::recordIteration()
{
    iterationCount++;

    if (frameTimer.elapsed() >= 30)
    {
        emit boardUpdated(board, iterationCount);
        frameTimer.restart();
    }

    emit progress(iterationCount);
};

const Board &Solver::getBoard() const
{
    return board;
};

long long Solver::getIterationCount() const
{
    return iterationCount;
}

bool Solver::getSolFound() const
{
    return solutionFound;
}
