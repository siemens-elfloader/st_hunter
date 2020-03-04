#include <QDebug>
#include "comconnection.h"


QString ComConnection::err_text [6] = {
    "OK",
    "Can`t send AT ping",
    "Can`t enter to BFC mode",
    "Can`t send BFC ping",
    "Can`t read display info",
    "Can`t read display buffer info"
};


ComConnection::ComConnection() :
    working(false),
    connected(false)
{
    bfc.setComPort(&port);
}


ComConnection::~ComConnection()
{
    terminate();
}


bool ComConnection::isWorking() const {
    return working;
}


void ComConnection::screenShoot()
{
    start();
}


void ComConnection::run()
{
    working = true;
    if(!connected) {
        int err = connectPhone();
        if(err) {
            emit shootingFinished(err);
            emit state("");
            working = false;
            return;
        }
    }

    int err = displayInfo();
    if(err) {
        emit shootingFinished(err);
        emit state("");
        working = false;
        return;
    }

    emit recivedSrceenInfo(&disp_info, &disp_buf_info);
    emit state("Receiving ...");

    int sizeof_pixel = (disp_buf_info.type == RGB565_MODE? 2 : 4);
    int len;
    int line_size = disp_buf_info.width * sizeof_pixel;
    QByteArray src(line_size, 0);


    for (int y = 0; y < disp_buf_info.height; ++y) {

        len = bfc.readMem(disp_buf_info.address + y * line_size, src.data(), line_size);
        if (len == line_size) {
            emit recivedLine(disp_buf_info.width, y, src);
        }

    }

    emit state("Done.");
    working = false;

    emit shootingFinished(0);
}



int ComConnection::connectPhone()
{
    char buff[1024];
    setTimeout(300);
    setMultiplierTimeout(20);

    while (port.readChar() >= 0); // WTF?

    emit state("Connecting ...");

    if ( !bfc.auth(0x11) ) {

        memset(buff, 0, 11);
        port.write("ATE0\r", 5);
        if ( port.read(buff, 11) <= 0 || strstr(buff, "\r\nOK\r\n") == NULL) {
            qWarning() << buff;
            return CANT_SEND_AT_PING;
        }


        memset(buff, 0, 11);
        port.write("AT^SQWE=1\r", 10);
        if ( port.read(buff, 6) != 6 || strncmp(buff, "\r\nOK\r\n", 6) != 0) {
            qWarning() << buff;
            return CANT_ENTER_BFC_MODE;
        }

        usleep(300000);
        if (!bfc.auth(0x11)) {
            return CANT_SEND_BFC_PING;
        }
    } else {
        qWarning() << "Already in BFC mode";
    }

    connected = true;
    return 0;
}


int ComConnection::displayInfo()
{
    if ( !bfc.getDisplayInfo(&disp_info) ) {
        return CANT_READ_DPY_INFO;
    }

    if ( !bfc.getDisplayBufferInfo(&disp_buf_info, disp_info.client_id) ) {
        return CANT_READ_DPY_BUF_INFO;
    }

    return 0;
}
