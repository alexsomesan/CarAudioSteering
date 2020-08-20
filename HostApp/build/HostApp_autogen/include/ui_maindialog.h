/********************************************************************************
** Form generated from reading UI file 'maindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINDIALOG_H
#define UI_MAINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *speedLabel;
    QComboBox *portComboBox;
    QPushButton *connectPushButton;
    QLabel *portLabel;
    QComboBox *speedComboBox;
    QPushButton *disconnectPushButton;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QSlider *potValueSlider;
    QLCDNumber *lcdNumber;
    QCheckBox *ringCheckBox;
    QPushButton *potValuePushButton;
    QGroupBox *analogValue;
    QHBoxLayout *horizontalLayout_2;
    QLCDNumber *analogLabel;
    QPushButton *analogButton;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout;
    QTableView *buttonsView;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QLabel *statusLabel;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QString::fromUtf8("MainDialog"));
        MainDialog->resize(1079, 819);
        verticalLayout_2 = new QVBoxLayout(MainDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(MainDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMaximumSize(QSize(16777215, 100));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        speedLabel = new QLabel(groupBox);
        speedLabel->setObjectName(QString::fromUtf8("speedLabel"));
        speedLabel->setMinimumSize(QSize(150, 0));

        gridLayout->addWidget(speedLabel, 1, 0, 1, 1);

        portComboBox = new QComboBox(groupBox);
        portComboBox->setObjectName(QString::fromUtf8("portComboBox"));
        portComboBox->setMinimumSize(QSize(150, 0));

        gridLayout->addWidget(portComboBox, 0, 1, 1, 1);

        connectPushButton = new QPushButton(groupBox);
        connectPushButton->setObjectName(QString::fromUtf8("connectPushButton"));

        gridLayout->addWidget(connectPushButton, 0, 3, 1, 1);

        portLabel = new QLabel(groupBox);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));
        portLabel->setMinimumSize(QSize(150, 0));

        gridLayout->addWidget(portLabel, 0, 0, 1, 1);

        speedComboBox = new QComboBox(groupBox);
        speedComboBox->setObjectName(QString::fromUtf8("speedComboBox"));
        speedComboBox->setMinimumSize(QSize(150, 0));

        gridLayout->addWidget(speedComboBox, 1, 1, 1, 1);

        disconnectPushButton = new QPushButton(groupBox);
        disconnectPushButton->setObjectName(QString::fromUtf8("disconnectPushButton"));
        disconnectPushButton->setEnabled(false);

        gridLayout->addWidget(disconnectPushButton, 1, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 1, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(MainDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        groupBox_2->setMaximumSize(QSize(16777215, 65));
        horizontalLayout = new QHBoxLayout(groupBox_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        potValueSlider = new QSlider(groupBox_2);
        potValueSlider->setObjectName(QString::fromUtf8("potValueSlider"));
        potValueSlider->setMaximum(1023);
        potValueSlider->setOrientation(Qt::Horizontal);
        potValueSlider->setTickPosition(QSlider::TicksBelow);
        potValueSlider->setTickInterval(10);

        horizontalLayout->addWidget(potValueSlider);

        lcdNumber = new QLCDNumber(groupBox_2);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setFrameShadow(QFrame::Plain);
        lcdNumber->setDigitCount(4);
        lcdNumber->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout->addWidget(lcdNumber);

        ringCheckBox = new QCheckBox(groupBox_2);
        ringCheckBox->setObjectName(QString::fromUtf8("ringCheckBox"));

        horizontalLayout->addWidget(ringCheckBox);

        potValuePushButton = new QPushButton(groupBox_2);
        potValuePushButton->setObjectName(QString::fromUtf8("potValuePushButton"));

        horizontalLayout->addWidget(potValuePushButton);


        verticalLayout_2->addWidget(groupBox_2);

        analogValue = new QGroupBox(MainDialog);
        analogValue->setObjectName(QString::fromUtf8("analogValue"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(analogValue->sizePolicy().hasHeightForWidth());
        analogValue->setSizePolicy(sizePolicy1);
        analogValue->setMinimumSize(QSize(0, 65));
        analogValue->setLayoutDirection(Qt::LeftToRight);
        horizontalLayout_2 = new QHBoxLayout(analogValue);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        analogLabel = new QLCDNumber(analogValue);
        analogLabel->setObjectName(QString::fromUtf8("analogLabel"));
        analogLabel->setDigitCount(4);
        analogLabel->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_2->addWidget(analogLabel);

        analogButton = new QPushButton(analogValue);
        analogButton->setObjectName(QString::fromUtf8("analogButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(analogButton->sizePolicy().hasHeightForWidth());
        analogButton->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(analogButton);


        verticalLayout_2->addWidget(analogValue);

        groupBox_3 = new QGroupBox(MainDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout = new QVBoxLayout(groupBox_3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonsView = new QTableView(groupBox_3);
        buttonsView->setObjectName(QString::fromUtf8("buttonsView"));

        verticalLayout->addWidget(buttonsView);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButton_2 = new QPushButton(groupBox_3);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_3->addWidget(pushButton_2);

        pushButton = new QPushButton(groupBox_3);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_3->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(groupBox_3);

        statusLabel = new QLabel(MainDialog);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        sizePolicy1.setHeightForWidth(statusLabel->sizePolicy().hasHeightForWidth());
        statusLabel->setSizePolicy(sizePolicy1);
        statusLabel->setMinimumSize(QSize(0, 25));
        statusLabel->setFocusPolicy(Qt::NoFocus);
        statusLabel->setAcceptDrops(false);
        statusLabel->setFrameShape(QFrame::Panel);
        statusLabel->setFrameShadow(QFrame::Sunken);
        statusLabel->setScaledContents(false);

        verticalLayout_2->addWidget(statusLabel);


        retranslateUi(MainDialog);
        QObject::connect(connectPushButton, SIGNAL(clicked()), MainDialog, SLOT(slotConnectSerial()));
        QObject::connect(disconnectPushButton, SIGNAL(clicked()), MainDialog, SLOT(slotDisconnectSerial()));
        QObject::connect(portComboBox, SIGNAL(currentTextChanged(QString)), MainDialog, SLOT(slotPortChanged(QString)));
        QObject::connect(potValueSlider, SIGNAL(valueChanged(int)), lcdNumber, SLOT(display(int)));
        QObject::connect(potValuePushButton, SIGNAL(pressed()), MainDialog, SLOT(slotSetPotValue()));
        QObject::connect(potValuePushButton, SIGNAL(released()), MainDialog, SLOT(slotClearPotValue()));
        QObject::connect(analogButton, SIGNAL(clicked(bool)), MainDialog, SLOT(slotCaptureAnalog()));

        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QCoreApplication::translate("MainDialog", "Car Audio Steering", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainDialog", "Connection", nullptr));
        speedLabel->setText(QCoreApplication::translate("MainDialog", "Speed", nullptr));
        connectPushButton->setText(QCoreApplication::translate("MainDialog", "Connect", nullptr));
        portLabel->setText(QCoreApplication::translate("MainDialog", "Port", nullptr));
        disconnectPushButton->setText(QCoreApplication::translate("MainDialog", "Disconnect", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainDialog", "Send discrete values", nullptr));
        ringCheckBox->setText(QCoreApplication::translate("MainDialog", "Ring", nullptr));
        potValuePushButton->setText(QCoreApplication::translate("MainDialog", "Set", nullptr));
        analogValue->setTitle(QCoreApplication::translate("MainDialog", "Analog value", nullptr));
        analogButton->setText(QCoreApplication::translate("MainDialog", "Capture", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainDialog", "Programmed buttons", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainDialog", "Load", nullptr));
        pushButton->setText(QCoreApplication::translate("MainDialog", "Save", nullptr));
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
