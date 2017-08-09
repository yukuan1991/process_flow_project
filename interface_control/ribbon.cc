#include "ribbon.h"
#include <QHBoxLayout>
#include <base/lang/scope.hpp>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QApplication>
#include <QFrame>
#include <QLineEdit>
#include <QTabBar>
#include <QWidgetAction>
#include <base/io/file/file.hpp>
#include <QDebug>

using namespace std;
using std::unique_ptr;

ribbon::ribbon(QWidget *parent)
    : QTabWidget(parent), button_enabled_(false)
{
    this->setTabBar (new ribbon_bar (this));

    setMaximumHeight (130);
    setMinimumHeight (130);

    graph_ = make_unique<draw_graph> (this);
    line_ = make_unique<draw_line> (this);

//    connect(this, &ribbon::set_enabled, [this] (bool b){ qDebug() << b; });
    setup_ui();
}

QString ribbon::status()
{
    if(!graph_->status().isEmpty())
    {
        return graph_->status();
    }

    if(!line_->status().isEmpty())
    {
        return line_->status();
    }

    return "";
}

void ribbon::reset_status()
{
    graph_->reset_status();
    line_->reset_status();
}

void ribbon::reset_buttons(draw* d)
{
    auto buttons = d->buttons();
    if(buttons.empty())
    {
        return;
    }

    for (auto it : buttons)
    {
        if(it->isChecked())
        {
            it->setChecked(false);
        }
    }
}

std::unique_ptr<QToolButton> ribbon::make_button(const QPixmap &icon, const QString &text)
{
    auto button = make_unique<ribbon_tool> ();

    button->setIconSize (icon.size ());
    button->setIcon (icon);
    button->setText (text);
    button->setToolButtonStyle (Qt::ToolButtonTextUnderIcon);
    button->setAutoRaise (true);

    return button;
}

void ribbon::setup_ui()
{
    setup_menu();

    this->addTab (ui_edit ().release (), "编辑");
    this->addTab (ui_draw ().release (), "绘图");
    this->addTab (ui_window ().release (), "窗口");
    this->addTab (ui_help ().release (), "帮助");



}

unique_ptr<QWidgetAction> make_action (const QPixmap & pix, const QString & text, QWidget * parent = nullptr)
{
    auto action = make_unique<QWidgetAction> (parent);
    action->setDefaultWidget (new ribbon_menu_item(pix, text, parent));

    return action;
}

void ribbon::setup_menu()
{
    auto file = new ribbon_button ("  文件", this);
    auto menu = new ribbon_menu (file);
    file->setMenu (menu);
    file->setObjectName("ribbon_button_file");

    auto action =  make_action (QPixmap ("png/新建.png"), "  新建");
    connect (action.get (), &QAction::triggered, this, &ribbon::create_new);
    menu->addAction (action.release ());

    action =  make_action (QPixmap ("png/打开.png"), "  打开");
    connect (action.get (), &QAction::triggered, this, &ribbon::open);
    menu->addAction (action.release ());

    action =  make_action (QPixmap ("png/打印.png"), "  打印");
    connect (action.get (), &QAction::triggered, this, &ribbon::print);
    connect (this, &ribbon::set_enabled, action.get(), &QAction::setEnabled);
    menu->addAction (action.release ());

    action =  make_action (QPixmap ("png/保存.png"), "  保存");
    connect (action.get (), &QAction::triggered, this, &ribbon::save);
    connect (this, &ribbon::set_enabled, action.get(), &QAction::setEnabled);
    menu->addAction (action.release ());

    action =  make_action (QPixmap ("png/另存为.png"), "  另存为");
    connect (action.get (), &QAction::triggered, this, &ribbon::save_as);
    connect (this, &ribbon::set_enabled, action.get(), &QAction::setEnabled);
    menu->addAction (action.release ());

    action =  make_action (QPixmap ("png/退出.png"), "  退出");
    connect (action.get (), &QAction::triggered, this, &ribbon::quit);
    menu->addAction (action.release ());

    menu->setContentsMargins(10, 0, 0, 0);
}

std::unique_ptr<QWidget> ribbon::ui_edit()
{
    auto widget = make_unique<QWidget> ();
    auto layout = make_unique<QHBoxLayout> ();

    layout->setContentsMargins (1, 1, 1, 1);
    layout->setSpacing (1);

    auto block1_layout = make_unique<QVBoxLayout> ();
    block1_layout->setContentsMargins (1, 1, 1, 1);
    block1_layout->setSpacing (1);

    auto upper_layout = make_unique<QHBoxLayout> ();
    upper_layout->setContentsMargins(10, 0, 10, 0);
    upper_layout->setSpacing (10);

    constexpr auto len = 39;
    {
        auto btn = make_button (QPixmap ("png/剪切.png").scaled (len, len), "剪切");
        connect (this, &ribbon::set_enabled, btn.get(), &ribbon_tool::setEnabled);
        connect (btn.get (), &QToolButton::clicked, this, &ribbon::cut);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/复制.png").scaled (len, len), "复制");
        connect (this, &ribbon::set_enabled, btn.get(), &ribbon_tool::setEnabled);
        connect (btn.get (), &QToolButton::clicked, this, &ribbon::copy);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/粘贴.png").scaled (len, len), "粘贴");
        connect (this, &ribbon::set_enabled, btn.get(), &ribbon_tool::setEnabled);
        connect (btn.get (), &QToolButton::clicked, this, &ribbon::paste);
        upper_layout->addWidget (btn.release ());
    }


    {
        auto btn = make_button (QPixmap ("png/删除.png").scaled (len, len), "删除");
        btn->setToolTip("Delete");
        connect (this, &ribbon::set_enabled, btn.get(), &ribbon_tool::setEnabled);
        connect (btn.get (), &ribbon_tool::clicked, this, &ribbon::delete_selected);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/全选.png").scaled (len, len), "全选");
        btn->setToolTip("Ctrl + A");
        connect (this, &ribbon::set_enabled, btn.get(), &ribbon_tool::setEnabled);
        connect (btn.get (), &ribbon_tool::clicked, this, &ribbon::selected_all);
        upper_layout->addWidget (btn.release ());
    }

    block1_layout->addLayout (upper_layout.release ());
    auto label = new QLabel ("画布操作");
    label->setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
    block1_layout->addWidget (label);
    layout->addLayout (block1_layout.release (), 0);

    auto line = new QFrame(widget.get ());
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget (line);

    layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    widget->setLayout (layout.release ());

    return widget;
}

