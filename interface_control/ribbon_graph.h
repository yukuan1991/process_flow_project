#pragma once
#include <QFrame>
#include <QToolButton>
#include "ribbon.h"

class ribbon_graph : public QFrame
{
    Q_OBJECT
public:
    ribbon_graph(QWidget *parent = 0);

    ~ribbon_graph()
    {}

//protected:
//    void paintEvent(QPaintEvent*) override;

private:
};


