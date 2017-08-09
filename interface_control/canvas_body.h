#pragma once
#include "canvas_scene.h"
#include <QGraphicsView>

struct scoped_connection
{
    scoped_connection (QMetaObject::Connection conn)
        :data_ (std::move (conn))
    {

    }

    ~scoped_connection ()
    {
        if (data_)
        {
            QObject::disconnect (data_);
        }
    }

    QMetaObject::Connection data_;
};


class canvas_body : public QGraphicsView
{
    Q_OBJECT
signals:
    void selection_changed (QGraphicsItem*);
public:
    template<typename ... ARGS>
    static unique_ptr<canvas_body> make (ARGS && ... args)
    {
        unique_ptr<canvas_body> ret (new canvas_body (std::forward<ARGS> (args)...));
        if (ret == nullptr or ret->init() == false)
        {
            return nullptr;
        }
        return ret;
    }

    virtual ~canvas_body ();
protected:
    bool init();

    canvas_body(QWidget *parent = Q_NULLPTR): QGraphicsView (parent) { }
    canvas_body(QGraphicsScene *scene, QWidget *parent) : QGraphicsView (scene, parent) { }
    void on_selection_changed ();
protected:
    unique_ptr<canvas_scene> scene_ = canvas_scene::make (QRectF{0, 0, 1500, 1200});
    scoped_connection selection_conn_ = connect (scene_.get (), &canvas_scene::selectionChanged,
                                                       this, &canvas_body::on_selection_changed);
};


