#include "item/item.h"
#include <QGraphicsItem>
#include <QPainter>
#include <algorithm>
#include "item/machining.h"
#include "item/checkout.h"
#include "item/raw_material.h"
#include "item/finished_product.h"
#include "item/broken_line.h"
#include "item/straight_line.h"
#include <QGraphicsScene>
#include <QCursor>
#include <QApplication>

using std::find_if;
using std::string;
using std::begin;
using std::end;


void item::add_attribute(const std::string & key)
{
    auto& attribute = item_info_ ["attribute"];
    if (!attribute.is_array())
    {
        attribute = json::array ();
        attribute.push_back({{key, ""}});
        return;
    }

    for (auto & it : attribute)
    {
        if (!it.is_object() or it.empty())
        {
            continue;
        }

        std::string current_key = it.begin().key();
        if (key == current_key)
        {
            return;
        }
    }

    attribute.push_back({{key, ""}});
}


void item::add_detail(const std::string & key, nlohmann::json data)
{
    item_info_[key] = std::move (data);
}

optional<std::string> item::find_attribute(const std::string &key) const try
{
    auto& attribute = item_info_ ["attribute"];
    for (auto & it : attribute)
    {
        if (!it.is_object() or it.empty())
        {
            continue;
        }

        std::string current_key = it.begin().key();
        if (key == current_key)
        {
            std::string value = it.begin().value();
            return std::move (value);
        }
    }

    return {};
}
catch (const std::exception &)
{
    return {};
}

//bool sceneEvent(QEvent *event)
//{
//    if(event->type() == )
//    QCursor cursor;
//    cursor.setShape(Qt::CrossCursor);
//    QApplication::setOverrideCursor(cursor);
//}

QPolygonF item::get_line_polygon(QLineF l, qreal width)
{

    const auto mid_pos = l.pointAt (0.5);
    const auto len = l.length ();
    QMatrix matrix;

    matrix.translate (mid_pos.x (), mid_pos.y ());
    matrix.rotate (- l.angle ());

    const auto y_diff = width / 2;
    const auto rect = QRectF (- len / 2, - y_diff, len, width);
    const auto polygon = QPolygonF (rect);
    auto mapped_polygon = matrix.map (polygon);

    return mapped_polygon;

}



void item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    painter->setPen(Qt::black);
}

//item* item::copy()
//{
//    auto cp_item = new item;
//    cp_item->item_info_ = item_info_;
//    return cp_item;
//}



item::item(nlohmann::json data, QPointF pos, item *parent)
    :QGraphicsObject (parent)
{
    setFlags (ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setCursor (Qt::SizeAllCursor);
    item_info_ = std::move(data);
    setPos(pos);
}


unique_ptr<item> item::make(nlohmann::json full_data, item *parent) try
{
//    qDebug () << full_data.dump(4).data();
    const string type = full_data ["detail"]["type"];
    const qreal x = full_data ["pos"]["x"];
    const qreal y = full_data ["pos"]["y"];
    auto & data = full_data ["detail"];

    const QPointF pos (x, y);
    if (type == "加工")
    {
        return machining::make(std::move(data), pos, parent);

    }
    if (type == "检验")
    {
        return checkout::make(std::move(data), pos, parent);

    }

    if (type == "原材料")
    {
        return raw_material::make(std::move(data), pos, parent);

    }

    if (type == "产成品")
    {
        return finished_product::make(std::move(data), pos, parent);

    }

    if (type == "直线")
    {
        return straight_line::make(std::move(data), pos, parent);

    }

    if (type == "折线")
    {
        return broken_line::make(std::move(data), pos, parent);

    }
    return nullptr;
}
catch (const std::exception & )
{
    return nullptr;
}

nlohmann::json item::dump_scene(QGraphicsScene *scene)
{
    const auto list_item = scene->items ();
    json json_items;
    for (auto it : list_item)
    {
        auto casted_item = dynamic_cast<item*> (it);
        if (casted_item == nullptr)
        {
            continue;
        }

        const auto pos = casted_item->pos ();

        json json_pos {{"x", pos.x ()}, {"y", pos.y ()}};
        json_items.push_back ({{"pos", std::move (json_pos)}, {"detail", casted_item->dump ()}});
    }
    return json_items;
}

void item::set_item_type(const std::string &type)
{
    item_info_ ["type"] = type;
}

void item::set_attribute(string_view key, string value)
{
    auto& attribute = item_info_ ["attribute"];
    if (!attribute.is_array())
    {
        attribute = json::array ();
        attribute.push_back({{key.to_string (), value}});
        return;
    }

    for (auto & it : attribute)
    {
        if (!it.is_object() or it.empty())
        {
            continue;
        }

        std::string current_key = it.begin().key();
        if (key == current_key)
        {
            auto& target = *(it.begin());
            target = value;
            return;
        }
    }

    attribute.push_back({{key.to_string (), value}});
}

QRectF item::boundingRect() const
{
    return {0, 0, item_width_, item_height_};
}




