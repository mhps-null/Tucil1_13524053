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

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty())
            break;

        std::vector<int> row;

        for (char c : line)
        {
            if (!std::isalpha(static_cast<unsigned char>(c)))
            {
                error = "Invalid character";
                return false;
            }

            row.push_back(std::toupper(static_cast<unsigned char>(c)) - 'A');
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

bool FileManager::loadImage(const QString &path, std::vector<std::vector<int>> &color, QString &error)
{
    QImage image(path);

    if (image.isNull())
    {
        error = "Failed to load image";
        return false;
    }

    int width = image.width();
    int height = image.height();

    if (width != height)
    {
        error = "Image must be square";
        return false;
    }

    int n = 0;

    QColor prev = image.pixelColor(0, 0);
    for (int x = 1; x < width; x++)
    {
        QColor curr = image.pixelColor(x, 0);
        if (curr != prev)
        {
            n++;
            prev = curr;
        }
    }

    n++;

    if (n <= 0)
    {
        error = "Cannot detect grid size";
        return false;
    }

    int cellSize = width / n;

    if (cellSize * n != width)
    {
        error = "Image size not divisible by grid size";
        return false;
    }

    color.assign(n, std::vector<int>(n));

    QMap<QString, int> colorMap;
    int nextId = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int sampleX = j * cellSize + cellSize / 2;
            int sampleY = i * cellSize + cellSize / 2;

            QColor cellColor = image.pixelColor(sampleX, sampleY);
            QString key = cellColor.name();

            if (!colorMap.contains(key))
                colorMap[key] = nextId++;

            color[i][j] = colorMap[key];
        }
    }

    return true;
}