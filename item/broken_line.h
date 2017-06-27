#pragma once
#include <memory>
#include <QVector>
#include <QPointF>
#include "item/item.h"

using std::unique_ptr;
class broken_line : public item
{
public:
    static std::unique_ptr<broken_line> make (json data, QPointF pos, item* parent);
    QRectF boundingRect () const override;
    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;
    QPainterPath shape () const override;
private:
    broken_line (QVector<QPointF> points);
    broken_line (json data, QPointF pos, item* parent = nullptr);
    bool init ();
    QVector<QPointF> points_;

};





