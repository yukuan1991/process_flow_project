#include "broken_line.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

std::unique_ptr<broken_line> broken_line::make(nlohmann::json data, QPointF pos, item *parent)
{
    auto ret = unique_ptr<broken_line> (new broken_line(std::move(data), pos, parent));

    if (ret == nullptr or !ret->init())
    {
        return nullptr;
    }

    return ret;
}

QRectF broken_line::boundingRect() const
{
    return QRectF(-5, -5, 1500, 1200);
}
//{
//    QPointF min(100000, 100000);
//    QPointF max(0, 0);
//    for(auto& point : points_)
//    {
//        if(point.x() < min.x())
//        {
//            min = QPointF(point.x(), min.y());
//        }
//        if(point.y() < min.y())
//        {
//            min = QPointF(min.x(), point.y());
//        }
//        if(point.x() > max.x())
//        {
//            max = QPointF(point.x(), min.y());
//        }
//        if(point.y() > min.y())
//        {
//            max = QPointF(max.x(), point.y());
//        }
//    }
//    const auto x_min = std::min (points_.constFirst().x(), points_.constLast().x());
//    const auto y_min = std::min (points_.constFirst().y(), points_.constLast().y());
//    const auto x_max = std::max (points_.constFirst().x(), points_.constLast().x());
//    const auto y_max = std::max (points_.constFirst().y(), points_.constLast().y());
//    const QPointF left_top (x_min, y_min);
//    const QPointF bottom_right (x_max, y_max);
//    return QRectF(min.x(), min.y() - 5, (max.x() - min.y() + 5), (max.y() - min.y() + 5));
//}

void broken_line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    item::paint(painter, option, widget);

    const QColor color = option->state bitand QStyle::State_Selected ? selected_color() : Qt::black;
    auto the_pen = painter->pen();
    the_pen.setWidthF(0.01 * item_width_);
    the_pen.setColor(color);
    painter->setPen(the_pen);

    painter->drawPolyline(points_);
//    painter->drawPath(shape());
//    painter->drawRect(boundingRect());
}

QPainterPath broken_line::shape() const
{
    QPainterPath path;

    if (points_.empty())
    {
        return {};
    }
    auto start = points_.front();

    std::for_each (points_.begin() + 1, points_.end(), [&] (auto && it)
    {
        const auto line = QLineF (start, it);
        const auto polygon = get_line_polygon (line, 10);
        path.addPolygon(polygon);
        start = it;
    });


    return path;
}


//broken_line::broken_line(QVector<QPointF> points)
//    :item ({}, {}, {})
//    ,points_ (std::move (points))
//{

//}

broken_line::broken_line(nlohmann::json data, QPointF pos, item *parent)
    : item(std::move(data), pos, parent)
{

}


bool broken_line::init() try
{
    set_item_type("折线");
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

    if (points->size() % 2 != 0)
    {
        return false;
    }

    QVector<QPointF> pts;
    pts.push_back({0, 0});

    for (unsigned i = 0; i < points->size(); i += 2)
    {
        const qreal x = points->at(i);
        const qreal y = points->at(i + 1);

        const auto p = QPointF (x, y);
        pts.push_back(p);
    }

    points_ = std::move (pts);

    return true;
}
catch (const std::exception & e)
{
//    qDebug () << e.what ();
    return false;
}




