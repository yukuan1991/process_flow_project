#include "draw.h"

draw::draw(QWidget *parent)
    : QFrame(parent)
{
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Raised);
}

void draw::reset_status()
{
    for (auto it : buttons_)
    {
        it->setChecked(false);
    }
    emit graph_clicked("");
}


void draw::on_button_pressed()
{
    auto button = dynamic_cast<ribbon_tool*>(sender ()); //返回一个指向信号发送方的指针
    if (button == nullptr)
    {
        return;
    }
    auto name = button->objectName();

    if (name.isEmpty())
    {
        return;
    }

    ///
    if (button->isChecked())
    {
        for (auto it : buttons_)
        {
            if (it != button)
            {
                it->setChecked (false);
            }
        }
        emit graph_clicked(name);
    }
    else
    {
        emit graph_clicked("");
    }
}
