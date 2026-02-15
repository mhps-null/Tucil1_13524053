#pragma once
#include <vector>
#include <QString>

class FileParser
{
public:
    static std::vector<std::vector<char>> parseTxt(const QString &path);
};