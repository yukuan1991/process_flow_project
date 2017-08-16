#pragma once
#include <QGraphicsObject>
#include <base/lang/not_null.h>


class conn_line : public QGraphicsObject
{
public:
    conn_line (QGraphicsObject * start, QGraphicsObject * stop);
    ~ conn_line () override;
    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * w) override;
    QRectF boundingRect () const override;
private slots:
    void reset_start ();
    void reset_stop ();
private:
    QPointF start_;
    QPointF stop_;
};
