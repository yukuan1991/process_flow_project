#pragma once
#include <QFrame>
#include <QToolButton>
#include "ribbon.h"

class ribbon_graph : public QFrame
{
    Q_OBJECT
signals:
    void graph_clicked (const QString&);
public:
    ribbon_graph(QWidget *parent = 0);

    ~ribbon_graph()
    {}

//protected:
//    void paintEvent(QPaintEvent*) override;

private:
    void on_button_pressed();
};


