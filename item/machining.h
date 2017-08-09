#pragma once
#include "item/item.h"
#include <QFont>

class machining : public item
{
public:
    static unique_ptr<machining> make (json data, QPointF pos, item* parent, QColor color = Qt::black);
//    machining* copy();
protected:
    machining(json data, QPointF pos, item* parent = nullptr);
    bool init();
    void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
//    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

    QPainterPath shape() const override;

private:
//    QString machining_info_;
    QFont font_;
};


