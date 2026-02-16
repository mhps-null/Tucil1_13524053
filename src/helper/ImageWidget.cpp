#include "ImageWidget.h"
#include <QPainter>
#include <QMouseEvent>

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
{
}

void ImageWidget::setImage(const QImage &img)
{
    image = img;
    update();
}

const QImage &ImageWidget::getImage() const
{
    return image;
}

QRect ImageWidget::getSelectedRect() const
{
    return selectedRect;
}

void ImageWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (!image.isNull())
    {
        painter.drawImage(rect(), image);
    }

    if (selecting || !selectedRect.isNull())
    {
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(QRect(startPoint, endPoint));
    }
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    selecting = true;
    startPoint = event->pos();
    endPoint = startPoint;
    update();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting)
    {
        endPoint = event->pos();
        update();
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    selecting = false;
    endPoint = event->pos();
    selectedRect = QRect(startPoint, endPoint).normalized();
    update();
}