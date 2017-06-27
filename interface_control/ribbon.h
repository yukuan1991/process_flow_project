#pragma once
#include <QTabWidget>
#include <base/lang/not_null.h>
#include <memory>
#include <QToolButton>
#include <QPushButton>
#include <QTabBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QStyleOption>
#include <QLineEdit>
#include <QPainter>
#include <QMenu>

class ribbon : public QTabWidget
{
    Q_OBJECT
signals:
    void create_new ();
    void open ();
    void print ();
    void save ();
    void save_as ();
    void quit ();

    void help();

    void graph_clicked(const QString &);
public:
    ribbon (QWidget * parent = nullptr);
private:
    static std::unique_ptr<QToolButton> make_button (const QPixmap & icon, const QString & text);
    void setup_ui ();
    void setup_menu ();
    std::unique_ptr<QWidget> ui_edit ();
    std::unique_ptr<QWidget> ui_window ();
    std::unique_ptr<QWidget> ui_help ();
};

class ribbon_button : public QPushButton
{
    Q_OBJECT
public:
    template<typename ... Args>
    ribbon_button (Args && ... p) : QPushButton(std::forward<Args> (p)...) {}
};

class ribbon_menu : public QMenu
{
    Q_OBJECT
public:
    template<typename ... Args>
    ribbon_menu (Args && ... p) : QMenu (std::forward<Args> (p)...) {}
};

class ribbon_tool : public QToolButton
{
    Q_OBJECT
public:
    template<typename ... Args>
    ribbon_tool (Args && ... p) : QToolButton (std::forward<Args> (p)...) {}
};

class ribbon_bar : public QTabBar
{
    Q_OBJECT
public:
    ribbon_bar (QWidget * parent = nullptr) : QTabBar (parent) {}
};

class ribbon_menu_item : public QWidget
{
    Q_OBJECT
public:
    ribbon_menu_item(const QPixmap& pix, const QString& text, QWidget *parent = 0)
        :QWidget (parent)
    {
        QLabel* label_icon = new QLabel(this);
        label_icon->setFixedSize(32, 32);
        label_icon->setScaledContents(true);
        label_icon->setPixmap(pix);

        QLabel* label_text = new QLabel(text,this);
        label_text->setAlignment (Qt::AlignCenter);

        QHBoxLayout* layout = new QHBoxLayout;
        layout->setContentsMargins(1, 1, 1, 1);
        layout->setSpacing(1);
        layout->addWidget(label_icon);
        layout->addWidget(label_text);
        setLayout(layout);

        setFixedWidth(120);
    }
    ~ribbon_menu_item()
    {}

protected:
    void paintEvent(QPaintEvent*) override {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

private:
};



