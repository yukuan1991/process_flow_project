#pragma once
#include <QMimeData>

class QGraphicsItem;
class shape_mime_data : public QMimeData
{
    Q_OBJECT
public:
    shape_mime_data(QList<QGraphicsItem*> items);
    ~shape_mime_data();
    QList<QGraphicsItem*> items() const { return items_; }
private:
    QList<QGraphicsItem*> items_;
};

