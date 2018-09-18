#include "mainwindow.h"
#include "FrameOperator/deviceutility.h"
#include "FrameOperator/equipframe.h"
#include "FrameOperator/mmcFrameStructure.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QPair>
#include <vector>

using namespace FrameOperatorSpace;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isMulticastConnected(false)
{
    logger()->info("Start App!");
    cfg = new Config(this);

    QPair<QHostAddress, quint16> p1;
    QPair<QString, QSerialPort::BaudRate> p2;
    switch (cfg->GetGDCType()) {
    case Config::TCP:
        p1 = cfg->GetGDCAddress();
        ctrl = new MMCControl(p1.first, p1.second, this);
        break;
    case Config::Serialport:
        p2 = cfg->GetGDCPort();
        ctrl = new MMCControl(p2.first, p2.second, QSerialPort::Data8, QSerialPort::NoParity, QSerialPort::OneStop, this);
        break;
    default:;
    }
    ui->setupUi(this);
    qRegisterMetaType<QList<EquipFrame>>("QList<EquipFrame>");

    p1 = cfg->GetTelemetryAddress();
    MulticastGetter* getter = new MulticastGetter();
    multicastConnectThread = new QThread();
    getter->moveToThread(multicastConnectThread);
    connect(multicastConnectThread, &QThread::finished, getter, &MulticastGetter::deleteLater);
    connect(getter, &MulticastGetter::resultReady, this, &MainWindow::multicastResult);
    connect(this, &MainWindow::MulticastConnect, getter, &MulticastGetter::doWork);
    multicastConnectThread->start();
    emit MulticastConnect(p1.first, p1.second, this);
    vsComboBox.append(ui->cbVS_1);
    vsComboBox.append(ui->cbVS_2);
    vsComboBox.append(ui->cbVS_3);
    vsComboBox.append(ui->cbVS_4);

    QStringList strs;
    QMetaEnum me = QMetaEnum::fromType<MMCState::VideoSource>();
    for (int i = 0; i < me.keyCount(); ++i) {
        strs.append(me.key(i));
    }
    foreach (QComboBox* cb, vsComboBox) {
        cb->addItems(strs);
        cb->setMinimumWidth(100);
        connect(cb, &QComboBox::currentTextChanged, this, &MainWindow::vsInputValidate);
        cb->setCurrentText(me.valueToKey(MMCState::SDI0));
    }
    ssComboBox.append(ui->cbSS_1);
    ssComboBox.append(ui->cbSS_2);
    ssComboBox.append(ui->cbSS_3);
    ssComboBox.append(ui->cbSS_4);

    me = QMetaEnum::fromType<MMCState::Sync422Source>();
    strs.clear();
    for (int i = 0; i < me.keyCount(); ++i) {
        strs.append(me.key(i));
    }
    foreach (QComboBox* cb, ssComboBox) {
        cb->addItems(strs);
        cb->setMinimumWidth(100);
        cb->setCurrentText(me.valueToKey(MMCState::Input0));
    }

    connect(ui->pbVideoSwitch, &QPushButton::clicked, this, &MainWindow::sendVsCmd);
    connect(ui->pbSync422Switch, &QPushButton::clicked, this, &MainWindow::sendSsCmd);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::multicastResult(QIODevice* udp, bool result)
{
    isMulticastConnected = result;
    if (isMulticastConnected) {
        qDebug() << "Multicast connect successful!";
        FrameIDLib lib;
        vector<FrameTrait*> fts;
        fts.push_back(lib.CloneTrait(ID_FCC_O_Dynamic));
        fts.push_back(lib.CloneTrait(ID_AIS_O_Telemetry));
        fts.push_back(lib.CloneTrait(ID_EO_O_Dynamic));
        fts.push_back(lib.CloneTrait(ID_MMC_O_Telemetry));
        fo = new FrameReceiverMMC(FrameReceiverMMC::MulticastType, udp, fts, this);
    }
    if (nullptr != fo) {
        connect(fo, &FrameReceiverMMC::FrameIsFound, this, &MainWindow::PrintFrame);
        fo->Start();
    }
    multicastConnectThread->quit();
    multicastConnectThread->wait();
}
bool MainWindow::vsInputValidate()
{
    QMetaEnum me = QMetaEnum::fromType<MMCState::VideoSource>();
    MMCState::VideoSource item;
    QMap<MMCState::VideoSource, int> counter;
    counter.insert(MMCState::CVBS0, 0);
    counter.insert(MMCState::CVBS1, 0);
    counter.insert(MMCState::CVBS2, 0);
    counter.insert(MMCState::CVBS3, 0);

    foreach (QComboBox* cb, vsComboBox) {
        item = static_cast<MMCState::VideoSource>(me.keyToValue(cb->currentText().toStdString().c_str()));
        //        item = cb->currentText().toStdString().c_str();
        switch (item) {
        case MMCState::CVBS0:
        case MMCState::CVBS1:
        case MMCState::CVBS2:
        case MMCState::CVBS3:
            counter[item] = counter[item] + 1;
            break;
        default:;
        }
    }
    int counter2 = 0;
    foreach (int d, counter) {
        if (d > 0) {
            counter2++;
        }
    }
    if (counter2 > 2) {
        QMessageBox msg;
        msg.setText(tr("Only two different CVBS signals can be selected, please correct it!"));
        msg.exec();
        return false;
    }
    return true;
}

void MainWindow::sendVsCmd()
{
    if (vsComboBox.size() >= 4 && vsInputValidate()) {
        QMetaEnum me = QMetaEnum::fromType<MMCState::VideoSource>();
        MMCState::VideoSource state[4];
        for (int i = 0; i < 4; ++i) {
            state[i] = static_cast<MMCState::VideoSource>(me.keyToValue(vsComboBox.at(i)->currentText().toStdString().c_str()));
        }
        ctrl->VideoSwitch(state);
    }
}

void MainWindow::sendSsCmd()
{
    if (ssComboBox.size() >= 4) {
        QMetaEnum me = QMetaEnum::fromType<MMCState::Sync422Source>();
        MMCState::Sync422Source state[4];
        for (int i = 0; i < 4; ++i) {
            state[i] = static_cast<MMCState::Sync422Source>(me.keyToValue(ssComboBox.at(i)->currentText().toStdString().c_str()));
        }
        ctrl->Sync422Switch(state);
    }
}

void MainWindow::PrintFrame(const QList<EquipFrame>& frames)
{
    for (QList<EquipFrame>::const_iterator i = frames.begin(); i != frames.end(); ++i) {
        switch (i->GetID()) {
        case ID_MMC_O_Telemetry:
            ui->teMMC->setText(i->ToString());
            break;
        case ID_EO_O_Dynamic:
            ui->teEO->setText(i->ToString());
            break;
        default:
            ui->teElse->setText(i->ToString());
            break;
        }
    }
}

void MainWindow::Start()
{
}

void MainWindow::Stop()
{
}

void MainWindow::SetText()
{
    //    ui->textEditTest->setText(lib.ToString());
}

void MainWindow::on_pbVideoSwitch_clicked()
{
    ui->statusBar->showMessage(u8"视频切换命令已发送", 2000);
}

void MainWindow::on_pbSync422Switch_clicked()
{
    ui->statusBar->showMessage(u8"同步422切换命令已发送", 2000);
}
