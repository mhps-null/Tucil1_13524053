#pragma once
#include <QString>
#include "core/Board.h"

class FileManager
{
public:
    static bool save(const Board &board, const QString &path);
    static bool load(const QString &path, std::vector<std::vector<int>> &color, QString &error);
};