﻿#include "processflow_main.h"
#include "ui_processflow_main.h"
#include <QMdiArea>
#include "interface_control/about_us_dlg.h"
#include "item/item.h"
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QPrintDialog>
#include <QFileDialog>
#include <base/io/file/file.hpp>
#include <base/utils/charset.hpp>
#include <QCloseEvent>
#include <QDebug>
#include "gen_dlg.h"




#include <base/lang/defines.hpp>

processflow_main::processflow_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::processflow_main)
{
    ui->setupUi(this);
    set_button_enabled();
    ui->mdiarea->setViewMode(QMdiArea::TabbedView);
    setWindowIcon(QIcon("png/工艺流程图标.png"));
    setWindowTitle("Process Flowchart");
    init_conn();
}

processflow_main::~processflow_main()
{
    delete ui;
}

void processflow_main::on_view_closed()
{
    auto ret = QMessageBox::question(this, "提示", "内容尚未保存，是否保存",
                          "保存","忽略","取消");
    if (ret == 2)
    {
        return;
    }
    auto active_window = ui->mdiarea->activeSubWindow();

    if (ret == 0)
    {
        file_save ();
    }

    active_window->deleteLater();
}

void processflow_main::file_new()
{
    auto canvas = create_canvas_body();
    canvas->show();
}

void processflow_main::file_open()
{
    auto file_name = QFileDialog::getOpenFileName(this, "打开文件", ".", "Images (*.pfs)");
    if (file_name.isEmpty())
    {
        return;
    }

    auto file_content = file::read_all(::utf_to_sys(file_name.toStdString()).data());
    if (!file_content)
    {
        QMessageBox::information(this, "打开", "打开文件失败，请检查文件是否存在");
        return;
    }

    auto canvas = create_canvas_body();
    if (!canvas->load (*file_content))
    {
        QMessageBox::information(this, "打开", "打开文件失败，文件已经损坏");
        return;
    }

    canvas->set_attached_file(std::move (file_name));
    canvas->ensureVisible(0, 0, 300,200 ); //打开文件时确保显示画布的区域在左上角
}

void processflow_main::file_save()
{
    auto sub_window = ui->mdiarea->activeSubWindow();
    assert (sub_window);
    save_subwindow (sub_window);
}

void processflow_main::save_subwindow(QMdiSubWindow *sub_window)
{
    auto w = dynamic_cast<canvas_view*> (sub_window->widget()); assert(w); ///获取到当前要保存的窗口

    QString path;
    if (w->attached_file().isEmpty())
    {
        path = QFileDialog::getSaveFileName(this, "文件保存", ".", "Process Flow Sheet (*.pfs)");
        if (path.isEmpty())
        {
            return;
        }
    }
    else
    {
        path = w->attached_file();
    }

    ///这里进行判断
    file::write_buffer (::utf_to_sys(path.toStdString()).data(), w->dump());
    w->set_attached_file(std::move (path));
    emit w->saved ();
}

void processflow_main::file_save_as()
{
    auto sub_window = ui->mdiarea->activeSubWindow();
    assert (sub_window);

    auto w = dynamic_cast<canvas_view*> (sub_window->widget()); assert(w);

    auto path = QFileDialog::getSaveFileName(this, "另存为", ".", "Progress Flow Sheet (*.pfs)");

    if (path.isEmpty())
    {
        return;
    }

    file::write_buffer (::utf_to_sys(path.toStdString()).data(), w->dump());
    w->set_attached_file(std::move (path));
    emit w->saved();
}

void processflow_main::print_order()
{
    auto view = active_canvas_view();
    if (view == nullptr)
    {
        QMessageBox::information(this, "打印", "没有选中的窗口");
        return;
    }

    QPrinter printer;
    printer.setOrientation(QPrinter::Landscape);
    printer.setPageSize(QPrinter::A4);
    QPrintDialog dlg (&printer);
    if (QPrintDialog::Accepted == dlg.exec())
    {
        view->print_render(&printer);
    }
}

void processflow_main::generate_chart ()
{
    auto view = active_canvas_view();
    if (view == null)
    {
        return;
    }
    auto dlg = view->dlg ();

    auto res = dlg->exec ();
    if (res != QDialog::Accepted)
    {
        return;
    }

    view->generate_chart (dlg->dump ());
}

void processflow_main::cut()
{
    auto w = active_canvas_view();
    if(w == nullptr)
    {
        return;
    }
    w->on_cut();
}

void processflow_main::copy()
{
    auto w = active_canvas_view();
    if(w == nullptr)
    {
        return;
    }
    w->on_copy();
}

void processflow_main::paste()
{
    auto w = active_canvas_view();
    if(w == nullptr)
    {
        return;
    }
    w->on_paste();
}

void processflow_main::del()
{
    auto w = active_canvas_view();
    if(w == nullptr)
    {
        return;
    }
    w->delete_selected();
}

void processflow_main::select_allitems()
{
    auto w = active_canvas_view();
    if(w == nullptr)
    {
        return;
    }
    w->select_allitems();
}

void processflow_main::zoom_in_active()
{
    auto active_canvas = active_canvas_view();
    if (active_canvas == nullptr)
    {
        return;
    }

    active_canvas->scale_object(1.1);
}

void processflow_main::zoom_out_active()
{
    auto active_canvas = active_canvas_view();
    if (active_canvas == nullptr)
    {
        return;
    }

    active_canvas->scale_object(1 / 1.1);
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

    connect (this, &processflow_main::attribute_changed, ptr_canvas, &canvas_view::scene_item_changed);

    ///确保当前被按下的图形按钮，在新建画布的时候有效
    ptr_canvas->set_type_string(ui->process_ribbon->status());

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

    connect (ui->attribute_widget,&attribute::submit, [this]{ emit attribute_changed(); });
    connect (ui->attribute_widget,&attribute::clear, [this]{ emit attribute_changed(); });

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
    connect (ui->process_ribbon, &ribbon::open, this, &processflow_main::file_open);
    connect (ui->process_ribbon, &ribbon::save, this, &processflow_main::file_save);
    connect (ui->process_ribbon, &ribbon::save_as, this, &processflow_main::file_save_as);
    connect (ui->process_ribbon, &ribbon::print, this, &processflow_main::print_order);
    connect (ui->process_ribbon, &ribbon::quit, [this](){ QWidget::close();});

    connect (ui->process_ribbon, &ribbon::copy, this, &processflow_main::copy);
    connect (ui->process_ribbon, &ribbon::cut, this, &processflow_main::cut);
    connect (ui->process_ribbon, &ribbon::paste, this, &processflow_main::paste);
    connect (ui->process_ribbon, &ribbon::delete_selected, this, &processflow_main::del);
    connect (ui->process_ribbon, &ribbon::selected_all, this, &processflow_main::select_allitems);

    connect (ui->process_ribbon, &ribbon::zoom_in_active, this, &processflow_main::zoom_in_active);
    connect (ui->process_ribbon, &ribbon::zoom_out_active, this, &processflow_main::zoom_out_active);

    connect (ui->process_ribbon, &ribbon::help, this, &processflow_main::help_advice);
    connect (ui->process_ribbon, &ribbon::gen, this, &processflow_main::generate_chart);

    connect (ui->mdiarea, &QMdiArea::subWindowActivated, this, &processflow_main::set_button_enabled);

}

void processflow_main::set_button_enabled()
{
    const bool has_canvas_view = (active_canvas_view() != nullptr);
    ui->process_ribbon->set_enabled(has_canvas_view);
}
