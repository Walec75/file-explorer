#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileInfo>
#include <QStyledItemDelegate>
#include <QDesktopServices>
#include <QFont>
#include <QUrl>

#define EXTENDED_OPTS_COUNT 6

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    default_settings = std::make_unique<QSettings>("Walec", "file_explorer");;
    set_default_settings();
    settings_window = std::make_unique<SettingsWindow>(model.get(), default_settings.get());
    init_side_panel();
    default_settings->beginGroup("main_panel");
    init_main_panel(default_settings->value("view_index", 0).toInt());
    init_size(default_settings->value("icon_size", normal).toInt());
    init_options(default_settings->value("extended_view_options", QBitArray{6}).toBitArray());
    default_settings->endGroup();

}

void MainWindow::set_default_settings()
{

    model = std::make_unique<FSModel>();
    connect(model.get(), &QAbstractItemModel::layoutChanged, this, &MainWindow::resize_views_to_contents);
    connect(model.get(), &QAbstractItemModel::rowsInserted, this, &MainWindow::resize_views_to_contents);
    model->setRootPath("/");

    cwd_path = default_settings->value("General/cwd", QDir::homePath()).toString();
}

void MainWindow::init_side_panel()
{
    ui->dirtree_view->setModel(model.get());
    ui->dirtree_view->setRootIndex(model->index(cwd_path));
    for (int i = 1; i < model->columnCount(); ++i) {
        ui->dirtree_view->hideColumn(i);
    }
}

void MainWindow::init_main_panel(int index)
{
    ui->main_list_view->setModel(model.get());
    ui->main_table_view->setModel(model.get());

    ui->main_list_view->setViewMode(QListView::IconMode);
    // Sets check boxes and page
    on_view_type_combo_box_currentIndexChanged(index);

    ui->main_list_view->setRootIndex(model->index(cwd_path));
    ui->main_table_view->setRootIndex(model->index(cwd_path));
    ui->path_line->setText(cwd_path);
    connect(model.get(), &QAbstractItemModel::layoutChanged, this, &MainWindow::resize_views_to_contents);
    connect(model.get(), &QAbstractItemModel::rowsInserted, this, &MainWindow::resize_views_to_contents);
}

void MainWindow::init_size(int size)
{
    set_main_panel_size(size);
    switch (size) {
    case tiny:
        ui->icon_size_combo_box->setCurrentIndex(0);
        break;
    case small:
        ui->icon_size_combo_box->setCurrentIndex(1);
        break;
    case normal:
        ui->icon_size_combo_box->setCurrentIndex(2);
        break;
    case large:
        ui->icon_size_combo_box->setCurrentIndex(3);
        break;
    case extra_large:
        ui->icon_size_combo_box->setCurrentIndex(4);
        break;
    default:
        ui->icon_size_combo_box->setCurrentIndex(5);
        ui->icon_size_spinbox->setValue(size);
        break;
    }
}


void MainWindow::init_options(const QBitArray& opts)
{
    ui->check_box_size->setChecked(opts.testBit(0));
    change_column_state(opts.testBit(0), 1);
    ui->check_box_type->setChecked(opts.testBit(1));
    change_column_state(opts.testBit(1), 2);
    ui->check_box_date->setChecked(opts.testBit(2));
    change_column_state(opts.testBit(2), 3);
    ui->check_box_perms->setChecked(opts.testBit(3));
    change_column_state(opts.testBit(3), 4);
    ui->check_box_created->setChecked(opts.testBit(4));
    change_column_state(opts.testBit(4), 5);
    ui->check_box_owner->setChecked(opts.testBit(5));
    change_column_state(opts.testBit(5), 6);
}

void resize_to_contents(Ui::MainWindow *ui)
{
    ui->main_table_view->resizeRowsToContents();
    ui->main_table_view->resizeColumnsToContents();
}

void MainWindow::set_working_directory(const QString& path, bool went_back)
{
    if (!went_back) {
        previous_wds.push_front(cwd_path);
        if (previous_wds.size() > 20) {
            previous_wds.pop_back();
        }
    } else {
        next_wds.push_front(cwd_path);
        if (next_wds.size() > 20) {
            next_wds.pop_back();
        }
    }
    cwd_path = path;
    const QModelIndex index = model->index(path);
    ui->main_table_view->setRootIndex(index);
    ui->main_list_view->setRootIndex(index);
    ui->path_line->setText(cwd_path);
    resize_views_to_contents();
}

void MainWindow::go_back()
{
    if (previous_wds.empty()) {
        return;
    }

    set_working_directory(previous_wds.front(), true);
    previous_wds.pop_front();
}


