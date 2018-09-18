#ifndef MMCSTATE_H
#define MMCSTATE_H

#include <QObject>

class MMCState : public QObject {
    Q_OBJECT
public:
    explicit MMCState(QObject* parent = nullptr);

    enum VideoSource : quint8 {
        /// <summary>
        /// 模拟视频0通道
        /// </summary>
        CVBS0 = 0,
        /// <summary>
        /// 模拟视频1通道
        /// </summary>
        CVBS1 = 1,
        /// <summary>
        /// 模拟视频2通道
        /// </summary>
        CVBS2 = 2,
        /// <summary>
        /// 模拟视频3通道
        /// </summary>
        CVBS3 = 3,
        /// <summary>
        /// SDI视频0通道
        /// </summary>
        SDI0 = 4,
        /// <summary>
        /// SDI视频1通道
        /// </summary>
        SDI1 = 5,
        /// <summary>
        /// SDI视频2通道
        /// </summary>
        SDI2 = 6,
        /// <summary>
        /// SDI视频3通道
        /// </summary>
        SDI3 = 7,
    };
    Q_ENUM(VideoSource)

    enum Sync422Source : quint8 {
        /// <summary>
        /// 输入通道0
        /// </summary>
        Input0 = 0,
        /// <summary>
        /// 输入通道1
        /// </summary>
        Input1 = 1,
        /// <summary>
        /// 输入通道2
        /// </summary>
        Input2 = 2,
        /// <summary>
        /// 输入通道3
        /// </summary>
        Input3 = 3,
        /// <summary>
        /// 任管机内部数据
        /// </summary>
        MMCInternal = 4,
        /// <summary>
        /// 输入通道0
        /// </summary>
        Disabled = 255
    };
    Q_ENUM(Sync422Source)

signals:

public slots:
};

#endif // MMCSTATE_H
