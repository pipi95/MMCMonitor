#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>
#include <QHostAddress>
#include <QSerialPort>
#include <QPair>


class Config : public QObject
{
    Q_OBJECT
public:
    enum GDCType{ Serialport, TCP, UDP };
    Q_ENUM(GDCType)
    explicit Config(QObject *parent = nullptr);
    GDCType GetGDCType();
    QPair<QHostAddress, quint16> GetGDCAddress();
    QPair<QString ,QSerialPort::BaudRate> GetGDCPort();
    QPair<QHostAddress, quint16> GetTelemetryAddress();

signals:

public slots:
private:
    QSettings *setting;
    QSettings::SettingsMap settingMap;
    void loadDefaultSettings();
    void saveSettings();
    GDCType getGdcType(QString str);
    QString getGdcTypeString(GDCType type);
};

#endif // CONFIG_H
