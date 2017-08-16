#include "canvas_body.h"

#include <QDebug>

canvas_body::~canvas_body()
{
}

bool canvas_body::init()
{
    this->setScene(scene_.get());
    return true;
}

void canvas_body::on_selection_changed()
{
    const auto items = scene_->selectedItems ();
    if (items.size () != 1)
    {
        emit selection_changed (nullptr);
    }
    else
    {
        emit selection_changed (items.at (0));
    }
}
