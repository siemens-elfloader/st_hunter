#include "mainwindow.h"
#include "ui_mainwindow.h"

static const int speed_list[7] = {
    B57600,
    B115200,
    B230400,
    B460800,
    B500000,
    B576000,
    B921600
};

static const char *speed_list_names[7] = {
    "57600",
    "115200",
    "230400",
    "460800",
    "500000",
    "576000",
    "921600"
};



static inline uint32_t rgb16ToRgb32(uint16_t c) {
    return 0xff000000
        | ((((c) << 3) & 0xf8) | (((c) >> 2) & 0x7))
        | ((((c) << 5) & 0xfc00) | (((c) >> 1) & 0x300))
        | ((((c) << 8) & 0xf80000) | (((c) << 3) & 0x70000));
}


QSettings *MainWindow::global_settings = 0;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings("Siemens Hackers inc.", "SiemensScreenTool", this)
{
    global_settings = &settings;
    ui->setupUi(this);

    ui->action->setIcon(QIcon::fromTheme("document-save"));
    ui->action_2->setIcon(QIcon::fromTheme("application-exit"));
    ui->action_3->setIcon(QIcon::fromTheme("help-about"));

    settings.setParent(this);
    status_label.setText("idle");
    light_label.setText(QString("<img src=\":/images/res/%1.png\"/>").arg("red"));

    ui->statusBar->addWidget(&light_label);
    ui->statusBar->addWidget(&status_label);

    ui->progressBar->setVisible(false);

    connect(&connection, SIGNAL(recivedLine(int, int, const QByteArray &)), this, SLOT(recivedLine(int, int, const QByteArray &)));

    connect(&connection, SIGNAL(recivedSrceenInfo(BFC_DisplayInfo*,BFC_DisplayBufferInfo*)),
            this, SLOT(recivedSrceenInfo(BFC_DisplayInfo*,BFC_DisplayBufferInfo*)));

    connect(&connection, SIGNAL(shootingFinished(int)), this, SLOT(shootingFinished(int)));

    connect(&connection, SIGNAL(state(const QString &)), this, SLOT(state(const QString &)));

    setupPortFeatures();
}


MainWindow::~MainWindow()
{
    settings.setValue("port", ui->devComboBox->currentText());
    settings.setValue("speed", ui->speedComboBox->currentText());

    delete ui;
    global_settings = 0;
}


void MainWindow::setupPortFeatures()
{
    ui->devComboBox->clear();

    QVector<QString> vec = {"/dev/ttyUSB%1", "/dev/ttyS%1"};


    for(const QString & s : vec)
    {
        for(int i = 0; i<26; ++i) {

            QString dev = s.arg(i);
            if( QFile::exists(dev) ) {
                ui->devComboBox->addItem(dev);
            }
        }
    }

    if(settings.contains("port")) {
        int i = ui->devComboBox->findText(settings.value("port").toString());
        if(i > -1)
            ui->devComboBox->setCurrentIndex(i);
    }


    ui->speedComboBox->clear();
    for(const char *s : speed_list_names) {
        ui->speedComboBox->addItem(s);
    }

    if(settings.contains("speed")) {
        int i = ui->speedComboBox->findText((QString)settings.value("speed").toString());
        if(i > -1)
            ui->speedComboBox->setCurrentIndex(i);
    }
}


void MainWindow::state(const QString & s)
{
    if( s.isEmpty() )
        status_label.setText("idle");
    else
        status_label.setText(s);
}

void MainWindow::recivedLine(int width, int nline, const QByteArray & line)
{
    //qWarning() << __FUNCTION__ << width << nline;
    QImage & img = ui->screenWidget->screenImage();

    int cp_bytes = 2;
    switch(connection.dpyBufferInfo()->type)
    {
    case RGB565_MODE:
        cp_bytes = 2;
        break;

    case RGB888_MODE:
        cp_bytes = 4;
        break;
    }

    qMemCopy(img.scanLine(nline), line.data(), width * cp_bytes);

    ui->progressBar->setValue(nline);
    ui->screenWidget->repaint();
}


