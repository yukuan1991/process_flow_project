#include "raw_material.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <algorithm>

using namespace std;


std::unique_ptr<raw_material> raw_material::make(QPointF point)
{
    auto ret = std::unique_ptr<raw_material> ( new raw_material (point));

    if (ret == nullptr or !ret->init())
    {
        return nullptr;
    }

    return ret;
}

std::unique_ptr<raw_material> raw_material::make(nlohmann::json data, QPointF pos, item *parent)
{
    auto ret = std::unique_ptr<raw_material> ( new raw_material (std::move(data), pos, parent));

    if (ret == nullptr or !ret->init())
    {
        return nullptr;
    }

    return ret;
}

raw_material::raw_material(QPointF point)
{
    const QFontMetricsF metrics(font_);
    const auto height = metrics.height();
    bounding_rect_ = QRectF(0.07 * item_width_ - 2, item_height_ * 25 / 80 -2,
                             0.85 * item_width_ + 2, height * 2 + 2);

    const QPointF pos(point.x() - item_width_/2, point.y() - item_height_/2);
    setPos (pos);

    setSelected(true);
}

raw_material::raw_material(nlohmann::json data, QPointF pos, item *parent)
    : item(std::move(data), pos, parent)
{
    const QFontMetricsF metrics(font_);
    const auto height = metrics.height();
    bounding_rect_ = QRectF(0.07 * item_width_ - 2, item_height_ * 25 / 80 -2,
                             0.85 * item_width_ + 2, height * 2 + 2);

    setPos (pos);
    up_date();


}



bool raw_material::init()
{
    set_item_type("原材料");

    add_attribute("名称");
    add_attribute("规格");
    setZValue(1);

    connect(this, &raw_material::attr_applied, this, &raw_material::up_date);
    return true;
}

void raw_material::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED (widget);

    painter->setFont (font_);

    if (option->state bitand QStyle::State_Selected)
    {
        auto pen = painter->pen ();
        pen.setStyle (Qt::DashLine);
        pen.setWidthF (1.0);
        painter->setPen (pen);

        painter->drawRect (bounding_rect_);
    }

    painter->drawText(name_rect_, Qt::AlignCenter, name_.data());

    painter->drawText(specification_rect_,Qt::AlignCenter, specification_.data());
}

QRectF raw_material::boundingRect() const
{
    return bounding_rect_;
}

//QVariant raw_material::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
//{
//    auto var = item::itemChange(change, value);
//    if (change == ItemSelectedHasChanged)
//    {
//        const auto selected = value.toBool();
//        if (selected == false)
//        {
//            const auto name_is_space = all_of (begin (name_), end (name_),
//                    [] (auto && it ) { return it == ' ' or it == '\t'; });

//            const auto spec_is_space = all_of (begin (specification_), end (specification_),
//                    [] (auto && it ) { return it == ' ' or it == '\t'; });

//            if (name_is_space and spec_is_space)
//            {
//                deleteLater();
//            }
//        }
//    }

//    return var;
//}


void raw_material::up_date()
{
    const auto center = item::boundingRect().center();

    QFontMetricsF metrics(font_);
    const auto height = metrics.height();

    const auto name = find_attribute("名称");
    name_ = *name;
    name_width_ = metrics.width(name_.data());
    const QPointF name_topleft (center.x() - name_width_/2, center.y() - height);
    const QPointF name_bottom (center.x() + name_width_/2, center.y());
    name_rect_ = QRectF(name_topleft, name_bottom);

    const auto specification = find_attribute("规格");
    specification_ = *specification;
    specification_width_ = metrics.width(specification_.data());
    const QPointF specification_topleft (center.x() - specification_width_/2, center.y());
    const QPointF specification_bottom (center.x() + specification_width_/2, center.y() + height);
    specification_rect_ = QRectF(specification_topleft, specification_bottom);

    const auto max_width = std::max(name_width_, specification_width_);

    QPointF topleft(center.x() - max_width/2 - 2, center.y() - height - 2);
    QPointF bottom(center.x() + max_width/2 + 2, center.y() + height + 2);
    if (max_width > (item_width_ * 0.78))
    {
        bounding_rect_ = QRectF(topleft, bottom);
    }
    else
    {
        bounding_rect_ = QRectF(0.07 * item_width_ - 2, item_height_ * 25 / 80 -2,
                                 0.85 * item_width_ + 2, height * 2 + 2);
    }
}




