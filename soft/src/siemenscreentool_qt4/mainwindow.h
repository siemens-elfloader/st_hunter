#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <QFile>
#include <QSettings>
#include <QFileDialog>
#include <stdint.h>
#include "comconnection.h"
#include "about.h"


namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    

private:
    void setupPortFeatures();
    uint getSpeed();

private slots:
    void on_openButton_clicked();
    void shootingFinished(int code);
    void recivedSrceenInfo(BFC_DisplayInfo *dpy_info, BFC_DisplayBufferInfo *dpy_buf_info);
    void recivedLine(int width, int nline, const QByteArray &);
    void state(const QString &);
    void on_closeButton_clicked();
    void on_pushButton_3_clicked();


    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

private:
    Ui::MainWindow *ui;
    QLabel status_label;
    QLabel light_label;
    ComConnection connection;

    QSettings settings;
    static QSettings *global_settings;
};

#endif // MAINWINDOW_H
