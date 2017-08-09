#pragma once
#include <QGraphicsScene>
#include <memory>

using std::unique_ptr;
using std::make_unique;

class canvas_scene : public QGraphicsScene
{
    Q_OBJECT
public:
    template<typename ... ARGS>
    static unique_ptr<canvas_scene> make (ARGS && ... args)
    {
        unique_ptr<canvas_scene> ret (new canvas_scene (std::forward<ARGS> (args)...));
        if (ret == nullptr or ret->init() == false)
        {
            return nullptr;
        }
        return ret;
    }
    QRectF effective_rect () { return effective_rect_; }

    virtual ~canvas_scene();
    template<typename ... ARGS>
    canvas_scene (ARGS && ... args) : QGraphicsScene (std::forward<ARGS> (args)...) {}

signals:
    void selection_changed (bool);
private:
    void notification_canvas_select ();
//protected:


    bool init ();

    void drawBackground(QPainter *painter, const QRectF &rect) override;
    //void drawForeground(QPainter *painter, const QRectF &rect) override;
private:
    qreal height_ = 1000;
    qreal width_ = 1000 * 1.4142135;
    QRectF effective_rect_;
};


