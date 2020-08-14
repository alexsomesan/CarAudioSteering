#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

public slots:
    void slotPortChanged(QString);
    void slotConnectSerial();
    void slotDisconnectSerial();
    void slotSetPotValue();
    void slotClearPotValue();

private:
    Ui::MainDialog *ui;
    QSerialPort *serPort;
};
#endif // MAINDIALOG_H
