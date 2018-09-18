#include "mmcdisplaytablemodel.h"

MMCDisplayTableModel::MMCDisplayTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

int MMCDisplayTableModel::rowCount(const QModelIndex& ) const
{
    return numberOfContent;
}

int MMCDisplayTableModel::columnCount(const QModelIndex& ) const
{
    return  numberOfColumn;
}
QModelIndex MMCDisplayTableModel::parent(const QModelIndex& ) const
{
    return QModelIndex();

}
QVariant MMCDisplayTableModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return QVariant("Null data");
    default:
        return QVariant("Null data");
    }
}
void MMCDisplayTableModel::ParseFrame(const EquipFrame& fr)
{

}
