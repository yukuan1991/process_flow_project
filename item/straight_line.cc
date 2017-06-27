#include "straight_line.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

//unique_ptr<straight_line> straight_line::make(QPointF begin, QPointF end)
//{
//    auto ret = unique_ptr<straight_line>(new straight_line(begin, end));

//    if (ret == nullptr or !ret->init())
//    {
//        return nullptr;
//    }

//    return ret;
//}

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
//    painter->drawLine(begin_, end_);


}

//QRectF straight_line::boundingRect() const
//{
//    QPointF offset(0, 1);
//    auto rect = QRectF(QPointF(0, 0) - offset, end_ + offset);
////    auto rect = QRectF(begin_ - offset, end_ + offset);
//    return rect;
//}

QPainterPath straight_line::shape() const
{
    QPainterPath path;
//    QPointF offset(0, 1);
//    path.addRect(QRectF(QPointF (0, 0) - offset, end_ + offset));
////    path.addRect(QRectF(begin_ - offset, end_ + offset));
//    QPainter painter;
//    painter.drawRect(QRectF(QPointF (0, 0) - offset, end_ + offset));
////    painter.drawRect(QRectF(begin_ - offset, end_ + offset));
    const QPointF start(0,0);
    const QLineF line (start, end_);
    const auto polygon = get_line_polygon(line, 10);
    path.addPolygon(polygon);
    return path;
}

//straight_line::straight_line(QPointF begin, QPointF end)
//{
//    begin_ = begin;
//    end_ = end;
//    auto array = json::array();

//    const qreal begin_x = begin_.x();
//    const qreal begin_y = begin_.y();
//    const qreal end_x = end_.x();
//    const qreal end_y = end_.y();

//    array.push_back(begin_x);
//    array.push_back(begin_y);
//    array.push_back(end_x);
//    array.push_back(end_y);

//    add_detail("points", std::move (array));
//}

straight_line::straight_line(nlohmann::json data, QPointF pos, item *parent)
    : item(std::move(data), pos, parent)
{
//    qDebug() << __LINE__;
//    const qreal begin_x = data["detail"]["detail"]["points"][0];
//    qDebug() << __LINE__;
//    const qreal begin_y = data["points"][1];
//    const qreal end_x = data["points"][2];
//    const qreal end_y = data["points"][3];
//    qDebug() << __LINE__;
////    const qreal begin_y = data["points"][1];
////    const qreal end_x = data["points"][2];
////    const qreal end_y = data["points"][3];
//    qDebug() << begin_x;
//    begin_ = QPointF(begin_x, begin_y);
////    qDebug() << __LINE__;
//    end_ = QPointF(end_x, end_y);
//    qDebug() << __LINE__;
//    const auto & data = dump ();
//    auto points = data.find("points");
//    if (points == end (data))
//    {
//        return false;
//    }

//    if (!points->is_array())
//    {
//        return false;
//    }

//    const qreal x = points[0];
//    const qreal y = points[1];
}

bool straight_line::init() try
{
    set_item_type("连线1");
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

//bool straight_line::load_init()
//{
//    const auto & data = dump ();
//    auto points = data.find("points");
//    if (points == end (data))
//    {
//        return false;
//    }

//    if (!points->is_array())
//    {
//        return false;
//    }

//    const qreal begin_x = points[0];
//    const qreal begin_y = points[1];
//    const qreal end_x = points[2];
//    const qreal end_y = points[3];

//    begin_ = QPointF(begin_x, begin_y);
//    end_ = QPointF(end_x, end_y);
//}

