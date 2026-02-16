#pragma once
#include <QString>
#include <QImage>
#include <QMap>

#include "core/Board.h"
#include "helper/ImageWidget.h"

class FileManager
{
public:
    static bool save(const Board &board, const QString &path);
    static bool load(const QString &path, std::vector<std::vector<int>> &color, QString &error);
    static bool loadImage(const QString &path, std::vector<std::vector<int>> &color, QString &error);
};
