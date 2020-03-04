#ifndef COMCONNECTION_H
#define COMCONNECTION_H



#include <QString>
#include <QByteArray>
#include <QImage>
#include <QThread>

#include "comport.h"
#include "bfc.h"


#define RGB565_MODE 0x04
#define RGB888_MODE 0x05

class ComConnection : public QThread
{
    Q_OBJECT

public:

    static QString err_text[6];

    enum {
        ERR_NONE = 0,
        CANT_SEND_AT_PING,
        CANT_ENTER_BFC_MODE,
        CANT_SEND_BFC_PING,
        CANT_READ_DPY_INFO,
        CANT_READ_DPY_BUF_INFO
    };

public:
    ComConnection();
    ~ComConnection();

    inline ComConnection & setDev(const QString & _dev) {
        port.setDevice(_dev.toLocal8Bit().constData());
        return *this;
    }

    inline ComConnection & setSpeed(int _speed) {
        port.setSpeed(_speed);
        return *this;
    }

    inline ComConnection & setTimeout(uint timeout) {
        port.setTimeout(timeout);
        return *this;
    }

    inline ComConnection & open() {
        port.open();
        return *this;
    }

    inline ComConnection & setMultiplierTimeout(uint timeout) {
        port.setMultiplierTimeout(timeout);
        return *this;
    }

    inline ComConnection & close() {
        connected = false;
        port.close();
        return *this;
    }

    inline BFC_DisplayBufferInfo *dpyBufferInfo() {
        return &disp_buf_info;
    }

    inline BFC_DisplayInfo *dpyInfo() {
        return &disp_info;
    }

    bool isWorking() const ;
    void run();

    void screenShoot();



signals:
    void shootingFinished(int code);
    void recivedSrceenInfo(BFC_DisplayInfo *dpy_info, BFC_DisplayBufferInfo *dpy_buf_info);
    void recivedLine(int width, int nline, const QByteArray &);
    void state(const QString &);

public:
    ComPort port;
    Bfc bfc;


private:
    int connectPhone();
    int displayInfo() ;

private:

    BFC_DisplayInfo disp_info;
    BFC_DisplayBufferInfo disp_buf_info;
    bool working, connected;
};

#endif // COMCONNECTION_H
