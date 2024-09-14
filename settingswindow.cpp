#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(FSModel* mod, QSettings* qs, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWindow), model(mod), dfs(qs)
{
    ui->setupUi(this);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

QBitArray SettingsWindow::get_extended_view_opts()
{
    QBitArray opts(8);
    const auto& layout = ui->extended_opts_layout;
    for (int i = 0; i < layout->count(); ++i) {
        QCheckBox* check_box = qobject_cast<QCheckBox*>(layout->itemAt(i)->widget());
        if (check_box && check_box->isChecked()) {
            opts.setBit(i);
        }
    }
    return opts;
}

void SettingsWindow::on_save_button_clicked()
{
    model->change_show_hidden(ui->dot_files_check_box->isChecked());
}

