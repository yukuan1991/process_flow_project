#pragma once
#include <QStyledItemDelegate>
#include <base/lang/not_null.h>


class gen_delegate : public QStyledItemDelegate
{
public:
    gen_delegate(QObject * parent = null);
    ~gen_delegate() override;
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData (QWidget * editor, const QModelIndex & index) const override;
    void setModelData (QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const override;
private:
};
