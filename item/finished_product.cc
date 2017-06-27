#include "finished_product.h"
#include <QInputDialog>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QFocusEvent>
#include <QFontMetricsF>

using namespace std::string_literals;

unique_ptr<finished_product> finished_product::make(QPointF point)
{
    auto ret = unique_ptr <finished_product> (new finished_product(point));

    if (ret == nullptr or !ret->init())
    {
        return nullptr;
    }

    return ret;
}

unique_ptr<finished_product> finished_product::make(nlohmann::json data, QPointF pos, item *parent)
{
    auto ret = unique_ptr<finished_product> (new finished_product (std::move(data), pos, parent));

    if ( ret == nullptr or !ret->init())
    {
        return nullptr;
    }

    return ret;
}

finished_product::finished_product(QPointF point)
{
    const QFontMetricsF metrics(font_);
    const auto height = metrics.height();
    bounding_rect_ = QRectF(0.07 * item_width_ - 2, item_height_ * 25 / 80 -2,
                             0.85 * item_width_ + 2, height * 2 + 2);
    const QPointF pos (point.x() - item_width_/2, point.y() - item_height_/2);
    setPos(pos);
}

finished_product::finished_product(nlohmann::json data, QPointF pos, item *parent)
    : item(std::move(data), pos, parent)
{

    const QFontMetricsF metrics(font_);
    const auto height = metrics.height();
    bounding_rect_ = QRectF(0.07 * item_width_ - 2, item_height_ * 25 / 80 -2,
                             0.85 * item_width_ + 2, height * 2 + 2);
    setPos(pos);
    update_data();
}

bool finished_product::init()
{
    set_item_type("产成品");
    setZValue(1);

    add_attribute("名称");
    add_attribute("数量");
    add_attribute("型号");
    connect(this, &finished_product::attr_applied, this, &finished_product::update_data);
    return true;
}

void finished_product::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    item::paint(painter, option, widget);
    const QColor color = option->state bitand QStyle::State_Selected ? selected_color() : Qt::black;
    {
    auto the_pen = painter->pen();
    the_pen.setColor(color);
    the_pen.setWidthF(0.02 * item_width_);
    painter->setPen(the_pen);
    }

    painter->setBrush(Qt::white);

    painter->drawRect(bounding_rect_);

    painter->drawText(name_number_rect_, Qt::AlignCenter, name_number_.data());

    painter->drawText(model_number_rect_, Qt::AlignCenter, model_number_.data());
    update();
}

QRectF finished_product::boundingRect() const
{
    return bounding_rect_;
}

void finished_product::update_data()
{
    const auto center = item::boundingRect().center();
    QFontMetricsF metrics (font_);
    const auto height = metrics.height();
    const auto p_name = find_attribute("名称");
    auto name = p_name == nullopt ? ""s : *p_name;

    const auto p_number = find_attribute("数量");
    auto number = p_number == nullopt ? ""s : *p_number;

    name_number_ = number.empty() ? name : name + " X " + number;
    const auto first_line_width = metrics.width(name_number_.data());

    const QPointF name_number_topleft (center.x() - first_line_width/2, center.y() - height);
    const QPointF name_number_bottom (center.x() + first_line_width/2, center.y());
    name_number_rect_ = QRectF(name_number_topleft, name_number_bottom);

    const auto p_model = find_attribute("型号");
    model_number_ = p_model == nullopt ? ""s : *p_model;

    const auto second_line_width = metrics.width(model_number_.data());

    const QPointF model_number_topleft (center.x() - second_line_width/2, center.y());
    const QPointF model_number_bottom (center.x() + second_line_width/2, center.y() + height);
    model_number_rect_ = QRectF(model_number_topleft, model_number_bottom);

    const auto max_width = std::max (first_line_width, second_line_width);
    const auto actual_width = std::max (max_width, item_width_);

    const QPointF product_topleft (center.x() - actual_width/2 - 2, center.y() - height - 2);
    const QPointF product_bottom (center.x() + actual_width/2 + 2, center.y() + height + 2);

    if (actual_width > (item_width_ * 0.78))
    {
        bounding_rect_ = QRectF(product_topleft, product_bottom);
    }
    else
    {
        bounding_rect_ = QRectF(0.07 * item_width_ - 2, item_height_ * 25 / 80 -2,
                                 0.85 * item_width_ + 2, height * 2 + 2);
    }
}



