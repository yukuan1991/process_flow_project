#include "canvas_view.h"
#include <QMouseEvent>
#include "json.hpp"
#include "item/item.h"
#include <QEvent>
#include <QIcon>
#include <QMenu>
#include <QClipboard>
#include <QDebug>

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
    setDragMode(RubberBandDrag);


    connect (this, &canvas_view::type_changed, [this] (auto && )
    {
        straight_line_item_ = nullptr;
        broken_lines_.clear();
    });

//    connect (this, &canvas_view::draw_finished, [this]
//    {
//        this->setCursor(Qt::ArrowCursor);
//    });

//    connect (this, &canvas_view::scene_item_changed,
//             [] { static auto i = 0; qDebug () << "scene_item_changed" << ++i; });

    connect (this, &canvas_view::scene_item_changed, [this]{ unsaved_content_ = true; });
    connect (this, &canvas_view::saved, [this]{ unsaved_content_ = false; });

    return true;
}

//void canvas_view::create_actions()
//{
//    pop_menu_ = new QMenu (this);
//    copy_action_ = new QAction (QIcon("png/复制.png"), "复制", this);
//    paste_action_ = new QAction (QIcon("png/粘贴.png"), "粘贴", this);
//    cut_action_ = new QAction (QIcon("png/剪切.png"), "剪切",this);

//    pop_menu_ = make_unique<QMenu> ();
//    copy_action_ = make_unique<QAction> (QIcon("png/复制.png"), "复制", this);
//    paste_action_ = make_unique<QAction> (QIcon("png/粘贴.png"), "粘贴", this);
//    cut_action_ = make_unique<QAction> (QIcon("png/剪切.png"), "剪切",this);

//}

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
    if(return_type() != canvas_view::draw_type::NONE)
    {
        qDebug() << __FUNCTION__ << " " << __LINE__;
        const auto scene_pos = mapToScene (event->pos());

        if (scene_->effective_rect().contains(scene_pos))
        {
            switch (return_type())
            {
            case canvas_view::draw_type::FINISHEDPRODUCTED:
                qDebug() << __FUNCTION__ << " " << __LINE__;

                finished_product_press_event(event);
                canvas_body::mousePressEvent (event);
                break;
            case canvas_view::draw_type::RAWMATERIAL:
                qDebug() << __FUNCTION__ << " " << __LINE__;

                rawmaterial_press_event(event);
                canvas_body::mousePressEvent (event);
                break;
            case canvas_view::draw_type::MACHINING:
                qDebug() << __FUNCTION__ << " " << __LINE__;

                machining_press_event(event);
                canvas_body::mousePressEvent (event);
                break;
            case canvas_view::draw_type::CHECKOUT:
                qDebug() << __FUNCTION__ << " " << __LINE__;

                checkout_press_event(event);
                canvas_body::mousePressEvent (event);
                break;
            case canvas_view::draw_type::STRAIGHTLINE:
                qDebug() << __FUNCTION__ << " " << __LINE__;

                straightline_press_event(event);
                canvas_body::mousePressEvent(event);
                break;
            case canvas_view::draw_type::BROKENLINE:
                qDebug() << __FUNCTION__ << " " << __LINE__;

                brokenline_press_event(event);
                canvas_body::mousePressEvent(event);
                break;
            default:
                qDebug() << __FUNCTION__ << " " << __LINE__;

                canvas_body::mousePressEvent (event);
                break;
            }
        }
    }
    else
    {
        qDebug() << __FUNCTION__ << " " << __LINE__;

        canvas_body::mousePressEvent (event);
    }
}

void canvas_view::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__ << " " << __LINE__;
    if(return_type() != canvas_view::draw_type::NONE)
    {
        qDebug() << __PRETTY_FUNCTION__ << " " << __LINE__;
        const auto scene_pos = mapToScene (event->pos());
        qDebug() << __PRETTY_FUNCTION__ << " " << __LINE__;
        if (scene_->effective_rect().contains(scene_pos))
        {
            qDebug() << __PRETTY_FUNCTION__ << " " << __LINE__;
            setCursor(Qt::CrossCursor);
        }
//        else
//        {
//            setCursor(Qt::ArrowCursor);
//        }
        switch (return_type())
        {
        case canvas_view::draw_type::STRAIGHTLINE:
            qDebug() << __PRETTY_FUNCTION__ << " " << __LINE__;
            straightline_move_event(event);
            break;
        case canvas_view::draw_type::BROKENLINE:
            qDebug() << __PRETTY_FUNCTION__ << " " << __LINE__;
            brokenline_move_event(event);
            break;
        default:
            qDebug() << __PRETTY_FUNCTION__ << " " << __LINE__;
            canvas_body::mouseMoveEvent (event);
            break;
        }
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        qDebug() << __PRETTY_FUNCTION__ << " " << __LINE__;
        const auto scene_pos = mapToScene (event->pos());
        if (scene_->effective_rect().contains(scene_pos))
        {
            qDebug() << __PRETTY_FUNCTION__ << " " << __LINE__;
            canvas_body::mouseMoveEvent(event);
            auto items = scene_->items();
            for(auto & it : items)
            {
                it->setCursor(Qt::SizeAllCursor);
            }
        }
    }
}

