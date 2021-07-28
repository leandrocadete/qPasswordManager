#include "databasefilemodel.h"

databaseFileModel::databaseFileModel(QObject *parent) : QAbstractItemModel(parent){}

QModelIndex databaseFileModel::index(int row, int column, const QModelIndex &parent) const {
    // FIXME: Implement me!
}
QModelIndex databaseFileModel::parent(const QModelIndex &index) const{
    // FIXME: Implement me!
}
int databaseFileModel::rowCount(const QModelIndex &parent) const {
    if (!parent.isValid())
        return 100;
    return 100;
}
int databaseFileModel::columnCount(const QModelIndex &parent) const {
    if (!parent.isValid())
        return 5;
    return 5;
}
QVariant databaseFileModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    // FIXME: Implement me!
    return QVariant();
}
