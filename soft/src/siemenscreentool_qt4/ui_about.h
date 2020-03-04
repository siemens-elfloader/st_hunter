/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created: Tue Sep 18 23:09:23 2012
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label;
    QPushButton *pushButton;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QString::fromUtf8("About"));
        About->resize(415, 181);
        gridLayout = new QGridLayout(About);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(About);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 3, 1, 1, 2);

        label_3 = new QLabel(About);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 1, 1, 2);

        label_5 = new QLabel(About);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 6, 1, 1, 2);

        label = new QLabel(About);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 1, 1, 2);

        pushButton = new QPushButton(About);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 7, 3, 1, 1);

        label_4 = new QLabel(About);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 5, 1, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 3, 5, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 0, 5, 1);


        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QApplication::translate("About", "About", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("About", "<html><head/><body><p align=\"center\"><span style=\" font-size:10pt;\">1.0.0</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("About", "<html><head/><body><p align=\"center\"><span style=\" font-size:10pt;\">Tool for taking screenshots. </span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("About", "<html><head/><body><p align=\"center\"><span style=\" font-size:8pt;\">Copyright \302\251 2012 Kiril Zhumarin</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("About", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">Siemens Screen Tool</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("About", "\320\231\320\260 \320\277\320\276\320\275\321\217\320\273", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("About", "<html><head/><body><p align=\"center\"><a href=\"http://zhumarin.ru/siemens/sst/\"><span style=\" font-size:11pt; text-decoration: underline; color:#0057ae;\">\320\224\320\276\320\274\320\260\321\210\320\275\321\217\321\217 \321\201\321\202\321\200\320\260\320\275\320\270\321\206\320\260</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
