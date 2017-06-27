#pragma once
#include "item/item.h"

using std::unique_ptr;
class straight_line : public item
{

public:
//    static unique_ptr<straight_line> make (QPointF begin, QPointF end);
    static unique_ptr<straight_line> make (json data, QPointF pos, item* parent);
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* option, QWidget* widget) override;
//    QRectF boundingRect () const override;
    QPainterPath shape () const override;
protected:
//    straight_line(QPointF begin, QPointF end);
    straight_line(json data, QPointF pos, item* parent = nullptr);
    bool init ();
//    bool load_init();
private:
//    QPointF begin_;
    QPointF end_;
};


