#pragma once
#include <vector>
using namespace std;

class Board
{
private:
    int n;
    vector<vector<int>> grid;
    vector<vector<int>> color;

public:
    Board(int n, const vector<vector<int>> &color);

    void placeQueen(int row, int col);
    void removeQueen(int row, int col);
    bool isValidWholeBoard() const;

    const vector<vector<int>> &getGrid() const;
    int getSize() const;
};