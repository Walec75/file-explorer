#ifndef FSMODEL_H
#define FSMODEL_H

#include <QFileSystemModel>
#include <QObject>

class FSModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit FSModel(QObject *parent = nullptr);
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    void change_show_hidden(bool show);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

#endif // FSMODEL_H
