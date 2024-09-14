#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "fsmodel.h"

#include <QWidget>
#include <QSettings>
#include <QBitArray>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(FSModel* mod, QSettings* qs, QWidget *parent = nullptr);
    ~SettingsWindow();
private slots:
    void on_save_button_clicked();

private:
    QBitArray get_extended_view_opts();
    Ui::SettingsWindow *ui;
    FSModel* model;
    QSettings* dfs;
};

#endif // SETTINGSWINDOW_H
