#pragma once
#include <QWidget>
#include <QToolButton>
#include "ribbon.h"

class ribbon_graph : public QWidget
{
    Q_OBJECT
public:
    ribbon_graph(QWidget *parent = 0);

    ~ribbon_graph()
    {}

protected:
    void paintEvent(QPaintEvent*) override;

private:
};


