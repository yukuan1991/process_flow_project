#include "canvas_view.h"
#include <QMouseEvent>
#include "json.hpp"
#include "item/item.h"

using nlohmann::json;

bool canvas_view::init()
{
    if (canvas_body::init () == false)
    {
        return false;
    }

    setViewportUpdateMode(FullViewportUpdate);
    setMouseTracking(true);
    setRenderHints (QPainter::Antialiasing);

    connect (this, &canvas_view::type_changed, [this] (auto && )
    {
        straight_line_item_ = nullptr;
        broken_lines_.clear();
    });

//    connect (this, &canvas_view::scene_item_changed,
//             [] { static auto i = 0; qDebug () << "scene_item_changed" << ++i; });

    connect (this, &canvas_view::scene_item_changed, [this]{ unsaved_content_ = true; });
    connect (this, &canvas_view::saved, [this]{ unsaved_content_ = false; });

    return true;
}

void canvas_view::mousePressEvent(QMouseEvent *event)
{
    switch (return_type())
    {
//    case canvas_view::draw_type::FINISHEDPRODUCTED:
//        finished_product_press_event(event);
//        canvas_body::mousePressEvent (event);
//        break;
    case canvas_view::draw_type::RAWMATERIAL:
        rawmaterial_press_event(event);
        canvas_body::mousePressEvent (event);
        break;
    case canvas_view::draw_type::MACHINING:
        machining_press_event(event);
        canvas_body::mousePressEvent (event);
        break;
    case canvas_view::draw_type::CHECKOUT:
        checkout_press_event(event);
        canvas_body::mousePressEvent (event);
        break;
//    case canvas_view::draw_type::STRAIGHTLINE:
//        straightline_press_event(event);
//        break;
//    case canvas_view::draw_type::BROKENLINE:
//        brokenline_press_event(event);
//        break;
    default:
        canvas_body::mousePressEvent (event);
        break;
    }
}

void canvas_view::mouseMoveEvent(QMouseEvent *event)
{
    switch (return_type())
    {
//    case canvas_view::draw_type::STRAIGHTLINE:
//        straightline_move_event(event);
//        break;
//    case canvas_view::draw_type::BROKENLINE:
//        brokenline_move_event(event);
//        break;
    default:
        canvas_body::mouseMoveEvent (event);
        break;
    }
}

void canvas_view::mouseReleaseEvent(QMouseEvent *event)
{
    switch (return_type())
    {
//    case canvas_view::draw_type::FINISHEDPRODUCTED:
//        finished_product_release_event(event);
//        canvas_body::mouseReleaseEvent (event);
//        break;
    case canvas_view::draw_type::RAWMATERIAL:
        rawmaterial_release_event(event);
        canvas_body::mouseReleaseEvent (event);
        break;
//    case canvas_view::draw_type::STRAIGHTLINE:
//        straightline_release_event(event);
//        break;
//    case canvas_view::draw_type::BROKENLINE:
//        brokenline_release_event(event);
//        break;
    default:
        canvas_body::mouseReleaseEvent (event);
        break;
    }
}

void canvas_view::machining_press_event(QMouseEvent *event)
{
    begin_ = mapToScene (event->pos());
    json data
    {
        {"pos",
            {
                {"x", begin_.x()},
                {"y", begin_.y()}
            }
        },
        {"detail",
            {
                {"type", "加工"},
                {"attribute", ""}
            }
        }
    };
//    auto rawmaterial = raw_material::make(begin_);
    auto machining = item::make(std::move(data));
    const auto rect_center = machining->boundingRect().center();
    auto center_pos = begin_ - rect_center;
    machining->setPos(center_pos);
    machining->setSelected(true);
    scene()->addItem(machining.get());
    connect(machining.get(), &item::xChanged, this, &canvas_view::scene_item_changed);
    connect(machining.get(), &item::yChanged, this, &canvas_view::scene_item_changed);

    emit scene_item_changed ();
    machining.release();
}

void canvas_view::machining_release_event(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit draw_finished();
}

void canvas_view::checkout_press_event(QMouseEvent *event)
{
    begin_ = mapToScene (event->pos());
    json data
    {
        {"pos",
            {
                {"x", begin_.x()},
                {"y", begin_.y()}
            }
        },
        {"detail",
            {
                {"type", "检验"},
                {"attribute", ""}
            }
        }
    };
//    auto rawmaterial = raw_material::make(begin_);
    auto checkout = item::make(std::move(data));
    const auto rect_center = checkout->boundingRect().center();
    auto center_pos = begin_ - rect_center;
    checkout->setPos(center_pos);
    checkout->setSelected(true);
    scene()->addItem(checkout.get());
    connect(checkout.get(), &item::xChanged, this, &canvas_view::scene_item_changed);
    connect(checkout.get(), &item::yChanged, this, &canvas_view::scene_item_changed);

    emit scene_item_changed ();
    checkout.release();
}

void canvas_view::checkout_release_event(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit draw_finished();
}

void canvas_view::rawmaterial_press_event(QMouseEvent *event)
{
    begin_ = mapToScene (event->pos());
    json data
    {
        {"pos",
            {
                {"x", begin_.x()},
                {"y", begin_.y()}
            }
        },
        {"detail",
            {
                {"type", "原材料"},
                {"attribute", ""}
            }
        }
    };
//    auto rawmaterial = raw_material::make(begin_);
    auto rawmaterial = item::make(std::move(data));
    const auto rect_center = rawmaterial->boundingRect().center();
    auto center_pos = begin_ - rect_center;
    rawmaterial->setPos(center_pos);
    rawmaterial->setSelected(true);
    scene()->addItem(rawmaterial.get());
    connect(rawmaterial.get(), &item::xChanged, this, &canvas_view::scene_item_changed);
    connect(rawmaterial.get(), &item::yChanged, this, &canvas_view::scene_item_changed);

    emit scene_item_changed ();
    rawmaterial.release();
}

void canvas_view::rawmaterial_release_event(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit draw_finished();
}

canvas_view::draw_type canvas_view::return_type()
{
    return type_;
}

void canvas_view::set_type_string(const QString &type)
{
    if (type == "产成品")
    {
        set_type(draw_type::FINISHEDPRODUCTED);
    }
    else if (type == "原材料")
    {
        set_type(draw_type::RAWMATERIAL);
    }
    else if (type == "加工")
    {
        set_type(draw_type::MACHINING);
    }
    else if (type == "检验")
    {
        set_type(draw_type::CHECKOUT);
    }
    else if (type == "连线1")
    {
        set_type(draw_type::STRAIGHTLINE);
    }
    else if (type == "连线2")
    {
        set_type(draw_type::BROKENLINE);
    }
    else
    {
        set_type(draw_type::NONE);
    }
}

void canvas_view::set_type(canvas_view::draw_type t)
{
    if (t == type_)
    {
        return;
    }

    type_ = t;
    emit type_changed(t);
}

canvas_view::~canvas_view()
{

}
