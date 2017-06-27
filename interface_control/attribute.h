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
//    static std::unique_ptr<attribute> make (std::vector<std::pair<std::string, std::string>> data,
//                                     QWidget *parent = nullptr);
    static std::unique_ptr<attribute> make (nlohmann::json data, QWidget* parent = nullptr);
    const auto & changed_data () { return changed_data_; }
    void keyPressEvent(QKeyEvent *event);
private:
//    attribute (std::vector<std::pair<std::string, std::string>> data, QWidget *parent);
    explicit attribute(nlohmann::json data, QWidget* parent);
    bool init ();
    void set_changed_text (const QString &text);
    void clear_edit_text ();
private:
//    const std::vector<std::pair<std::string, std::string>> data_;
    const nlohmann::json data_;
    std::vector<std::pair<std::string, std::string>> changed_data_;
};