void canvas_view::mouseReleaseEvent(QMouseEvent *event)
{
    switch (return_type())
    {
    case canvas_view::draw_type::FINISHEDPRODUCTED:
        finished_product_release_event(event);
//        canvas_body::mouseReleaseEvent (event);
        break;
    case canvas_view::draw_type::RAWMATERIAL:
        rawmaterial_release_event(event);
//        canvas_body::mouseReleaseEvent (event);
        break;
    case canvas_view::draw_type::STRAIGHTLINE:
        straightline_release_event(event);
//        canvas_body::mouseReleaseEvent (event);
        break;
    case canvas_view::draw_type::BROKENLINE:  
        brokenline_release_event(event);
//        canvas_body::mouseReleaseEvent (event);
        break;
    case canvas_view::draw_type::MACHINING:
        machining_release_event(event);
        break;
    case canvas_view::draw_type::CHECKOUT:
        checkout_release_event(event);
        break;
    default:
        canvas_body::mouseReleaseEvent (event);
        break;
    }
}

void canvas_view::contextMenuEvent(QContextMenuEvent *event)
{
    pop_menu_->clear();
    pop_menu_->addAction(copy_action_);
    pop_menu_->addAction(paste_action_);
    pop_menu_->addAction(cut_action_);

    pop_menu_->exec(QCursor::pos());
    event->accept();
}

//void canvas_view::closeEvent(QCloseEvent *event)
//{
//    const bool unsaved = unsaved_content_;

//    if(unsaved)
//    {
//        emit view_closed();
//        event->ignore ();
//    }
//    else
//    {
//        canvas_body::closeEvent(event);
//    }
//}

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
//    machining->setSelected(true);
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
//    checkout->setSelected(true);
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
//    rawmaterial->setSelected(true);
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
                {"type", "直线"},
                {"points",
                    {pos.x (), pos.y ()}
                }
            }
        }
    };

    auto straight = item::make(std::move (data));
//    auto straight = straight_line::make(begin_, pos);
    straight->setSelected(true);
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
                    {"type", "折线"},
                    {"points", json_points}
                }
            }
        };

        auto broke = item::make (std::move (data));

        broke->setSelected(true);

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

std::string canvas_view::dump()
{
    return item::dump_scene (scene()).dump (4);
}

bool canvas_view::load(const std::string &data)
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
//    qDebug() << type;
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
    else if (type == "直线")
    {
        set_type(draw_type::STRAIGHTLINE);
    }
    else if (type == "折线")
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


void canvas_view::on_cut()
{
    const auto selected_items = scene ()->selectedItems();
    if(!selected_items.empty())
    {
        data_.clear();

        for (auto & it : selected_items)
        {
            auto casted_item = dynamic_cast<item*> (it);
            if (casted_item == nullptr)
            {
                continue;
            }

            const auto pos = casted_item->pos ();

            json json_pos {{"x", pos.x ()}, {"y", pos.y ()}};
            data_.push_back ({{"pos", std::move (json_pos)}, {"detail", casted_item->dump ()}});
            delete it;
        }
    }
}

void canvas_view::on_copy()
{
    const auto selected_items = scene ()->selectedItems();
    if(selected_items.empty())
    {
        return;
    }

    data_.clear();


    for (auto & it : selected_items)
    {
        auto casted_item = dynamic_cast<item*> (it);
        if (casted_item == nullptr)
        {
            continue;
        }

        const auto pos = casted_item->pos ();

        json json_pos {{"x", pos.x ()}, {"y", pos.y () }};
        data_.push_back ({{"pos", json_pos}, {"detail", casted_item->dump ()}});
    }
}

void canvas_view::on_paste()
{
    qDebug() << "on_paste";
    const auto selected_items = scene ()->selectedItems();
    if(!selected_items.empty())
    {
        for (auto & it : selected_items)
        {
            it->setSelected(false);
        }
    }


    if(!data_.empty())
    {
        for(auto& d : data_)
        {
            auto it = item::make(d);
            scene()->addItem(it.get());
            it->setPos (it->pos () + QPointF (10, 10));
            it->setSelected(true);
            it.release();
        }

    }
}

//void canvas_view::on_cut()
//{
//    QList<QGraphicsItem *> copylist ;
//    foreach (QGraphicsItem *i , scene_->selectedItems()) {
//        item *sp = qgraphicsitem_cast<item*>(i);
//        QGraphicsItem * copy = sp->copy();
//        if ( copy )
//            copylist.append(copy);
//    }
//    QUndoCommand *deleteCommand = new DeleteCommand(scene);
//    undoStack->push(deleteCommand);
//    if ( copylist.count() > 0 ){
//        ShapeMimeData * data = new ShapeMimeData( copylist );
//        QApplication::clipboard()->setMimeData(data);
//    }
//}

//void canvas_view::on_copy()
//{
//    shape_mime_data* data = new shape_mime_data(scene_->selectedItems());
//    QApplication::clipboard()->setMimeData(data);
//}

//void canvas_view::on_paste()
//{
//    QMimeData * mp = const_cast<QMimeData*>(QApplication::clipboard()->mimeData()) ;
//    shape_mime_data * data = dynamic_cast<shape_mime_data*>(mp);
//    if (data)
//    {
//        scene_->clearSelection();
//        foreach (QGraphicsItem * i , data->items() )
//        {
//            item *sp = qgraphicsitem_cast<item*>(i);
//            item * copy = sp->copy();
//            if ( copy )
//            {
//                copy->setSelected(true);
//                copy->moveBy(10,10);
//                scene_->addItem(copy);
                //                QUndoCommand *addCommand = new AddCommand(copy, scene);
                //                undoStack->push(addCommand);
//            }
//        }
//    }
//}

canvas_view::~canvas_view()
{

}
