﻿#include "draw_graph.h"
#include <QLabel>
#include <QToolButton>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QPainter>

draw_graph::draw_graph(QWidget *parent)
    : draw (parent)
{
    auto button_raw_material = new ribbon_tool(this);
    auto button_machining = new ribbon_tool(this);
    auto button_checkout = new ribbon_tool(this);
    auto button_finished_product = new ribbon_tool(this);

    button_raw_material->setText("原材料");
    button_machining->setText("加工");    
    button_checkout->setText("检验");
    button_finished_product->setText("产成品");

    button_raw_material->setToolTip("原材料");
    button_machining->setToolTip("加工");
    button_checkout->setToolTip("检验");
    button_finished_product->setToolTip("产成品");

    button_raw_material->setObjectName("原材料");
    button_machining->setObjectName("加工");
    button_checkout->setObjectName("检验");
    button_finished_product->setObjectName("产成品");

    buttons_.emplace_back(button_raw_material);
    buttons_.emplace_back(button_machining);
    buttons_.emplace_back(button_checkout);
    buttons_.emplace_back(button_finished_product);



    for (auto it : buttons_)
    {
        it->setCheckable(true);
    }

    connect(button_raw_material, &ribbon_tool::clicked, this, &draw_graph::on_button_pressed);
    connect(button_machining, &ribbon_tool::clicked, this, &draw_graph::on_button_pressed);
    connect(button_checkout, &ribbon_tool::clicked, this, &draw_graph::on_button_pressed);
    connect(button_finished_product, &ribbon_tool::clicked, this, &draw_graph::on_button_pressed);

    QHBoxLayout* layout = new QHBoxLayout;

    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(1);

    layout->addWidget(button_raw_material);
    layout->addWidget(button_machining);
    layout->addWidget(button_checkout);
    layout->addWidget(button_finished_product);

    setLayout(layout);
}









