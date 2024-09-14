#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fsmodel.h"
#include "settingswindow.h"

#include <QMainWindow>
#include <QString>
#include <QFileSystemModel>
#include <memory>
#include <QListView>
#include <deque>
#include <QSettings>
#include <QBitArray>

enum icon_size {
    tiny = 10,
    small = 25,
    normal = 40,
    large = 60,
    extra_large = 80
};



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void set_dir_tree_view(const QString& root_path);
    ~MainWindow();

private slots:
    void on_icon_size_combo_box_currentIndexChanged(int index);

    void on_dirtree_view_clicked(const QModelIndex &index);

    void on_main_list_view_doubleClicked(const QModelIndex &index);

    void on_path_line_editingFinished();

    void on_view_type_combo_box_currentIndexChanged(int index);

    void on_check_box_size_stateChanged(int arg1);

    void on_check_box_date_stateChanged(int arg1);

    void on_check_box_type_stateChanged(int arg1);

    void on_icon_size_spinbox_editingFinished();

    void on_go_home_button_clicked();

    void on_go_back_button_clicked();

    void on_go_next_button_clicked();

    void on_main_table_view_doubleClicked(const QModelIndex &index);

    void on_save_settings_button_clicked();

    void on_save_def_dir_clicked();

    void on_change_def_setts_clicked();

    void on_check_box_created_stateChanged(int arg1);

    void on_check_box_perms_stateChanged(int arg1);

    void on_check_box_owner_stateChanged(int arg1);

private:
    void set_working_directory(const QString& path, bool went_back=false);
    void go_back();
    void change_column_state(int checked, int col);
    void set_main_panel_size(int size);
    int get_icon_size();
    void set_default_settings();
    void init_side_panel();
    void init_main_panel(int index);
    void init_size(int size);
    void init_options(const QBitArray& opts);
    void setup_mode_update_connection();
    void resize_views_to_contents();
    QBitArray get_extended_view_options();
    Ui::MainWindow *ui;
    std::unique_ptr<FSModel> model;
    QString cwd_path;
    std::unique_ptr<SettingsWindow> settings_window;
    std::unique_ptr<QSettings> default_settings;
    std::deque<QString> previous_wds;
    std::deque<QString> next_wds;


protected:
    void closeEvent(QCloseEvent* event) override;

};
#endif // MAINWINDOW_H
