#include "fsmodel.h"

#include <QDateTime>

FSModel::FSModel(QObject *parent)
    : QFileSystemModel{parent}
{}

int FSModel::columnCount(const QModelIndex& parent) const
{
    return QFileSystemModel::columnCount(parent) + 3;
}


QString perms_to_string(QFile::Permissions perms, std::array<QFile::Permission, 3> rwx)
{
    QString res;
    res += perms & rwx[0] ? "r" : "-";
    res += perms & rwx[1] ? "w" : "-";
    res += perms & rwx[2] ? "x" : "-";
    return res;
}


QVariant FSModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    if (index.column() == 1 && role == Qt::DisplayRole) {
        QFileInfo file_info = fileInfo(index);
        if (file_info.isDir()) {
            QDir dir(file_info.absoluteFilePath());
            int item_count = dir.entryList(QDir::Files | QDir::NoDotAndDotDot).count();
            if (item_count == 1)
               return  + "1 item";
            return QString::number(item_count) + " items";
        }
    }

    if (index.column() == 4 && role == Qt::DisplayRole) {
        QFile::Permissions perms = fileInfo(index).permissions();
        QString perms_str;
        perms_str += perms_to_string(perms, {QFile::ReadOwner, QFile::WriteOwner, QFile::ExeOwner});
        perms_str += perms_to_string(perms, {QFile::ReadGroup, QFile::WriteGroup, QFile::ExeGroup});
        perms_str += perms_to_string(perms, {QFile::ReadOther, QFile::WriteOther, QFile::ExeOther});
        return perms_str;
    }

    if (index.column() == 5 && role == Qt::DisplayRole) {
        QDateTime cdate = fileInfo(index).birthTime();
        if (cdate.isValid()) {
            return cdate.toString("dd MMMM");
        }
        return "N/A";
    }

    if (index.column() == 6 && role == Qt::DisplayRole) {
        return fileInfo(index).owner();
    }

    return QFileSystemModel::data(index, role);
}

void FSModel::change_show_hidden(bool show)
{
    if (show) {
        this->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    } else {
        this->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    }
}


QVariant FSModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && section == 4) {
        return tr("Permissions");
    }
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && section == 5) {
        return tr("Created");
    }
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && section == 6) {
        return tr("Owner");
    }
    return QFileSystemModel::headerData(section, orientation, role);
}
