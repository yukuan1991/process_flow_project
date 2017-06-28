#include "processflow_main.h"
#include "ui_processflow_main.h"
#include <QMdiArea>
#include "interface_control/about_us_dlg.h"
#include "item/item.h"
#include <QMdiSubWindow>

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

void processflow_main::help_advice()
{
    const QString text = R"(<html><head/><body><p>如果您有任何需求与改进建议，</p><p>请随时联系IEToolkit君qq3350436646</p>
                         <p><span style=" font-weight:600; color:red">加好友请扫右边二维码---&gt;</span></p></body></html>)";
    const QString qr_code = R"( <a href = "www.shionto.com"> <img src="./png/about-us.png" width="300" height = "400"/></a>)";
    about_us_dlg::show_info(text, qr_code);
}

canvas_view *processflow_main::create_canvas_body()
{
    auto canvas = canvas_view::make (ui->mdiarea);
    auto ptr_canvas = canvas.get();

    canvas->setAttribute (Qt::WA_DeleteOnClose);
    canvas->setWindowState(Qt::WindowMaximized);

    connect (ui->process_ribbon, &ribbon::graph_clicked, ptr_canvas, &canvas_view::set_type_string);
    connect (ptr_canvas, &canvas_view::draw_finished, ui->process_ribbon, &ribbon::reset_status);
    connect (ptr_canvas, &canvas_view::selection_changed, this, &processflow_main::canvas_selection);

//    connect (ptr_canvas, &canvas_view::view_closed, this, &sheetflow_main::on_view_closed, Qt::QueuedConnection);
//    connect (this, &sheetflow_main::attribute_changed, ptr_canvas, &canvas_view::scene_item_changed);

//    ptr_canvas->set_type_string(imp->draw_widget->status());

    ui->mdiarea->addSubWindow(canvas.release ());

    return ptr_canvas;
}

void processflow_main::canvas_selection(QGraphicsItem *the_item)
{
    if (the_item == nullptr)
    {
        ui->attribute_bar->setWidget (nullptr);
        return;
    }

    auto casted_item = dynamic_cast<item*> (the_item);

    if (casted_item == nullptr)
    {
        return;
    }

    auto & data = casted_item->attribute_data ();
    if (data.empty ())
    {
        return;
    }

    if (ui->attribute_bar->isHidden ())
    {
        ui->attribute_bar->show ();
    }

    ui->attribute_widget = attribute::make (data);
    connect (ui->attribute_widget, &attribute::submit, this, &processflow_main::on_attribute_clicked);
    connect (ui->attribute_widget, &attribute::clear, this, &processflow_main::on_attribute_clicked);

//    connect (imp->attribute_widget.get (),&attribute::submit, [this]{ emit attribute_changed(); });
//    connect (imp->attribute_widget.get (),&attribute::clear, [this]{ emit attribute_changed(); });

    ui->attribute_bar->setWidget (ui->attribute_widget);
}

void processflow_main::on_attribute_clicked()
{
    if (ui->attribute_widget == nullptr)
    {
        return;
    }

    auto & changed_data = ui->attribute_widget->changed_data ();

    auto active_view = active_canvas_view ();
    const auto items = active_view->scene ()->selectedItems ();

    if (items.size () != 1)
    {
        return;
    }

    auto the_item = items [0];

    auto casted_item = dynamic_cast<item*>(the_item);

    if (casted_item == nullptr)
    {
        return;
    }

    for (auto & it : changed_data)
    {
        casted_item->set_attribute (it.first, it.second);
    }

    casted_item->update ();
    ///发送信号，通知原材料和产成品
    emit casted_item->attr_applied();
}

canvas_view *processflow_main::active_canvas_view()
{
    if(QMdiSubWindow *active_subwindow = ui->mdiarea->activeSubWindow())
    {
        canvas_view* canvas_ptr = dynamic_cast<canvas_view*> (active_subwindow->widget());
        return canvas_ptr;
    }
    else
    {
        return nullptr;
    }
}

void processflow_main::init_conn()
{
    connect (ui->process_ribbon, &ribbon::create_new, this, &processflow_main::file_new);
    connect (ui->process_ribbon, &ribbon::quit, [this](){ QWidget::close();});
    connect (ui->process_ribbon, &ribbon::help, this, &processflow_main::help_advice);
}
