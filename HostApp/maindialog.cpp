#include "maindialog.h"
#include "./ui_maindialog.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QtDebug>
#include <QTextStream>

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
    , serPort(new QSerialPort)
    , serData(new QByteArray)
    , serReadMux(new QMutex)
{
    ui->setupUi(this);
    slotDisableUI();
    ui->portComboBox->clear();
    Q_FOREACH(const QSerialPortInfo& pif, QSerialPortInfo::availablePorts()) {
        ui->portComboBox->addItem(pif.portName());
    }
    ui->speedComboBox->clear();
    QList<qint32> bauds = QSerialPortInfo::standardBaudRates();
    Q_FOREACH(const qint32& br, bauds) {
        ui->speedComboBox->addItem(QString::number(br, 10));
    }
    ui->speedComboBox->setCurrentIndex(bauds.indexOf(115200));
    analogCapture = false;
    connect(serPort, SIGNAL(readyRead()),this, SLOT(slotReadSerial()) );
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::slotPortChanged(QString port) {
    if(serPort->isOpen()) {
        serPort->close();
        ui->disconnectPushButton->setEnabled(false);
        ui->connectPushButton->setEnabled(true);
    }
    qDebug() << "Changing port to " << port;
    serPort->setPortName(port);
}

void MainDialog::slotConnectSerial() {
    qint32 bSpeed = ui->speedComboBox->currentText().toInt();
    QString portName = serPort->portName();

    qDebug() << "Connecting to" << portName << "at" << bSpeed << "baudrate...";

    serPort->setBaudRate(bSpeed);

    if (serPort->open(QIODevice::ReadWrite)) {
        ui->statusLabel->setText(QString("Connected to %1 at %2 baudrate").arg(portName).arg(bSpeed));
        ui->disconnectPushButton->setEnabled(true);
        ui->connectPushButton->setEnabled(false);
    } else {
        ui->statusLabel->setText("Failed to connect");
        ui->disconnectPushButton->setEnabled(false);
        ui->connectPushButton->setEnabled(true);
    }
}

void MainDialog::slotDisconnectSerial() {
    if (serPort->isOpen()) {
        serPort->close();
        ui->disconnectPushButton->setEnabled(false);
        ui->connectPushButton->setEnabled(true);
        ui->statusLabel->setText("Disconnected");
    }
}

void MainDialog::slotSetPotValue() {
    if(serPort->isOpen()) {
        uint16_t potVal = ui->potValueSlider->value();
        qDebug() << "Setting pot value" << potVal;

        QByteArray payload(4, Qt::Uninitialized);

        payload[0] = 1; // set pot command

        payload[3] = potVal & 0xFF;          // set pot val LSB
        payload[2] = (potVal & 0xFF00) >> 8; // set pot val MSB

        if (ui->ringCheckBox->checkState() == Qt::Checked) {
            payload[1] = 1; // set ring off
        } else {
            payload[1] = 0; // set ring on
        }

        serPort->write(payload);
        serPort->flush();
        qDebug() << "Sent command:" << Qt::hex <<
            (uint8_t)(payload[0]) << "|" <<
            (uint8_t)(payload[1]) << "|" <<
            (uint8_t)(payload[2]) << (uint8_t)(payload[3]);
    }
}

void MainDialog::slotClearPotValue() {
    if(serPort->isOpen()) {
        qDebug() << "Clearing pot value";

        QByteArray payload(4,Qt::Uninitialized);

        payload[0] = 1; // set pot command
        payload[1] = 0; // set ring off
        payload[2] = 0x03; // set pot val MSB
        payload[3] = 0xFF; // set pot val LSB

        serPort->write(payload);
        serPort->flush();
        qDebug() << "Sent command:" << Qt::hex <<
            (uint8_t)(payload[0]) << "|" <<
            (uint8_t)(payload[1]) << "|" <<
            (uint8_t)(payload[2]) << (uint8_t)(payload[3]);
    }
}

void MainDialog::slotCaptureAnalog() {
    QByteArray payload(4,Qt::Uninitialized);

    payload[0] = 0x02; // set adc command
    payload[2] = 0x00; // padding
    payload[3] = 0x00; // padding
    
    if (analogCapture) {
        payload[1] = 0; 
        analogCapture = false;
        ui->analogButton->setText("Capture");
        ui->statusLabel->clear();
    } else {
        payload[1] = 1;
        analogCapture = true;
        ui->analogButton->setText("Stop");
    }    

    serPort->write(payload);
    serPort->flush();
}

void MainDialog::slotReadSerial() {
    QMutexLocker locker(serReadMux);

    serData->clear();
    serData->append(serPort->readAll());

    switch (serData->at(0)) {
    case 0x02:
        uint16_t adcVal = (uint8_t)(serData->at(1)) << 8 | (uint8_t)(serData->at(2));

        if(ui->adcBar->maximum() < adcVal) {
            ui->adcBar->setMaximum(adcVal);
        }
        ui->adcBar->setValue(adcVal);
        ui->statusLabel->setText(QString::number(adcVal));
        break;
    }
}

void MainDialog::slotEnableUI() {
    ui->analogValueBox->setEnabled(true);
    ui->potValueBox->setEnabled(true);
    ui->programmBox->setEnabled(true);
}

void MainDialog::slotDisableUI() {
    ui->analogValueBox->setEnabled(false);
    ui->potValueBox->setEnabled(false);
    ui->programmBox->setEnabled(false);
}