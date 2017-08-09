#include "straight_line.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

unique_ptr<straight_line> straight_line::make(nlohmann::json data, QPointF pos, item *parent)
{
    auto ret = unique_ptr<straight_line> (new straight_line(std::move(data), pos, parent));

    if (ret == nullptr or !ret->init())
    {
        return nullptr;
    }

    return ret;
}

void straight_line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    item::paint(painter, option, widget);

    const QColor color = option->state bitand QStyle::State_Selected ? selected_color() : Qt::black;
    auto the_pen = painter->pen();
    the_pen.setColor(color);
    painter->setPen(the_pen);

    painter->drawLine(QPointF (0, 0), end_);
//    painter->drawPath(shape());
//    painter->drawRect(boundingRect());

}

QRectF straight_line::boundingRect() const
{
    return QRectF(-5, -5, 1500, 1200);
}


QPainterPath straight_line::shape() const
{
    QPainterPath path;

    const QPointF start(0,0);
    const QLineF line (start, end_);
    const auto polygon = get_line_polygon(line, 10);
    path.addPolygon(polygon);
    return path;
}


straight_line::straight_line(nlohmann::json data, QPointF pos, item *parent)
    : item(std::move(data), pos, parent)
{

}

bool straight_line::init() try
{
    set_item_type("直线");
    const auto & data = dump ();
    auto points = data.find("points");
    if (points == end (data))
    {
        return false;
    }

    if (!points->is_array())
    {
        return false;
    }

    if (points->size() != 2)
    {
        return false;
    }

    const qreal x = points->at(0);
    const qreal y = points->at(1);

    end_ = QPointF(x, y);

    return true;
}
catch (const std::exception & )
{
    return false;
}




