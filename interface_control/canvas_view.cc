#include "canvas_view.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include "item/raw_material.h"
#include "item/maker.hpp"
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include "item/broken_line.h"
#include <QKeyEvent>
#include "item/straight_line.h"
#include "item/finished_product.h"
#include "json.hpp"
#include "attribute.h"
#include <QCloseEvent>
#include <QMessageBox>

//void canvas_view::send_scene_changed_signals()
//{
//    emit scene_item_changed();
//}

std::string canvas_view::dump()
{
    return item::dump_scene (scene()).dump (4);
}

bool canvas_view::load(const std::string &data) try
{
    auto content = nlohmann::json::parse (data);

    for (auto & it : content)
    {

        auto the_item = item::make (it);

        connect(the_item.get(), &item::xChanged, this, &canvas_view::scene_item_changed);
        connect(the_item.get(), &item::yChanged, this, &canvas_view::scene_item_changed);

        scene()->addItem (the_item.release ());

    }
    return true;
}
catch (const std::exception & e)
{
    Q_UNUSED(e);
    return false;
}

void canvas_view::print_render(QPrinter *printer)
{
    QPainter painter(printer);
    scene()->render(&painter, QRectF(), scene_.get()->effective_rect ());
}

void canvas_view::scale_object(qreal factor)
{
    auto m = matrix ();
    const auto scale_facter = m.m11();
    const auto new_scale_factor = scale_facter * factor;

    const auto final_scale_factor = new_scale_factor < 1 ? 1 : new_scale_factor;

    auto m_after = QMatrix ();
    m_after.scale(final_scale_factor, final_scale_factor);
    setMatrix (m_after);
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

//QPixmap canvas_view::pixmap()
//{

//    QPixmap pix {200, 160};
//    pix.fill(Qt::transparent);
//    QPainter painter (&pix);
//    QStyleOptionGraphicsItem item;

//    for (auto & it : graphics_)
//    {
//        it->paint(&painter, &item, nullptr);
//    }
//    return pix;
//}

canvas_view::~canvas_view()
{

}



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

void canvas_view::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);

    if (event->modifiers() & Qt::CTRL and event->key() == Qt::Key_A)
    {
        select_allitems();
        event->accept();
    }

    if (event->key() == Qt::Key_Delete)
    {
        delete_selected();
        event->accept();
    }
}

void canvas_view::mousePressEvent(QMouseEvent *event)
{

    switch (return_type())
    {
    case canvas_view::draw_type::FINISHEDPRODUCTED:
        finished_product_press_event(event);
        canvas_body::mousePressEvent (event);
        break;
    case canvas_view::draw_type::RAWMATERIAL:
        rawmaterial_press_event(event);
        canvas_body::mousePressEvent (event);
        break;
    case canvas_view::draw_type::STRAIGHTLINE:
        straightline_press_event(event);
        break;
    case canvas_view::draw_type::BROKENLINE:
        brokenline_press_event(event);
        break;
    default:
        canvas_body::mousePressEvent (event);
        break;
    }
}

void canvas_view::mouseMoveEvent(QMouseEvent *event)
{

    switch (return_type())
    {
    case canvas_view::draw_type::STRAIGHTLINE:
        straightline_move_event(event);
        break;
    case canvas_view::draw_type::BROKENLINE:
        brokenline_move_event(event);
        break;
    default:
        canvas_body::mouseMoveEvent (event);
        break;
    }


}

void canvas_view::mouseReleaseEvent(QMouseEvent *event)
{
    switch (return_type())
    {
    case canvas_view::draw_type::FINISHEDPRODUCTED:
        finished_product_release_event(event);
        canvas_body::mouseReleaseEvent (event);
        break;
    case canvas_view::draw_type::RAWMATERIAL:
        rawmaterial_release_event(event);
        canvas_body::mouseReleaseEvent (event);
        break;
    case canvas_view::draw_type::STRAIGHTLINE:
        straightline_release_event(event);
        break;
    case canvas_view::draw_type::BROKENLINE:
        brokenline_release_event(event);
        break;
    default:
        canvas_body::mouseReleaseEvent (event);
        break;
    }
}


