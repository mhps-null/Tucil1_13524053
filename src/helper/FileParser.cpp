#include "FileParser.h"
#include <fstream>
#include <string>
#include <cctype>
#include <stdexcept>

std::vector<std::vector<char>> FileParser::parseTxt(const QString &path)
{
    std::ifstream file(path.toStdString());

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file");
    }

    std::vector<std::vector<char>> grid;
    std::string line;

    while (std::getline(file, line))
    {

        if (line.empty())
            continue;

        std::vector<char> row;

        for (char c : line)
        {
            if (std::isalpha(c))
            {
                row.push_back(c);
            }
        }

        grid.push_back(row);
    }

    int n = grid.size();

    for (const auto &row : grid)
    {
        if (row.size() != n)
        {
            throw std::runtime_error("File not NxN");
        }
    }

    return grid;
}