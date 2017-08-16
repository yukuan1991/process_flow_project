#include "conn_line.h"
#include <QPainter>



conn_line::conn_line(QGraphicsObject *start, QGraphicsObject *stop)
{
    connect (start, &QGraphicsObject::xChanged, this, &conn_line::reset_start);
    connect (start, &QGraphicsObject::yChanged, this, &conn_line::reset_start);
    connect (stop, &QGraphicsObject::xChanged, this, &conn_line::reset_stop);
    connect (stop, &QGraphicsObject::yChanged, this, &conn_line::reset_stop);

    connect (start, &QGraphicsObject::destroyed, this, &conn_line::deleteLater);
    connect (stop, &QGraphicsObject::destroyed, this, &conn_line::deleteLater);

    start_ = start->mapToScene (start->shape ().boundingRect ().center ());
    stop_ = stop->mapToScene (stop->shape ().boundingRect ().center ());
}

conn_line::~conn_line()
{

}

void conn_line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
    Q_UNUSED (option);
    Q_UNUSED (w);
    painter->drawLine (start_, stop_);
}

QRectF conn_line::boundingRect() const
{
    const auto min_x = std::min (start_.x (), stop_.x ());
    const auto min_y = std::min (start_.y (), stop_.y ());

    const auto max_x = std::max (start_.x (), stop_.x ());
    const auto max_y = std::max (start_.y (), stop_.y ());

    const auto top_left = QPointF (min_x, min_y);
    const auto bottom_right = QPointF (max_x, max_y);

    return QRectF (top_left, bottom_right);
}

void conn_line::reset_start()
{
    const auto src = dynamic_cast<QGraphicsObject*> (sender ());
    if (src == null)
    {
        this->deleteLater ();
    }

    start_ = src->mapToScene (src->shape ().boundingRect ().center ());
    start_ = src->sceneBoundingRect ().center ();
}

void conn_line::reset_stop()
{
    const auto src = dynamic_cast<QGraphicsObject*> (sender ());
    if (src == null)
    {
        this->deleteLater ();
    }

    stop_ = src->mapToScene (src->shape ().boundingRect ().center ());
}