void MainWindow::resize_views_to_contents()
{
    ui->main_table_view->resizeRowsToContents();
    ui->main_table_view->resizeColumnsToContents();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::set_main_panel_size(int size)
{
    ui->main_list_view->setIconSize({size, size});
    ui->main_table_view->setIconSize({size, size});
    QFont font = ui->main_table_view->font();
    font.setPointSize(size/3 > 12 ? size/3 : 12);
    ui->main_table_view->setFont(font);
    resize_to_contents(ui);
}

void MainWindow::on_icon_size_combo_box_currentIndexChanged(int index)
{
    if (index == 5) {
        ui->icon_size_spinbox->setEnabled(true);
        return;
    }
    ui->icon_size_spinbox->setEnabled(false);
    switch (index) {
    case 0:
        set_main_panel_size(tiny);
        break;
    case 1:
        set_main_panel_size(small);
        break;
    case 2:
        set_main_panel_size(normal);
        break;
    case 3:
        set_main_panel_size(large);
        break;
    case 4:
        set_main_panel_size(extra_large);
        break;
    default:
        qInfo() << "Invalid icon size option";
        break;
    }
    resize_views_to_contents();
}

int MainWindow::get_icon_size()
{
    int index = ui->icon_size_combo_box->currentIndex();
    switch (index) {
    case 0:
        return tiny;
    case 1:
        return small;
    case 2:
        return normal;
    case 3:
        return large;
    case 4:
        return extra_large;
    case 5:
        return ui->icon_size_spinbox->value();
    default:
        qInfo() << "Invalid icon size option";
    }
    return normal;
}

void MainWindow::on_dirtree_view_clicked(const QModelIndex &index)
{
    if (model->isDir(index)) {
        set_working_directory(model->filePath(index));
    }
}

void MainWindow::setup_mode_update_connection()
{
    connect(model.get(), &QAbstractItemModel::dataChanged, this, [this]() {
        ui->main_table_view->resizeColumnsToContents();
        ui->main_table_view->resizeRowsToContents();
    });
}

void MainWindow::on_main_list_view_doubleClicked(const QModelIndex &index)
{
    if (model->isDir(index)) {
        set_working_directory(model->filePath(index));
    } else {
        QDesktopServices::openUrl(QUrl::fromLocalFile(model->filePath(index)));
    }
}



void MainWindow::on_path_line_editingFinished()
{
    QFileInfo file_info(ui->path_line->text());
    if (file_info.exists() && file_info.isDir()) {
        set_working_directory(ui->path_line->text());
    } else {
        ui->path_line->setText(cwd_path);
        qInfo() << "Invalid directory path";
    }
}




void MainWindow::on_view_type_combo_box_currentIndexChanged(int index)
{
    ui->main_panel_stacked->setCurrentIndex(index);
    if (index != ui->view_type_combo_box->currentIndex())
        ui->view_type_combo_box->setCurrentIndex(index);
    // TODO use array
    if (index == 0) {
        ui->check_box_date->setEnabled(false);
        ui->check_box_type->setEnabled(false);
        ui->check_box_size->setEnabled(false);
        ui->check_box_perms->setEnabled(false);
        ui->check_box_created->setEnabled(false);
        ui->check_box_owner->setEnabled(false);
    } else if (index == 1) {
        ui->check_box_date->setEnabled(true);
        ui->check_box_type->setEnabled(true);
        ui->check_box_size->setEnabled(true);
        ui->check_box_perms->setEnabled(true);
        ui->check_box_created->setEnabled(true);
        ui->check_box_owner->setEnabled(true);
    }
}


void MainWindow::change_column_state(int checked, int col)
{
    if (checked) {
        ui->main_table_view->showColumn(col);
    } else {
        ui->main_table_view->hideColumn(col);
    }
    resize_views_to_contents();
}

void MainWindow::on_check_box_size_stateChanged(int arg1) { change_column_state(arg1, 1); }

void MainWindow::on_check_box_type_stateChanged(int arg1) { change_column_state(arg1, 2); }

void MainWindow::on_check_box_date_stateChanged(int arg1) { change_column_state(arg1, 3); }

void MainWindow::on_check_box_perms_stateChanged(int arg1) { change_column_state(arg1, 4); }

void MainWindow::on_check_box_created_stateChanged(int arg1) { change_column_state(arg1, 5); }

void MainWindow::on_check_box_owner_stateChanged(int arg1) { change_column_state(arg1, 6); }


void MainWindow::on_icon_size_spinbox_editingFinished()
{
    set_main_panel_size(ui->icon_size_spinbox->value());
}


void MainWindow::on_go_home_button_clicked()
{
    set_working_directory(QDir::homePath());
}


void MainWindow::on_go_back_button_clicked()
{
    if (previous_wds.empty())
        return;
    set_working_directory(previous_wds.front(), true);
    previous_wds.pop_front();
}


void MainWindow::on_go_next_button_clicked()
{
    if (next_wds.empty())
        return;
    set_working_directory(next_wds.front(), false);
    next_wds.pop_front();
}


void MainWindow::on_main_table_view_doubleClicked(const QModelIndex &index)
{
    if (model->isDir(index)) {
        set_working_directory(model->filePath(index));
    } else {
        QDesktopServices::openUrl(QUrl::fromLocalFile(model->filePath(index)));
    }
}


QBitArray MainWindow::get_extended_view_options()
{
    QBitArray result(6);
    if (ui->check_box_size->isChecked())
        result.setBit(0);
    if (ui->check_box_type->isChecked())
        result.setBit(1);
    if (ui->check_box_date->isChecked())
        result.setBit(2);
    if (ui->check_box_perms->isChecked())
        result.setBit(3);
    if (ui->check_box_created->isChecked())
        result.setBit(4);
    if (ui->check_box_owner->isChecked())
        result.setBit(5);
    return result;
}

void MainWindow::on_save_settings_button_clicked()
{
    QSettings& df = *default_settings;


    df.beginGroup("main_panel");
    df.setValue("view_index", ui->view_type_combo_box->currentIndex());
    df.setValue("icon_size", get_icon_size());
    df.setValue("extended_view_options", get_extended_view_options());
    df.endGroup();

}


void MainWindow::on_save_def_dir_clicked()
{
    default_settings->beginGroup("general");
    default_settings->setValue("cwd", cwd_path);
    default_settings->endGroup();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    settings_window->close();
    QMainWindow::closeEvent(event);
}

void MainWindow::on_change_def_setts_clicked()
{
    settings_window->show();
}



