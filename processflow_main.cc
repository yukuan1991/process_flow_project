#include "processflow_main.h"
#include "ui_processflow_main.h"
#include <QMdiArea>

processflow_main::processflow_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::processflow_main)
{
    ui->setupUi(this);
    ui->mdiarea->setViewMode(QMdiArea::TabbedView);
    ui->mdiarea->setBackground(QBrush(QColor("#D7D7D7")));
    init_conn();
}

processflow_main::~processflow_main()
{
    delete ui;
}

QMdiArea *processflow_main::area()
{
    return ui->mdiarea;
}

void processflow_main::file_new()
{
    auto canvas = create_canvas_body();

    canvas->show();
}

canvas_view *processflow_main::create_canvas_body()
{
    auto canvas = canvas_view::make (ui->mdiarea);
    auto ptr_canvas = canvas.get();

    canvas->setAttribute (Qt::WA_DeleteOnClose);
    canvas->setWindowState(Qt::WindowMaximized);

//    connect (imp->draw_widget.get(), &drag_widget::button_triggered, ptr_canvas, &canvas_view::set_type_string);
//    connect (ptr_canvas, &canvas_view::draw_finished, imp->draw_widget.get(), &drag_widget::reset_status);
//    connect (ptr_canvas, &canvas_view::selection_changed, this, &sheetflow_main::canvas_selection);

//    connect (ptr_canvas, &canvas_view::view_closed, this, &sheetflow_main::on_view_closed, Qt::QueuedConnection);
//    connect (this, &sheetflow_main::attribute_changed, ptr_canvas, &canvas_view::scene_item_changed);

//    ptr_canvas->set_type_string(imp->draw_widget->status());

    ui->mdiarea->addSubWindow(canvas.release ());

    return ptr_canvas;
}

void processflow_main::init_conn()
{
    connect (ui->process_ribbon, &ribbon::create_new, this, &processflow_main::file_new);
}