std::unique_ptr<QWidget> ribbon::ui_draw()
{
    auto widget = make_unique<QWidget> ();
    auto layout = make_unique<QHBoxLayout> ();

    layout->setContentsMargins (1, 1, 1, 1);
    layout->setSpacing (1);

    auto block1_layout = make_unique<QVBoxLayout> ();
    block1_layout->setContentsMargins (1, 1, 1, 1);
    block1_layout->setSpacing (1);


    {
        connect(graph_.get(), &draw_graph::graph_clicked,
                this, &ribbon::graph_clicked);
        connect(this, &ribbon::set_enabled, graph_.get(), &draw_graph::set_enabled);
        auto ptr_line = line_.get();
        connect(graph_.get(), &draw_graph::graph_clicked,
                [this, ptr_line] { reset_buttons(ptr_line); });
        block1_layout->addWidget(graph_.get());
    }

    auto label = new QLabel ("绘制图形");

    label->setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
    block1_layout->addWidget (label);

    layout->addLayout (block1_layout.release (), 0);

    auto line = new QFrame(widget.get ());
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget (line);

    auto block2_layout = make_unique<QVBoxLayout> ();
    block2_layout->setContentsMargins (1, 1, 1, 1);
    block2_layout->setSpacing (1);

    {
        connect(line_.get(), &draw_line::graph_clicked,
                this, &ribbon::graph_clicked);
        connect(this, &ribbon::set_enabled, line_.get(), &draw_line::set_enabled);
        auto ptr_graph = graph_.get();
        connect(line_.get(), &draw_graph::graph_clicked,
                [this, ptr_graph] { reset_buttons(ptr_graph); });
        block2_layout->addWidget(line_.get());
    }


    label = new QLabel ("绘制连线");

    label->setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
    block2_layout->addWidget (label);

    layout->addLayout (block2_layout.release (), 0);

    line = new QFrame(widget.get ());
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget (line);

    layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    widget->setLayout (layout.release ());


    return widget;
}

std::unique_ptr<QWidget> ribbon::ui_window()
{
    auto widget = make_unique<QWidget> ();
    auto layout = make_unique<QHBoxLayout> ();

    layout->setContentsMargins (1, 1, 1, 1);
    layout->setSpacing (1);

    auto block1_layout = make_unique<QVBoxLayout> ();
    block1_layout->setContentsMargins (1, 1, 1, 1);
    block1_layout->setSpacing (1);

    auto upper_layout = make_unique<QHBoxLayout> ();
    upper_layout->setContentsMargins(10, 0, 10, 0);
    upper_layout->setSpacing (10);

    constexpr auto len = 39;

    {
        auto btn = make_button (QPixmap ("png/放大.png").scaled (len, len), "放大");
        connect (this, &ribbon::set_enabled, btn.get(), &ribbon_tool::setEnabled);
        connect (btn.get (), &ribbon_tool::clicked, this, &ribbon::zoom_in_active);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/缩小.png").scaled (len, len), "缩小");
        connect (this, &ribbon::set_enabled, btn.get(), &ribbon_tool::setEnabled);
        connect (btn.get (), &ribbon_tool::clicked, this, &ribbon::zoom_out_active);
        upper_layout->addWidget (btn.release ());
    }

    block1_layout->addLayout (upper_layout.release ());
    auto label = new QLabel ("缩放");
    label->setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
    block1_layout->addWidget (label);
    layout->addLayout (block1_layout.release (), 0);

    auto line = new QFrame(widget.get ());
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget (line);

    layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    widget->setLayout (layout.release ());

    return widget;
}

std::unique_ptr<QWidget> ribbon::ui_help()
{
    auto widget = make_unique<QWidget> ();
    auto layout = make_unique<QHBoxLayout> ();

    layout->setContentsMargins (1, 1, 1, 1);
    layout->setSpacing (1);

    auto block1_layout = make_unique<QVBoxLayout> ();
    block1_layout->setContentsMargins (1, 1, 1, 1);
    block1_layout->setSpacing (1);

    auto upper_layout = make_unique<QHBoxLayout> ();
    upper_layout->setContentsMargins(10, 0, 10, 0);
    upper_layout->setSpacing (10);

    constexpr auto len = 39;

    {
        auto btn = make_button (QPixmap ("png/帮助.png").scaled (len, len), "帮助");
        connect (btn.get (), &QToolButton::clicked, this, &ribbon::help);
        upper_layout->addWidget (btn.release ());
    }

    block1_layout->addLayout (upper_layout.release ());
    auto label = new QLabel ("联系我们");
    label->setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
    block1_layout->addWidget (label);
    layout->addLayout (block1_layout.release (), 0);

    auto line = new QFrame(widget.get ());
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget (line);

    layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    widget->setLayout (layout.release ());

    return widget;
}


