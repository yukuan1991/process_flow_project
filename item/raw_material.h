#pragma once
#include <QGraphicsTextItem>
#include <memory>
#include <item/item.h>
#include <QFont>

class raw_material : public item
{
public:
    static std::unique_ptr<raw_material> make (QPointF point);
    static std::unique_ptr<raw_material> make (json data, QPointF pos, item* parent);
protected:
    raw_material (QPointF point);
    raw_material (json data, QPointF pos, item* parent = nullptr);
    bool init ();
    void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;
//    QVariant itemChange (GraphicsItemChange change, const QVariant &value) override;
private:
    void up_date();
private:
    std::string name_;
    std::string specification_;
    qreal name_width_;
    qreal specification_width_;
    QRectF name_rect_;
    QRectF specification_rect_;
    QRectF bounding_rect_;
    QFont font_;
};


