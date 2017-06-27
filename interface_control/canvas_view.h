#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <memory>
#include <vector>
#include <QGraphicsItem>
#include "canvas_body.h"
#include <QPrinter>

using std::make_unique;
using std::unique_ptr;

class canvas_view : public canvas_body
{
    Q_OBJECT
public:
    enum class draw_type
    {
        NONE,
        FINISHEDPRODUCTED,
        RAWMATERIAL,
        MACHINING,
        STRAIGHTLINE,
        BROKENLINE
    };
signals:
    void scale_changed (qreal);
    void type_changed (canvas_view::draw_type);
    void draw_finished ();
    void scene_item_changed();
    void view_closed();
    void saved ();
public:
//    void set_unsaved_content_state();
//    void send_scene_changed_signals();
    std::string dump();
    QString attached_file () { return windowTitle (); }
    void set_attached_file (QString attached_file) { setWindowTitle(attached_file); }
    bool load (const std::string& data);
    bool is_unsaved () { return unsaved_content_; }
public:
    void print_render (QPrinter* printer);
    void scale_object (qreal factor);
public:
    draw_type return_type ();
    void set_type_string (const QString & type);
    void set_type (draw_type t);

public:
//    static unique_ptr<canvas_view> make (QWidget *parent = nullptr) { return make_helper (parent); }
//    static unique_ptr<canvas_view> make (QGraphicsScene *scene, QWidget *parent = nullptr)
//    { return make_helper (scene, parent); }
    template<typename ... ARGS>
    static unique_ptr<canvas_view> make (ARGS && ... args)
    {
        unique_ptr<canvas_view> ret (new canvas_view (std::forward<ARGS> (args)...));
        if (ret == nullptr or ret->init() == false)
        {
            return nullptr;
        }
        return ret;
    }

    ~canvas_view() ;
protected:
    bool init();

    canvas_view(QWidget *parent = Q_NULLPTR): canvas_body (parent) { }
    canvas_view(QGraphicsScene *scene, QWidget *parent) : canvas_body (scene, parent) { }

//    void keyPressEvent (QKeyEvent* event) override;

    void mousePressEvent (QMouseEvent* event) override;
    void mouseMoveEvent (QMouseEvent* event) override;
    void mouseReleaseEvent (QMouseEvent* event) override;

//    void dragEnterEvent(QDragEnterEvent * event) override;
//    void dragMoveEvent (QDragMoveEvent * event) override;
//    void dropEvent (QDropEvent * event) override;
//    void wheelEvent (QWheelEvent* event) override;

//    void closeEvent (QCloseEvent * event) override;
private:
    void machining_press_event (QMouseEvent* event);
    void machining_release_event (QMouseEvent* event);

    void finished_product_press_event (QMouseEvent* event);
    void finished_product_release_event (QMouseEvent* event);

    void rawmaterial_press_event (QMouseEvent* event);
    void rawmaterial_release_event (QMouseEvent* event);

    void straightline_press_event (QMouseEvent* event);
    void straightline_move_event (QMouseEvent* event);
    void straightline_release_event (QMouseEvent* event);

    void brokenline_press_event (QMouseEvent* event);
    void brokenline_move_event (QMouseEvent* event);
    void brokenline_release_event (QMouseEvent* event);
private:
    void select_allitems();
    void delete_selected ();
    void drop_action (QDropEvent* event);
//    template<typename ... ARGS>
//    static unique_ptr<canvas_view> make_helper (ARGS && ... args)
//    {
//        unique_ptr<canvas_view> ret (new canvas_view (std::forward<ARGS> (args)...));
//        if (ret == nullptr or ret->init() == false)
//        {
//            return nullptr;
//        }
//        return ret;
//    }

private:
    canvas_view::draw_type type_ = canvas_view::draw_type::NONE;

    QPointF begin_;

    unique_ptr<QGraphicsLineItem>  straight_line_item_ = nullptr;
    std::vector<unique_ptr<QGraphicsLineItem>> broken_lines_;

    std::vector<QGraphicsItem*> graphics_;

    bool unsaved_content_ = false;
};


