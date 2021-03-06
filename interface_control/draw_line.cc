﻿#include "draw_line.h"
#include <QHBoxLayout>

draw_line::draw_line(QWidget *parent)
    : draw(parent)
{
    auto button_straight_line = new ribbon_tool(this);
    auto button_broken_line = new ribbon_tool(this);

    const QSize size = QSize(32, 32);
    button_straight_line->setIcon(QIcon("png/直线.png"));
    button_straight_line->setIconSize(size);
    button_broken_line->setIcon(QIcon("png/折线.png"));
    button_broken_line->setIconSize(size);

    button_straight_line->setToolTip("直线");
    button_broken_line->setToolTip("折线");

    button_straight_line->setObjectName("直线");
    button_broken_line->setObjectName("折线");

    buttons_.emplace_back(button_straight_line);
    buttons_.emplace_back(button_broken_line);

    for (auto it : buttons_)
    {
        it->setCheckable(true);
    }

    connect(button_straight_line, &ribbon_tool::clicked, this, &draw_line::on_button_pressed);
    connect(button_broken_line, &ribbon_tool::clicked, this, &draw_line::on_button_pressed);

    connect(this, &draw_line::set_enabled, button_straight_line, &ribbon_tool::setEnabled);
    connect(this, &draw_line::set_enabled, button_broken_line, &ribbon_tool::setEnabled);

    QHBoxLayout* layout = new QHBoxLayout;

    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(1);

    layout->addWidget(button_straight_line);
    layout->addWidget(button_broken_line);

    setLayout(layout);
}
