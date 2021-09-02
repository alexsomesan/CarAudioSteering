#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QMutex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();
    void setPortOverride(QString);

signals:
    void deviceConnected();
    void deviceDisconnected();

public slots:
    void slotPortChanged(QString);
    void slotConnectSerial();
    void slotDisconnectSerial();
    void slotSetPotValue();
    void slotClearPotValue();
    void slotCaptureAnalog();
    void slotReadSerial();
    void slotEnableUI();
    void slotDisableUI();
    void slotSaveValues();

private:
    Ui::MainDialog *ui;
    QSerialPort *serPort;
    bool analogCapture;
    QByteArray *serData;
    QMutex *serReadMux;
    QString *serportOverride;

};
#endif // MAINDIALOG_H
