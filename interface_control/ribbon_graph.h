#pragma once
#include <QFrame>
#include <QToolButton>
#include <vector>

class ribbon_tool : public QToolButton
{
    Q_OBJECT
public:
    template<typename ... Args>
    ribbon_tool (Args && ... p) : QToolButton (std::forward<Args> (p)...) {}
};

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


