#pragma once
#include <QWidget>
#include <QImage>
#include <QRect>
#include <QPoint>

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);

    void setImage(const QImage &img);
    QRect getSelectedRect() const;
    const QImage &getImage() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QImage image;

    bool selecting = false;
    QPoint startPoint;
    QPoint endPoint;
    QRect selectedRect;
};