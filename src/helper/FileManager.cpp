#include "helper/FileManager.h"
#include <fstream>
#include <string>
#include <cctype>

bool FileManager::save(const Board &board, const QString &path)
{
    std::ofstream file(path.toStdString());
    if (!file.is_open())
        return false;

    const auto &color = board.getColor();
    const auto &grid = board.getGrid();
    int n = board.getSize();

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            char letter = 'A' + color[i][j];
            file << letter;
        }
        file << "\n";
    }

    file << "\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 1)
                file << "Q";
            else
                file << ".";
        }
        file << "\n";
    }

    file.close();
    return true;
}

bool FileManager::load(const QString &path, std::vector<std::vector<int>> &color, QString &error)
{
    std::ifstream file(path.toStdString());
    if (!file.is_open())
    {
        error = "Failed to open file";
        return false;
    }

    color.clear();
    std::string line;

    while (getline(file, line))
    {
        if (line.empty())
        {
            break;
        }

        std::vector<int> row;

        for (char c : line)
        {
            if (!isalpha(c))
            {
                error = "Invalid character";
                return false;
            }

            row.push_back(toupper(c) - 'A');
        }

        color.push_back(row);
    }

    int n = color.size();

    if (n == 0)
    {
        error = "Empty file";
        return false;
    }

    for (const auto &row : color)
    {
        if ((int)row.size() != n)
        {
            error = "Grid is not NxN";
            return false;
        }
    }

    return true;
}