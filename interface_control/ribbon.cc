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

using namespace std;

ribbon::ribbon(QWidget *parent)
    : QTabWidget(parent)
{
    this->setTabBar (new ribbon_bar (this));

    setMaximumHeight (130);
    setMinimumHeight (130);

    setup_ui();
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
    auto file = new ribbon_button ("文件", this);
    auto menu = new ribbon_menu (file);
    file->setMenu (menu);
    file->setObjectName("ribbon_button_file");

    auto action =  make_action (QPixmap ("png/新建.png"), "新建");
    connect (action.get (), &QAction::triggered, this, &ribbon::create_new);
    menu->addAction (action.release ());

    action =  make_action (QPixmap ("png/打开.png"), "打开");
    connect (action.get (), &QAction::triggered, this, &ribbon::open);
    menu->addAction (action.release ());

    action =  make_action (QPixmap ("png/保存.png"), "保存");
    connect (action.get (), &QAction::triggered, this, &ribbon::save);
    menu->addAction (action.release ());

    action =  make_action (QPixmap ("png/另存为.png"), "另存为");
    connect (action.get (), &QAction::triggered, this, &ribbon::save_as);
    menu->addAction (action.release ());

    action =  make_action (QPixmap ("png/退出.png"), "退出");
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
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::cut);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/复制.png").scaled (len, len), "复制");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::copy);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/粘贴.png").scaled (len, len), "粘贴");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::paste);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/删除.png").scaled (len, len), "删除");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::del);
        upper_layout->addWidget (btn.release ());
    }

    block1_layout->addLayout (upper_layout.release ());
    auto label = new QLabel ("第一类");
    label->setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
    block1_layout->addWidget (label);
    layout->addLayout (block1_layout.release (), 0);

    auto line = new QFrame(widget.get ());
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget (line);

    auto block2_layout = make_unique<QVBoxLayout> ();

    upper_layout = make_unique<QHBoxLayout> ();
    upper_layout->setContentsMargins(10, 0, 10, 0);

    {
        auto btn = make_button (QPixmap ("png/作业项数.png").scaled (len, len), "改变作业项数");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::change_task_count);
        upper_layout->addWidget (btn.release ());
    }

    block2_layout->addLayout (upper_layout.release ());

    label = new QLabel ("第二类");

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
        auto btn = make_button (QPixmap ("png/剪切.png").scaled (len, len), "剪切");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::cut);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/复制.png").scaled (len, len), "复制");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::copy);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/粘贴.png").scaled (len, len), "粘贴");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::paste);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/删除.png").scaled (len, len), "删除");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::del);
        upper_layout->addWidget (btn.release ());
    }

    block1_layout->addLayout (upper_layout.release ());
    auto label = new QLabel ("第一类");
    label->setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
    block1_layout->addWidget (label);
    layout->addLayout (block1_layout.release (), 0);

    auto line = new QFrame(widget.get ());
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget (line);

    auto block2_layout = make_unique<QVBoxLayout> ();

    upper_layout = make_unique<QHBoxLayout> ();
    upper_layout->setContentsMargins(10, 0, 10, 0);

    {
        auto btn = make_button (QPixmap ("png/作业项数.png").scaled (len, len), "改变作业项数");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::change_task_count);
        upper_layout->addWidget (btn.release ());
    }

    block2_layout->addLayout (upper_layout.release ());

    label = new QLabel ("第二类");

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
        auto btn = make_button (QPixmap ("png/剪切.png").scaled (len, len), "剪切");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::cut);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/复制.png").scaled (len, len), "复制");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::copy);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/粘贴.png").scaled (len, len), "粘贴");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::paste);
        upper_layout->addWidget (btn.release ());
    }

    {
        auto btn = make_button (QPixmap ("png/删除.png").scaled (len, len), "删除");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::del);
        upper_layout->addWidget (btn.release ());
    }

    block1_layout->addLayout (upper_layout.release ());
    auto label = new QLabel ("第一类");
    label->setAlignment (Qt::AlignHCenter | Qt::AlignBottom);
    block1_layout->addWidget (label);
    layout->addLayout (block1_layout.release (), 0);

    auto line = new QFrame(widget.get ());
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget (line);

    auto block2_layout = make_unique<QVBoxLayout> ();

    upper_layout = make_unique<QHBoxLayout> ();
    upper_layout->setContentsMargins(10, 0, 10, 0);

    {
        auto btn = make_button (QPixmap ("png/作业项数.png").scaled (len, len), "改变作业项数");
//        connect (btn.get (), &QToolButton::clicked, this, &ribbon::change_task_count);
        upper_layout->addWidget (btn.release ());
    }

    block2_layout->addLayout (upper_layout.release ());

    label = new QLabel ("第二类");

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


