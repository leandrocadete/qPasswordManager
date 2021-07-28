#ifndef DATABASEFILEMODEL_H
#define DATABASEFILEMODEL_H

#include <QAbstractItemModel>

class databaseFileModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit databaseFileModel(QObject *parent = nullptr);

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // DATABASEFILEMODEL_H