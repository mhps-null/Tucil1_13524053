#pragma once
#include <vector>
#include <set>
using namespace std;

class Board
{
private:
    int n;
    vector<vector<int>> grid;
    vector<vector<int>> color;

public:
    Board(int n, const vector<vector<int>> &color);

    int countColorId() const;

    void placeQueen(int row, int col);
    void removeQueen(int row, int col);
    bool isValidWholeBoard() const;

    const vector<vector<int>> &getGrid() const;
    const vector<vector<int>> &getColor() const;
    int getSize() const;
};