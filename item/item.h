#pragma once
#include <QGraphicsObject>
#include <memory>
#include "json.hpp"
//#include "interface.hpp"
#include <experimental/string_view>
#include <experimental/optional>


using std::unique_ptr;
using std::make_unique;
using nlohmann::json;
using string_view = std::experimental::string_view;
using string = std::string;
using std::experimental::optional;
using std::experimental::nullopt;

struct impl_item;

//constexpr qreal standard_width = 100;
//constexpr qreal standard_height = 0.8 * standard_width;

class item : public QGraphicsObject
{
    Q_OBJECT
signals:
    void attr_applied ();
public:
    static QPolygonF get_line_polygon (QLineF l, qreal width);
public:
    static unique_ptr<item> make (json full_data, item*parent = nullptr);
    static json dump_scene (QGraphicsScene* scene);
    const json & dump () { return item_info_; }
    void set_item_type (const std::string & type);
    void set_attribute(string_view key, string value = {});
    const json& attribute_data () { return item_info_ ["attribute"]; }

    QRectF boundingRect () const override;
    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

protected:
    explicit item(json data, QPointF pos, item* parent);
    explicit item(QGraphicsItem *parent = nullptr);
    void add_attribute (const std::string& key);
    void add_detail (const std::string & key, json data);
    optional<std::string> find_attribute (const std::string& key) const;
protected:
//    bool sceneEvent(QEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
protected:
    qreal item_width_ = 100;
    qreal item_height_ = item_width_ * 0.8;
    static constexpr qreal narrow_object_ratio_ = 1.2;
    static constexpr qreal enlarge_object_ratio_ = 2;
    QString type_;
    const QColor &  selected_color () { return  selected_color_ ; }
private:
    const QColor selected_color_ = Qt::red;
    json item_info_;
};
