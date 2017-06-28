#include "attribute.h"
#include <QGridLayout>///采用表格布局
#include <iterator>
#include <QLabel>
#include <assert.h>
#include <QLineEdit>
#include <algorithm>
#include <QPushButton>
#include <QSpacerItem>
#include <QKeyEvent>

using std::make_unique;
using std::unique_ptr;
using std::vector;
using std::string;
using std::pair;
using std::find_if;
using std::begin;
using std::end;


attribute* attribute::make(nlohmann::json data, QWidget *parent)
{
    auto ret = new attribute(data, parent);
    if (ret == nullptr or !ret->init())
    {
        return nullptr;
    }

    return ret;
}

attribute::attribute(QWidget *parent)
    : QWidget(parent)
{

}

void attribute::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter or event->key() == Qt::Key_Return)
    {
        emit submit();
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

attribute::attribute(nlohmann::json data, QWidget *parent)
    : QWidget (parent),
    data_(std::move(data))
{

}

bool attribute::init()
{
        auto layout = new QGridLayout(this);

        int i = 0;
        for (auto & it : data_)
        {
            std::string key = it.begin().key();
            std::string value = it.begin().value();
            auto label = make_unique<QLabel> (key.data(), this);
            auto edit = make_unique<QLineEdit> (value.data() , this);
            edit->setObjectName (key.data());

            connect (edit.get (), &QLineEdit::textChanged, this, &attribute::set_changed_text);
            connect (this, &attribute::clear, edit.get (), &QLineEdit::clear);
//            connect(edit.get(), &QLineEdit::textChanged, [this, key = key] (auto&& text)
//            {
//                changed_data_ [key] = text.toStdString ();
//            });
            layout->addWidget (label.release (), i, 0);
            layout->addWidget (edit.release (), i, 1);
            i ++;
        }

        auto button_submit = std::make_unique<QPushButton>("提交", this);
        connect (button_submit.get (), &QPushButton::clicked, this, &attribute::submit);
        layout->addWidget(button_submit.release(), i, 1);
        i ++;

        auto button_clear = std::make_unique<QPushButton>("清空", this);
        connect (button_clear.get(), &QPushButton::clicked, this, &attribute::clear_edit_text);
        layout->addWidget(button_clear.release(), i, 1);
        i++;

        layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), i, 1);

        return true;
}



void attribute::set_changed_text(const QString &text)
{
    auto edit = dynamic_cast<QLineEdit*> (sender ()); assert (edit);
    const auto key = edit->objectName ().toStdString ();
    auto value = text.toStdString ();
    auto found = find_if (begin (changed_data_), end (changed_data_), [&] (auto && it)
    {
        return it.first == key;
    });

    if (found == end (changed_data_))
    {
        changed_data_.emplace_back (std::move (key), std::move (value));
    }
    else
    {
        found->second = std::move (value);
    }
}

void attribute::clear_edit_text()
{
    for (auto & it : changed_data_)
    {
        it.second = {};
    }
    emit clear();
}

