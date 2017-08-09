#pragma once
#include <QGraphicsTextItem>
#include <QFont>
#include <memory>
#include <item/item.h>

using std::unique_ptr;
class finished_product : public item
{
public:
//    static unique_ptr<finished_product> make (QPointF point);
    static unique_ptr<finished_product> make (json data, QPointF pos, item* parent);
protected:
//    finished_product(QPointF point);
    finished_product(json data, QPointF pos, item* parent = nullptr);
    bool init();
    void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect () const override;
private:
    void update_data ();
private:
    std::string number_;
    std::string name_number_;
    std::string model_number_;
    qreal name_number_width_;
    qreal model_number_width_;
    QRectF name_number_rect_;
    QRectF model_number_rect_;
    QRectF bounding_rect_;
    QFont font_;
};


