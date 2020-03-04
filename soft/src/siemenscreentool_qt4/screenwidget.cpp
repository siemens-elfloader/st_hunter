#include "screenwidget.h"
#include <QDebug>

ScreenWidget::ScreenWidget(QWidget *parent) :
    QWidget(parent),
    screen(132, 176, QImage::Format_RGB16)
{
    screen.fill(0xFF000000);

    QPainter p(&screen);


    /*p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(QColor(0xFF000000)));
    p.drawText(0, 0, screen.width(), screen.height(),
               Qt::TextWordWrap,
               trUtf8("Сперва откройте порт кнопкой open. "
                            "Затем нажмите \"Снять скриншот\", "
                            "чтобы получить изображение с экрана мобильного"));*/

}


void ScreenWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    this->QWidget::paintEvent(event);


    QPainter p(this);

    int center_x = this->width()/2 - screen.width()/2,
        center_y = this->height()/2 - screen.height()/2;

    p.drawImage(center_x, center_y, screen);

}


void ScreenWidget::setScreenImageSize(int w, int h, QImage::Format format)
{
    screen = QImage(w, h, format);
    screen.fill(0x00000000);

    this->setMaximumSize(w+2, h+2);
    this->resize(w, h);

}
