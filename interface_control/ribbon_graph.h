#pragma once
#include <QFrame>
#include <QToolButton>
#include "ribbon.h"
#include <vector>

class ribbon_graph : public QFrame
{
    Q_OBJECT
signals:
    void graph_clicked (const QString&);
public:
    ribbon_graph(QWidget *parent = 0);

    ~ribbon_graph()
    {}

    void reset_status ();
//protected:
//    void paintEvent(QPaintEvent*) override;

private:
    void on_button_pressed();
private:
    std::vector<ribbon_tool*> buttons_;
};


