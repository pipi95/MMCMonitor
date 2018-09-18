#include "mmccontrol.h"
#include "FrameOperator/deviceutility.h"
#include <QThread>

using namespace FrameOperatorSpace;

MMCControl::MMCControl(QHostAddress gdcAddr, quint16 port, QObject* parent)
    : QObject(parent)
    , controlFrameId(0x01)
    , checkSumSize(1)
    , lengthSize(1)
    , maxControlFrameLength(30)
    , isGdcConnected(false)
{
    TcpGetter* getter = new TcpGetter();
    tcpConnectThread = new QThread();
    getter->moveToThread(tcpConnectThread);
    connect(tcpConnectThread, &QThread::finished, getter, &TcpGetter::deleteLater);
    connect(this, &MMCControl::TCPConnect, getter, &TcpGetter::doWork);
    connect(getter, &TcpGetter::resultReady, this, &MMCControl::tcpConnectResult);
    tcpConnectThread->start();
    emit TCPConnect(gdcAddr, port, this);
    mmcFrameHeader = QByteArray::fromHex("4D 43");
}

void MMCControl::tcpConnectResult(QIODevice* tcp, bool result)
{
    isGdcConnected = result;
    if (isGdcConnected) {
        gdcDevice = tcp;
        gdc = new FrameSender(FrameSender::TcpType, gdcDevice, this);
    }
    tcpConnectThread->quit();
    tcpConnectThread->wait();
}
MMCControl::MMCControl(
    const QString& portName,
    qint32 baudRate,
    QSerialPort::DataBits dataBits,
    QSerialPort::Parity parity,
    QSerialPort::StopBits stopBits,
    QObject* parent)
    : QObject(parent)
    , controlFrameId(0x01)
    , checkSumSize(1)
    , lengthSize(1)
    , maxControlFrameLength(30)
    , isGdcConnected(false)
{
    gdcDevice = DeviceUtility::GetSerialPort(portName, baudRate, dataBits, parity, stopBits, this);
    gdc = new FrameSender(FrameSender::SerialportType, gdcDevice, this);
    if (nullptr != gdcDevice) {
        isGdcConnected = true;
    }
    mmcFrameHeader = QByteArray::fromHex("4D 43");
}
void MMCControl::VideoSwitch(MMCState::VideoSource newState[4])
{
    QByteArray data;
    data.append((char)controlFrameId);
    data.append((char)VideoSwitchCID);
    for (int i = 0; i < 4; ++i) {
        data.append((char)newState[i]);
    }
    data.append((int)20, (char)0x00);
    EquipFrame fr(buildFrame(data), ID_MMC_S_Remote_Control);
    sendToGdc(fr);
}

void MMCControl::sendToGdc(const QByteArray& content)
{
    if (isGdcConnected && nullptr != gdc) {
        gdc->Send(content);
    }
}
void MMCControl::sendToGdc(const EquipFrame& content)
{
    if (isGdcConnected && nullptr != gdc) {
        gdc->Send(content);
    }
}
void MMCControl::Sync422Switch(MMCState::Sync422Source newState[4])
{
    QByteArray data;
    data.append((char)controlFrameId);
    data.append((char)Sync422SwitchCID);
    for (int i = 0; i < 4; ++i) {
        data.append((char)newState[i]);
    }
    data.append((int)20, (char)0x00);

    EquipFrame fr(buildFrame(data), ID_MMC_S_Remote_Control);
    sendToGdc(fr);
}
void MMCControl::ComSpeedCheck()
{
}
void MMCControl::BIT()
{
}
void MMCControl::TelemetryPeriod(quint16 period)
{
}

QByteArray MMCControl::calcCheckSum(const QByteArray& frame)
{
    return QByteArray().append(std::accumulate(frame.constBegin(), frame.constEnd(), 0x00));
}

QByteArray MMCControl::buildFrame(const QByteArray& data)
{
    QByteArray content(mmcFrameHeader);
    qint8 length = data.size() + mmcFrameHeader.size() + checkSumSize + lengthSize;
    qDebug() << "Length:" << length;
    if (length <= maxControlFrameLength) {
        content.append(length);
        content.append(data);
        content.append(calcCheckSum(content));
        return content;
    }
    return QByteArray();
}
