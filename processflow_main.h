﻿#pragma once
#include <QWidget>
#include "interface_control/canvas_view.h"
#include "interface_control/attribute.h"


class QMdiArea;
class QMdiSubWindow;
namespace Ui {
class processflow_main;
}

class processflow_main final : public QWidget
{
    Q_OBJECT

public:
    explicit processflow_main(QWidget *parent = 0);
    ~processflow_main();
    QMdiArea* area();
private:
    void init_conn();
private:
    void file_new();
    void file_save();
    void save_subwindow (QMdiSubWindow * sub_window);
    void print_order();
private:
    void zoom_in_active();
    void zoom_out_active();
    void help_advice();
private:
    canvas_view* create_canvas_body();
    void canvas_selection (QGraphicsItem* item);
    void on_attribute_clicked ();
    canvas_view* active_canvas_view();
private:
    Ui::processflow_main *ui;
};

