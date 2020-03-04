#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QPaintDevice>
#include <QPainter>


class ScreenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenWidget(QWidget *parent = 0);
    
    void setScreenImageSize(int w, int h, QImage::Format type);

    inline QImage & screenImage() {
        return screen;
    }


protected:
    void paintEvent(QPaintEvent *event);

private:
    QImage screen;
};

#endif // SCREENWIDGET_H
