#include "Board.h"

Board::Board(int n, const vector<vector<int>> &color)
    : n(n), grid(n, vector<int>(n, 0)), color(color) {}

void Board::placeQueen(int row, int col)
{
    grid[row][col] = 1;
}

void Board::removeQueen(int row, int col)
{
    grid[row][col] = 0;
}

bool Board::isValidWholeBoard() const
{
    // queen
    int qCount = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 1)
            {
                qCount++;
            }
        }
    }

    if (qCount != n)
    {
        return false;
    }

    // row
    for (int i = 0; i < n; i++)
    {
        int count = 0;
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 1)
            {
                count++;
                if (count > 1)
                {
                    return false;
                }
            }
        }

        if (count != 1)
        {
            return false;
        }
    }

    // column
    for (int j = 0; j < n; j++)
    {
        int count = 0;
        for (int i = 0; i < n; i++)
        {
            if (grid[i][j] == 1)
            {
                count++;
                if (count > 1)
                {
                    return false;
                }
            }
        }

        if (count != 1)
        {
            return false;
        }
    }

    // adjacent diagonally
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 1)
            {
                if (i < n - 1 && j < n - 1 && grid[i + 1][j + 1] == 1)
                {
                    return false;
                }
                if (i < n - 1 && j > 0 && grid[i + 1][j - 1] == 1)
                {
                    return false;
                }
            }
        }
    }

    // color
    int maxColorId = countColorId();
    vector<int> countColor(maxColorId, 0);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 1)
            {
                int c = color[i][j];
                countColor[c]++;

                if (countColor[c] != 1)
                {
                    return false;
                }
            }
        }
    }

    for (int c = 0; c < maxColorId; c++)
    {
        if (countColor[c] != 1)
        {
            return false;
        }
    }

    return true;
}

bool Board::isValidPos(int row, int col) const // tentatif
{
    // size
    if (row > n - 1 || row < 0 || col > n - 1 || col < 0)
    {
        return false;
    }

    // row
    for (int i = 0; i < n; i++)
    {
        if (grid[row][i] == 1)
        {
            return false;
        }
    }

    // column
    for (int i = 0; i < n; i++)
    {
        if (grid[i][col] == 1)
        {
            return false;
        }
    }

    // adjacent diagonally
    if (row < n - 1 && col < n - 1 && grid[row + 1][col + 1] == 1)
    {
        return false;
    }
    if (row < n - 1 && col > 0 && grid[row + 1][col - 1] == 1)
    {
        return false;
    }
    if (row > 0 && col > 0 && grid[row - 1][col - 1] == 1)
    {
        return false;
    }
    if (row > 0 && col < n - 1 && grid[row - 1][col + 1] == 1)
    {
        return false;
    }

    // color
    int cId = color[row][col];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (color[i][j] == cId && grid[i][j] == 1)
            {
                return false;
            }
        }
    }

    return true;
}

int Board::countColorId() const
{
    set<int> colorId;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            colorId.insert(color[i][j]);
        }
    }

    return colorId.size();
}

// getter grid
const vector<vector<int>> &Board::getGrid() const
{
    return grid;
}

// getter size
int Board::getSize() const
{
    return n;
}