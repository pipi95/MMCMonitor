#include "config.h"
#include <QDir>
#include <QHostAddress>
#include <QMetaEnum>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSettings>

Config::Config(QObject* parent)
    : QObject(parent)
{
    QString cPath = QDir::currentPath();
    loadDefaultSettings();
    setting = new QSettings(cPath.append("/config.ini"), QSettings::IniFormat, this);
    QStringList configFileKeys = setting->allKeys();
    if (configFileKeys.size() > 0) {
        for (QStringList::const_iterator i = configFileKeys.constBegin();
             i != configFileKeys.constEnd(); ++i) {
            if (settingMap.contains(*i)) {

                settingMap[*i] = setting->value(*i, settingMap.value(*i));
            }
        }
    }
    saveSettings();
}
void Config::saveSettings()
{
    for (QSettings::SettingsMap::const_iterator i = settingMap.constBegin();
         i != settingMap.constEnd(); ++i) {
        setting->setValue(i.key(), i.value());
    }
}

Config::GDCType Config::getGdcType(QString str)
{
    QMetaEnum me = QMetaEnum::fromType<GDCType>();
    int type = me.keyToValue(str.toStdString().c_str());
    if (-1 != type) {
        return (GDCType)type;
    } else {
        return TCP;
    }
}
QString Config::getGdcTypeString(GDCType type)
{
    QMetaEnum me = QMetaEnum::fromType<GDCType>();
    return QString("%1").arg(me.valueToKey(type));
}
void Config::loadDefaultSettings()
{
    QHostAddress addr("192.168.8.5");
    QString serialName = "";
    settingMap.insert("GDC/Type", getGdcTypeString(GDCType::TCP));
    settingMap.insert("GDC/IP", addr.toString());
    settingMap.insert("GDC/Port", (qint16)7000);
    if (QSerialPortInfo::availablePorts().size() > 0) {
        serialName = QSerialPortInfo::availablePorts().last().portName();
    }
    settingMap.insert("GDC/COM", serialName);
    settingMap.insert("GDC/Baudrate", QSerialPort::Baud38400);

    settingMap.insert("Telemetry/MulticastIP", "225.0.0.22");
    settingMap.insert("Telemetry/MulticastPort", (quint16)7008);
}

Config::GDCType Config::GetGDCType()
{
    return static_cast<GDCType>(getGdcType(settingMap.value("GDC/Type").toString()));
}
QPair<QHostAddress, quint16> Config::GetGDCAddress()
{
    QPair<QHostAddress, quint16> p;
    p.first = QHostAddress(settingMap.value("GDC/IP").toString());
    p.second = setting->value("GDC/Port").toUInt();
    return p;
}
QPair<QHostAddress, quint16> Config::GetTelemetryAddress()
{

    QPair<QHostAddress, quint16> p;
    p.first = QHostAddress(settingMap.value("Telemetry/MulticastIP").toString());
    p.second = setting->value("Telemetry/MulticastPort").toUInt();
    return p;
}
QPair<QString, QSerialPort::BaudRate> Config::GetGDCPort()
{
    QPair<QString, QSerialPort::BaudRate> p;
    p.first = settingMap.value("GDC/COM").toString();
    p.second = static_cast<QSerialPort::BaudRate>(settingMap.value("GDC/Baudrate").toInt());
    return p;
}
