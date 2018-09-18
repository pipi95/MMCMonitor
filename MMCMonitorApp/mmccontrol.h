#ifndef MMCCONTROL_H
#define MMCCONTROL_H

#include "FrameOperator/equipframe.h"
#include "FrameOperator/framesender.h"
#include "mmcstate.h"
#include <QIODevice>
#include <QObject>
#include <QThread>
#include <QtNetwork>

class MMCControl : public QObject {
    Q_OBJECT
public:
    explicit MMCControl(QHostAddress gdcAddr, quint16 port, QObject* parent = nullptr);
    MMCControl(const QString& portName,
        qint32 baudRate = QSerialPort::Baud9600,
        QSerialPort::DataBits dataBits = QSerialPort::Data8,
        QSerialPort::Parity parity = QSerialPort::NoParity,
        QSerialPort::StopBits stopBits = QSerialPort::OneStop,
        QObject* parent = nullptr);

    enum CommandId : quint8 {
        VideoSwitchCID = 1,
        Sync422SwitchCID = 2,
        StateCheck = 3,
        ChangeTelemetryPeriod = 4,
        CommunicationDelayTest = 5,
        TimeSyncronous = 6
    };
    Q_ENUM(CommandId)
    bool IsGdcConnected() { return isGdcConnected; }
signals:
    void TCPConnect(QHostAddress address, quint16 port, QObject *invoker);
public slots:
    void VideoSwitch(MMCState::VideoSource newState[4]);
    void Sync422Switch(MMCState::Sync422Source newState[4]);
    void ComSpeedCheck();
    void BIT();
    void TelemetryPeriod(quint16 period);

private:
    QIODevice* gdcDevice;
    FrameOperatorSpace::FrameSender* gdc;
    QThread* tcpConnectThread;
    QByteArray calcCheckSum(const QByteArray& frame);
    QByteArray buildFrame(const QByteArray& data);
    QByteArray mmcFrameHeader;
    quint8 controlFrameId;
    quint8 checkSumSize;
    quint8 lengthSize;
    quint8 maxControlFrameLength;
    void tcpConnectResult(QIODevice *tcp, bool result);
    bool isGdcConnected;
    void sendToGdc(const QByteArray& content);
    void sendToGdc(const FrameOperatorSpace::EquipFrame& content);
};

#endif // MMCCONTROL_H
