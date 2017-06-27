#include "ribbon_graph.h"
#include <QLabel>
#include <QToolButton>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>

ribbon_graph::ribbon_graph(QWidget *parent)
    : QFrame (parent)
{
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Raised);

    QLabel* label_graph = new QLabel("图形", this);
    QLabel* label_line = new QLabel("连线", this);

//        label_graph->setFixedSize(32, 32);
//        label_graph->setScaledContents(true);

    auto button_raw_material = new ribbon_tool(this);
    auto button_machining = new ribbon_tool(this);
    auto button_checkout = new ribbon_tool(this);
    auto button_finished_product = new ribbon_tool(this);

    auto button_straight_line = new ribbon_tool(this);
    auto button_broken_line = new ribbon_tool(this);



    button_raw_material->setText("原材料");
    button_machining->setText("加工");    
    button_checkout->setText("检验");
    button_finished_product->setText("产成品");

    button_straight_line->setText("直线");
    button_broken_line->setText("折线");



    button_raw_material->setToolTip("原材料");
    button_machining->setToolTip("加工");
    button_checkout->setToolTip("检验");
    button_finished_product->setToolTip("产成品");

    button_straight_line->setToolTip("直线");
    button_broken_line->setToolTip("折线");

    button_raw_material->setObjectName("原材料");
    button_machining->setObjectName("加工");
    button_checkout->setObjectName("检验");

    connect(button_raw_material, &ribbon_tool::clicked, this, &ribbon_graph::on_button_pressed);
    connect(button_machining, &ribbon_tool::clicked, this, &ribbon_graph::on_button_pressed);
    connect(button_checkout, &ribbon_tool::clicked, this, &ribbon_graph::on_button_pressed);



    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* hlayout_graph = new QHBoxLayout;
    QHBoxLayout* hlayout_line = new QHBoxLayout;

    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(1);
    hlayout_graph->setContentsMargins(1, 1, 1, 1);
    hlayout_graph->setSpacing(1);
    hlayout_line->setContentsMargins(1, 1, 1, 1);
    hlayout_line->setSpacing(1);

    hlayout_graph->addWidget(button_raw_material);
    hlayout_graph->addWidget(button_machining);
    hlayout_graph->addWidget(button_checkout);
    hlayout_graph->addWidget(button_finished_product);

    hlayout_line->addWidget(button_straight_line);
    hlayout_line->addWidget(button_broken_line);

    layout->addWidget(label_graph);
    layout->addLayout(hlayout_graph);
    layout->addWidget(label_line);
    layout->addLayout(hlayout_line);

    setLayout(layout);

//    setFixedWidth(150);
    //    setFixedHeight(200);
}

void ribbon_graph::on_button_pressed()
{
    auto button = dynamic_cast<ribbon_tool*>(sender ()); //返回一个指向信号发送方的指针
    if (button == nullptr)
    {
        return;
    }
    auto name = button->objectName();

    if (name.isEmpty())
    {
        return;
    }

    emit graph_clicked(name);

    ///
//    if (button->isChecked())
//    {
//        for (auto it : imp->buttons)
//        {
//            if (it != button)
//            {
//                it->setChecked (false);
//            }
//        }
//        emit graph_clicked(name);
//    }
//    else
//    {
//        emit graph_clicked("");
//    }
}

//void ribbon_graph::paintEvent(QPaintEvent *)
//{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//}


