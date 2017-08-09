#pragma once
#include <QFrame>
#include "ribbon_tool.hpp"

class draw : public QFrame
{
    Q_OBJECT
signals:
    void graph_clicked (const QString&);
    void set_enabled(bool);
public:
    draw(QWidget *parent = 0);
    virtual ~draw() {}
    QString status() { return status_; }
    void reset_status ();
    const std::vector<ribbon_tool*>& buttons() { return buttons_; }
protected:
    void on_button_pressed();
protected:
    std::vector<ribbon_tool*> buttons_;
    QString status_;
};