void MainWindow::recivedSrceenInfo(BFC_DisplayInfo *dpy_info, BFC_DisplayBufferInfo *dpy_buf_info)
{
    qDebug() << "BFC_DisplayInfo dump:\n";
    qDebug("\tdpy_info->subcmd    = 0x%02X\n\
            \tdpy_info->width     = %d\n\
            \tdpy_info->height    = %d\n\
            \tdpy_info->client_id = 0x%02X\n",
              dpy_info -> subcmd,
              dpy_info -> width,
              dpy_info -> height,
              dpy_info -> client_id);


    qDebug() << "BFC_DisplayBufferInfo dump:\n";
    qDebug("\tdpy_buf_info->subcmd    = 0x%02X\n\
    \tdpy_buf_info->client_id = 0x%02X\n\
    \tdpy_buf_info->width     = %d\n\
    \tdpy_buf_info->height    = %d\n\
    \tdpy_buf_info->offet_x   = %d\n\
    \tdpy_buf_info->offet_y   = %d\n\
    \tdpy_buf_info->address   = 0x%X\n\
    \tdpy_buf_info->type      = 0x%02X\n",
            dpy_buf_info -> subcmd,
            dpy_buf_info -> client_id,
            dpy_buf_info -> width,
            dpy_buf_info -> height,
            dpy_buf_info -> offet_x,
            dpy_buf_info -> offet_y,
            dpy_buf_info -> address,
            dpy_buf_info -> type
    );

    QImage::Format fmt = QImage::Format_RGB16;
    switch(dpy_buf_info->type)
    {
    case RGB565_MODE:
        fmt = QImage::Format_RGB16;
        break;

    case RGB888_MODE:
        fmt = QImage::Format_RGB32;
        break;
    }

    qWarning() << "format: " << fmt;

    ui->screenWidget->setScreenImageSize(dpy_info->width, dpy_info->height, fmt);
    ui->progressBar->setMaximum(dpy_info->height -1);

    qWarning() << "pixel size: " << ui->screenWidget->screenImage().bitPlaneCount()/8;
}


void MainWindow::shootingFinished(int code)
{
    qWarning() << __FUNCTION__ << code;
    ui->progressBar->setVisible(false);

    if(code) {
        QMessageBox::critical(this, "Error", connection.err_text[code]);
    }
}



uint MainWindow::getSpeed()
{
    QString speed = ui->speedComboBox->currentText();

    int i = 0;
    for(const char *s : speed_list_names)
    {
        if(speed == s)
            return speed_list[i];

        ++i;
    }

    return 0;
}


void MainWindow::on_openButton_clicked()
{
    if( connection.isWorking() )
        return;

    try {
        connection.setDev(ui->devComboBox->currentText());
        connection.setSpeed(getSpeed());
        connection.open();
    } catch (ComPortException & e) {
        connection.close();
        QMessageBox::critical(this, "Error!", e.what());
        return;
    }

    qWarning() << "Port opened";

    light_label.setText(QString("<img src=\":/images/res/%1.png\"/>").arg("green"));
}


void MainWindow::on_closeButton_clicked()
{
    connection.close();
    light_label.setText(QString("<img src=\":/images/res/%1.png\"/>").arg("red"));
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
    connection.screenShoot();
}


// save
void MainWindow::on_action_triggered()
{
    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Save ScreenShoot"),
            QDir::currentPath(),
            tr("Png files (*.png);;Jpeg files (*.jpg);;Bmp files (*.bmp);;All files (*.*)") );

    if(filename.isEmpty())
        return;

    if(ui->screenWidget->screenImage().save(filename)) {
        QMessageBox::information(this, "Info", "Saved");
    } else
        QMessageBox::critical(this, "Error", "Can`t save!");
}


void MainWindow::on_action_2_triggered()
{
    close();
}


void MainWindow::on_action_3_triggered()
{
    (new About(this))->show();
}
