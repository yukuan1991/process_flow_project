#pragma once

#include <QWidget>
#include <memory>
#include "json.hpp"


class attribute : public QWidget
{
    Q_OBJECT
signals:
    void submit ();
    void clear ();
public:
    static attribute* make (nlohmann::json data, QWidget* parent = nullptr);
    const auto & changed_data () { return changed_data_; }
    void keyPressEvent(QKeyEvent *event);
private:
    explicit attribute(nlohmann::json data, QWidget* parent);
    bool init ();
    void set_changed_text (const QString &text);
    void clear_edit_text ();
private:
    const nlohmann::json data_;
    std::vector<std::pair<std::string, std::string>> changed_data_;
};

