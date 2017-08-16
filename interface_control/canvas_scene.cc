#include "canvas_scene.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <json.hpp>
#include <QPainter>
#include <base/lang/move.hpp>
#include "item/item.h"

using nlohmann::json;


bool canvas_scene::init()
{
    connect (this, &canvas_scene::selectionChanged, [this] { notification_canvas_select(); });
    setBackgroundBrush(QColor (230, 230, 230));

    setup_chart ();
    return true;
}

void canvas_scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);
    painter->setBrush(Qt::white);

    auto center_point = sceneRect().center();
    effective_rect_ = QRectF (center_point - QPointF (width_ / 2, height_ / 2), QSizeF (width_, height_));
    painter->drawRect(effective_rect_);

    if (table_ == nullptr)
    {
        auto table = new QTableWidget;
        setup_table (table);
        auto widget = addWidget (table);

        table_ = widget;
        const auto table_rect = table_->boundingRect ();
        const auto offset = table_rect.bottomRight () - table_rect.topLeft ();
        table_->setPos (effective_rect_.bottomRight () - offset);
    }
}

canvas_scene::~canvas_scene()
{
}

void canvas_scene::setup_chart()
{

}

void canvas_scene::setup_table(QTableWidget *table)
{
    table->setRowCount (4);
    table->setColumnCount (2);

    table->horizontalHeader()->hide();
    table->verticalHeader()->hide();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    const auto rowCount = table->rowCount();
    const auto colCount = table->columnCount();
    for(int row = 0; row < rowCount; row++)
    {
        for(int col = 0; col < colCount; col++)
        {
            table->setItem(row, col, new QTableWidgetItem);
            table->item(row, col)->setTextAlignment(Qt::AlignCenter);
        }
    }

    table->item(0, 0)->setData(Qt::DisplayRole, "工艺类型");
    table->item(0, 1)->setData(Qt::DisplayRole, "符号");
    table->item(1, 0)->setData(Qt::DisplayRole, "加工");
    table->item(2, 0)->setData(Qt::DisplayRole, "检验");
    table->item(3, 0)->setData(Qt::DisplayRole, "产成品");

    {
        QLabel* label = new QLabel;
        QPixmap pix("png/加工.png");
        label->setPixmap(pix);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        table->setCellWidget(1, 1, label);
    }
    {
        QLabel* label = new QLabel;
        QPixmap pix("png/检验.png");
        label->setPixmap(pix);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        table->setCellWidget(2, 1, label);
    }
    {
        QLabel* label = new QLabel;
        QPixmap pix("png/产成品.png");
        label->setPixmap(pix);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        table->setCellWidget(3, 1, label);
    }
}

void canvas_scene::notification_canvas_select()
{
    if(selectedItems().size() == 1)
    {
        emit selection_changed(true);
    }
    else
    {
        emit selection_changed(false);
    }
}

