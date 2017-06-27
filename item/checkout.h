#pragma once
#include "item/item.h"
#include <QFont>

class checkout : public item
{    
public:
    static unique_ptr<checkout> make (json data,QPointF pos, item* parent,QColor color = Qt::black);
protected:
    checkout(json data, QPointF pos, item* parent = nullptr);
    bool init();
    void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
//    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    QPainterPath shape () const override;

private:
//    QString checkout_info_;
    QFont font_;
};



