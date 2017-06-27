#include "machining.h"
#include <QPainter>
#include <QInputDialog>
#include <QStyleOptionGraphicsItem>

unique_ptr<machining> machining::make(json data, QPointF pos, item* parent, QColor color)
{
    Q_UNUSED(color);
    auto ret = unique_ptr <machining>(new machining (std::move(data), pos , parent));
    if (ret == nullptr or !ret->init())
    {
        return nullptr;
    }
    return ret;
}

machining::machining(json data, QPointF pos, item *parent)
    :item(std::move (data), pos, parent)
{
    item_width_ /= narrow_object_ratio_;
    item_height_ /= narrow_object_ratio_;
}

bool machining::init()
{
    add_attribute ("序号");
    add_attribute ("时长");
    add_attribute ("名称");
    add_attribute ("设备");

    set_item_type("加工");
    setZValue(1);

    return true;
}

void machining::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    item::paint(painter, option, widget);

//    const auto & serial_number = machining_info_;

    const QColor color = option->state bitand QStyle::State_Selected ? selected_color() : Qt::black;
    painter->setFont(font_);
    auto the_pen = painter->pen();
    the_pen.setColor(color);
    the_pen.setWidthF(0.02 * item_width_);

    painter->setPen(the_pen);
    painter->setBrush(Qt::white);

    const QFontMetricsF metrics (painter->font());

    const QRectF rect (0.3 * item_width_, item_height_ * 20 / 80, 0.5 * item_width_, item_height_ * 50 / 80);
    painter->drawEllipse(rect);

//    if (serial_number.isEmpty())
//    {
//        return;
    //    }
    auto serial_number = find_attribute("序号");

    if (serial_number != nullopt and !serial_number->empty())
    {
        const auto text_w = metrics.width(serial_number->data());
        const auto text_h = metrics.height();

        auto center = rect.center();

        QRectF text_rect (center.x() - text_w / 2, center.y() - text_h / 2, text_w, text_h);
        painter->drawText(text_rect, Qt::AlignCenter,serial_number->data());
    }

    const auto bounding_center = boundingRect().center();
    const auto bounding_width = boundingRect().width();


    auto time = find_attribute("时长");
    if (time != nullopt and !time->empty ())
    {
        const auto time_text_w = metrics.width(time->data());

        const auto time_text_h = metrics.height();

        const auto time_rect_bottom = bounding_center - QPointF(bounding_width/2, -time_text_h);
        const auto time_rect_topleft = time_rect_bottom - QPointF(time_text_w, time_text_h);
        QRectF time_rect (time_rect_topleft, time_rect_bottom);
        painter->drawText(time_rect, Qt::AlignCenter, tr(time->data()));
    }

    auto name = find_attribute("名称");
    if (name != nullopt and !name->empty())
    {
        const auto name_text_w = metrics.width(name->data());
        const auto name_text_h = metrics.height();

        const auto name_rect_topleft = bounding_center + QPointF(bounding_width/2, -name_text_h);
        const auto name_rect_bottom = name_rect_topleft + QPointF(name_text_w, name_text_h);
        QRectF name_rect (name_rect_topleft, name_rect_bottom);
        painter->drawText(name_rect, Qt::AlignCenter, tr(name->data()));
    }

    auto device = find_attribute("设备");
    if (device != nullopt and !device->empty())
    {
        const auto device_text_w = metrics.width(device->data());
        const auto device_text_h = metrics.height();

        const auto device_rect_topleft = bounding_center + QPointF(bounding_width/2, 0);
        const auto device_rect_bottom = device_rect_topleft + QPointF(device_text_w, device_text_h);

        QRectF device_rect (device_rect_topleft, device_rect_bottom);
        painter->drawText(device_rect, Qt::AlignCenter, tr(device->data()));
    }

    update();
}

//void machining::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
//{
//    item::mouseDoubleClickEvent(event);

//    QInputDialog dlg;
//    dlg.setInputMode (QInputDialog::IntInput);
//    dlg.setLabelText("序号");
//    dlg.setIntMaximum(999);
//    dlg.setIntValue (machining_info_.toInt());

//    auto action = dlg.exec();
//    if (action == QInputDialog::Rejected)
//    {
//        return;
//    }

//    const auto val = dlg.intValue();
//    if (val == 0)
//    {
//        machining_info_.clear();
//    }
//    else
//    {
//        machining_info_ = QString::number(val);
//    }

//    update();
//}

QPainterPath machining::shape() const
{
    QPainterPath path;
    const QRectF rect (0.3 * item_width_, item_height_ * 20 / 80, 0.5 * item_width_, item_height_ * 50 / 80);
    path.addEllipse(rect);

    return path;
}
