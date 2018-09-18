#ifndef MMCDISPLAYTABLEMODEL_H
#define MMCDISPLAYTABLEMODEL_H

#include <QAbstractTableModel>
#include "FrameOperator/equipframe.h"

using namespace FrameOperatorSpace;
class MMCDisplayTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MMCDisplayTableModel(QObject *parent = nullptr);

    void SetPeriodTelemetryData();
    void SetBitData();
    void SetCommunicationSpeedData();

    //--------------------
    // reimplemented
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent= QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
signals:

public slots:
    void ParseFrame(const EquipFrame& fr);
protected:
private:
    static const int numberOfContent = 9;
    static const int numberOfColumn = 1;
    unsigned char vsState[4];
    unsigned char sync422State[4];
    unsigned char errorCode;

};

#endif // MMCDISPLAYTABLEMODEL_H
