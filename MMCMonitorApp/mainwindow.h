#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "FrameOperator/equipframe.h"
#include "FrameOperator/framereceivermmc.h"
#include "FrameOperator/framesender.h"
#include "mmccontrol.h"
#include "config.h"
#include "Log4Qt/appender.h"
#include "Log4Qt/logmanager.h"
#include <QMainWindow>
#include <QComboBox>
#include <QIODevice>

using namespace FrameOperatorSpace;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
signals:
    void MulticastConnect(QHostAddress address, quint16 prot, QObject *invoker);
public slots:
    void PrintFrame(const QList<EquipFrame>& frames);
    void Start();
    void Stop();
    void SetText();
private slots:
    void sendVsCmd();
    void sendSsCmd();
    void multicastResult(QIODevice *udp, bool result);
    void on_pbVideoSwitch_clicked();

    void on_pbSync422Switch_clicked();

private:
    Ui::MainWindow* ui;
    QList<QComboBox *> vsComboBox;
    QList<QComboBox *> ssComboBox;
    FrameReceiverMMC* fo;
    MMCControl *ctrl;
    bool vsInputValidate();
    Config *cfg;
    QThread *multicastConnectThread;
    bool isMulticastConnected;
};

#endif // MAINWINDOW_H
