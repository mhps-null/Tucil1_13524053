#include <iostream>
#include "core/Solver.h"

int main()
{
    std::vector<std::vector<int>> color;

    color =
        {
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4}};

    int n = color.size();

    Solver solver(color, false);

    solver.solve();

    std::cout << "Iterasi: " << solver.getIterationCount() << std::endl;

    const Board &result = solver.getBoard();
    const auto &grid = result.getGrid();

    std::cout << "Board hasil:\n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cout << grid[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}