void canvas_view::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("item") and event->source() != this)
    {
        event->accept();
    }
    else
    {
        canvas_body::dragEnterEvent(event);
    }
}

void canvas_view::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("item") and event->source() != this)
    {
        event->accept();
    }
    else
    {
        canvas_body::dragMoveEvent(event);
    }
}

void canvas_view::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("item") and event->source() != this)
    {
        drop_action (event);
        event->accept();
    }
    else
    {
        canvas_body::dropEvent(event);
    }
}

void canvas_view::wheelEvent(QWheelEvent *event)
{
    QGraphicsView::wheelEvent(event);

    if (!(event->modifiers() & Qt::ControlModifier))
    {
        return;
    }

    if (event->delta() > 0)
    {
        scale_object (1.1);
    }
    else
    {
        scale_object (1 / 1.1);
    }
    event->accept();
}

void canvas_view::closeEvent(QCloseEvent *event)
{
    const bool unsaved = unsaved_content_;

    if(unsaved)
    {
        emit view_closed();
        event->ignore ();
    }
    else
    {
        canvas_body::closeEvent(event);
    }
}

void canvas_view::finished_product_press_event(QMouseEvent *event)
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
                {"type", "产成品"},
                {"attribute", ""}
            }
        }
    };
//    auto finished_product = finished_product::make(begin_);
    auto finished_product = item::make(std::move(data));
    const auto rect_center = finished_product->boundingRect().center();
    auto center_pos = begin_ - rect_center;
    finished_product->setPos(center_pos);
    scene()->addItem(finished_product.get());
    connect(finished_product.get(), &item::xChanged, this, &canvas_view::scene_item_changed);
    connect(finished_product.get(), &item::yChanged, this, &canvas_view::scene_item_changed);

    emit scene_item_changed ();
    finished_product.release();
}

void canvas_view::finished_product_release_event(QMouseEvent *event)
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

void canvas_view::straightline_press_event(QMouseEvent *event)
{
    begin_ = mapToScene (event->pos());
}

void canvas_view::straightline_move_event(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
    {
        return;
    }

    const auto current_pos = mapToScene(event->pos());
    const auto mouse_line = QLineF (begin_, current_pos);
    const auto angle = mouse_line.angle();

    QPointF new_pos;

    if ((angle < 180 - 45 and angle > 45 + 0) or (angle > 180 + 45 and angle < 360 - 45))
    {
        new_pos = QPointF(begin_.x(), current_pos.y());
    }
    else
    {
        new_pos = QPointF(current_pos.x(), begin_.y());
    }

    const auto pos = new_pos;

    if( straight_line_item_ == nullptr)
    {
        auto straight_item = scene()->addLine(QLineF(begin_, pos));
        straight_line_item_.reset(straight_item);
    }
    else
    {
        straight_line_item_->setLine(QLineF(begin_, pos));

    }
}

void canvas_view::straightline_release_event(QMouseEvent *event)
{
    Q_UNUSED(event);

    if(straight_line_item_ == nullptr)
    {
        return;
    }

    const auto current_pos = mapToScene(event->pos());
    const auto mouse_line = QLineF (begin_, current_pos);
    const auto angle = mouse_line.angle();

    QPointF new_pos;

    if ((angle < 180 - 45 and angle > 45 + 0) or (angle > 180 + 45 and angle < 360 - 45))
    {
        new_pos = QPointF(begin_.x(), current_pos.y());
    }
    else
    {
        new_pos = QPointF(current_pos.x(), begin_.y());
    }

//    const auto pos = new_pos;
    const auto pos = new_pos - begin_;

    json data
    {
        {
            "pos",
            {
                {"x", begin_.x()},
                {"y", begin_.y()}
            }
        },
        {
            "detail",
            {
                {"type", "连线1"},
                {"points",
                    {pos.x (), pos.y ()}
                }
            }
        }
    };

    auto straight = item::make(std::move (data));
//    auto straight = straight_line::make(begin_, pos);

    scene()->addItem(straight.get());

    connect(straight.get(), &item::xChanged, this, &canvas_view::scene_item_changed);
    connect(straight.get(), &item::yChanged, this, &canvas_view::scene_item_changed);

    emit scene_item_changed();

    straight.release();

    emit draw_finished();
}

