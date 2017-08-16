#include "gen_delegate.h"
#include "gen_model.h"
#include <QComboBox>
#include <QLineEdit>
#include <assert.h>


gen_delegate::gen_delegate(QObject *parent)
    :QStyledItemDelegate (parent)
{

}

gen_delegate::~gen_delegate()
{

}

QWidget *gen_delegate::createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED (option);
    const auto model = index.model ();
    const auto header = model->headerData (index.column (), Qt::Horizontal, Qt::DisplayRole).toString ();

    if (header == "名称" or header == "后续操作序列号")
    {
        return new QLineEdit (parent);
    }
    else if (header == "类型")
    {
        return new QComboBox (parent);
    }

    return null;
}

void gen_delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const auto model = index.model ();
    const auto header = model->headerData (index.column (), Qt::Horizontal, Qt::DisplayRole).toString ();

    if (header == "名称" or header == "后续操作序列号")
    {
        auto line_editor = dynamic_cast<QLineEdit*> (editor); assert (line_editor);
        line_editor->setText (index.data (Qt::DisplayRole).toString ());
    }
    else if (header == "类型")
    {
        auto combo = dynamic_cast<QComboBox*> (editor); assert (combo);
        QStringList items;
        items << "原材料" << "加工" << "检验" << "加工检验" << "产成品";
        combo->addItems (items);
        combo->setCurrentText (index.data (Qt::DisplayRole).toString ());
    }
}

void gen_delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    const auto header = model->headerData (index.column (), Qt::Horizontal, Qt::DisplayRole).toString ();

    if (header == "名称" or header == "后续操作序列号")
    {
        const auto line_editor = dynamic_cast<QLineEdit*> (editor); assert (line_editor);
        model->setData (index, line_editor->text (), Qt::EditRole);
    }
    else if (header == "类型")
    {
        const auto combo = dynamic_cast<QComboBox*> (editor); assert (combo);
        model->setData (index, combo->currentText (), Qt::EditRole);
    }
}
