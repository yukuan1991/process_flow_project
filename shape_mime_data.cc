#include "shape_mime_data.h"
#include <QGraphicsItem>
#include <item/item.h>

shape_mime_data::shape_mime_data(QList<QGraphicsItem *> items)
{
    foreach (QGraphicsItem *i , items)
    {
        item *sp = qgraphicsitem_cast<item*>(i);
//        items_.append(sp->copy());
    }
}

shape_mime_data::~shape_mime_data()
{
    foreach (QGraphicsItem *i, items_)
    {
        delete i;
    }
    items_.clear();
}