void canvas_view::brokenline_press_event(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        const auto scene_pos = mapToScene (event->pos());
        const auto begin_point = broken_lines_.empty() ? scene_pos : broken_lines_.back()->line().p2();

        auto new_line = scene()->addLine({begin_point, begin_point});
        broken_lines_.emplace_back (new_line);

    }
    else if (event->buttons() == Qt::RightButton)
    {
        if (broken_lines_.size() <= 1)
        {
            return;
        }

        QVector<QPointF> points;

        const auto p1 = broken_lines_.front()->line().p1();

        for (auto & it : broken_lines_)
        {
            points.push_back(it->line().p2() - p1);
        }

        json json_points = json::array();

        for (auto & it : points)
        {
            json_points.push_back(it.x());
            json_points.push_back(it.y());
        }

        json data
        {
            {
                "pos",
                {
                    {"x", p1.x()},
                    {"y", p1.y()}
                }
            },
            {
                "detail",
                {
                    {"type", "连线2"},
                    {"points", json_points}
                }
            }
        };

        auto broke = item::make (std::move (data));

        scene()->addItem(broke.get());

        connect(broke.get(), &item::xChanged, this, &canvas_view::scene_item_changed);
        connect(broke.get(), &item::yChanged, this, &canvas_view::scene_item_changed);

        emit scene_item_changed();

        broke.release();

        emit draw_finished();
    }
}

void canvas_view::brokenline_move_event(QMouseEvent *event)
{
    if (broken_lines_.empty())
    {
        return;
    }
    const auto line = broken_lines_.back()->line();
    const auto p1 = line.p1();

    const auto current_ptr = mapToScene(event->pos());
    const auto mouse_line = QLineF (p1, current_ptr);
    const auto angle = mouse_line.angle();

    QPointF _new_p2;
    if ((angle < 180 - 45 and angle > 45 + 0) or (angle > 180 + 45 and angle < 360 - 45))
    {
        _new_p2 = QPointF (p1.x(), current_ptr.y());
    }
    else
    {
        _new_p2 = QPointF (current_ptr.x(), p1.y());
    }

    const auto new_p2 = _new_p2;
    const auto new_line = QLineF (p1, new_p2);
    broken_lines_.back()->setLine(new_line);
}

void canvas_view::brokenline_release_event(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void canvas_view::select_allitems()
{
    const auto list = items ();
    for (auto & item : list)
    {
        item->setSelected(true);
    }
}

void canvas_view::delete_selected()
{
    const auto selected_items = scene ()->selectedItems();
    for (auto & it : selected_items)
    {
        delete it;
    }

    if (!selected_items.empty())
    {
        emit scene_item_changed ();
    }
}


void canvas_view::drop_action(QDropEvent *event)
{
    QString type = event->mimeData ()->data ("item");

    const auto pos = event->pos();
    const auto scene_pos = mapToScene(pos);

    nlohmann::json create_data {{"pos", {{"x", scene_pos.x()}, {"y", scene_pos.y()}}},
                                {"detail", {{"type", type.toStdString()}}}};
//    auto item = make_item(type, scene_pos);
    auto item = item::make(std::move(create_data));

    const auto rect_center = item->boundingRect().center();
    auto center_pos = scene_pos - rect_center;
    item->setPos(center_pos);
    scene ()->addItem(item.get());
    connect(item.get(), &item::xChanged, this, &canvas_view::scene_item_changed);
    connect(item.get(), &item::yChanged, this, &canvas_view::scene_item_changed);

    emit scene_item_changed ();
    item.release();
}

