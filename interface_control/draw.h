#pragma once
#include <QFrame>
#include "ribbon_tool.hpp"

class draw : public QFrame
{
    Q_OBJECT
signals:
    void graph_clicked (const QString&);
public:
    draw(QWidget *parent = 0);
    virtual ~draw() {}
    void reset_status ();
protected:
    void on_button_pressed();
protected:
    std::vector<ribbon_tool*> buttons_;
};

