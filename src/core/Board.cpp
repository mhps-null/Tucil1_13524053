#include "Board.h"

Board::Board(int n, const vector<vector<int>> &color) : n(n), grid(n, vector<int>(n, 0)), color(color) {}

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
    // baris
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

    // kolom
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

    // tetangga diagonal
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
                else if (i < n - 1 && j > 0 && grid[i + 1][j - 1] == 1)
                {
                    return false;
                }
                else if (i > 0 && j > 0 && grid[i - 1][j - 1] == 1)
                {
                    return false;
                }
                else if (i > 0 && j < n - 1 && grid[i - 1][j + 1] == 1)
                {
                    return false;
                }
            }
        }
    }

    // warna
    int maxColorId = n * n;
    vector<int> countColor(maxColorId + 1, 0);

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

    return true;
}

// selektor grid
const vector<vector<int>> &Board::getGrid() const
{
    return grid;
}

// selektor size
int Board::getSize() const
{
    return n;
}