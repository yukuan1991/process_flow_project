#include "draw_line.h"
#include <QHBoxLayout>

draw_line::draw_line(QWidget *parent)
    : draw(parent)
{
    auto button_straight_line = new ribbon_tool(this);
    auto button_broken_line = new ribbon_tool(this);

    button_straight_line->setText("直线");
    button_broken_line->setText("折线");

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

    QHBoxLayout* layout = new QHBoxLayout;

    layout->addWidget(button_straight_line);
    layout->addWidget(button_broken_line);

    setLayout(layout);
}
