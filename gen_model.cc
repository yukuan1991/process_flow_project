#include "gen_model.h"
#include <assert.h>
#include <base/lang/scope.hpp>

gen_model::gen_model(QObject *parent)
    :QAbstractTableModel (parent)
{
    headers_ << "名称" << "类型" << "后续操作序列号";
}

QVariant gen_model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            return headers_.at (section);
        }
        else
        {
            return section + 1;
        }
    }
    else
    {
        return {};
    }
}

QVariant gen_model::data(const QModelIndex &index, int role) const
{
    if (not index.isValid ())
    {
        return {};
    }

    const auto header = headerData (index.column (), Qt::Horizontal, Qt::DisplayRole).toString ();
    const auto list = data_ [header].toList ();
    if (role != Qt::DisplayRole)
    {
        return {};
    }

    if (list.empty ())
    {
        return {};
    }

    assert (list.size () == rowCount ());
    assert (index.row () < rowCount ());

    return list.at (index.row ());
}

bool gen_model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (not index.isValid ())
    {
        return {};
    }

    const auto header = headerData (index.column (), Qt::Horizontal, Qt::DisplayRole).toString ();
    auto list = data_ [header].toList ();
    if (role != Qt::EditRole)
    {
        return false;
    }

    if (list.empty ())
    {
        return false;
    }

    assert (list.size () == rowCount ());
    assert (index.row () < rowCount () and index.row () >= 0);

    list[index.row ()] = value;
    data_ [header] = list;

    emit dataChanged (index, index);

    return true;
}

int gen_model::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED (parent);
    return headers_.size ();
}

int gen_model::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    return rows_;
}

void gen_model::set_row(int row)
{
    beginResetModel ();
    SCOPE_EXIT { endResetModel (); };
    SCOPE_EXIT { rows_ = row; };

    for (auto it : headers_)
    {
        auto vec = data_ [it].toList ().toVector ();
        vec.resize (row);
        data_ [it] = QVariantList::fromVector (vec);
    }
}

Qt::ItemFlags gen_model::flags (const QModelIndex &index) const
{
    return QAbstractTableModel::flags (index) | Qt::ItemIsEditable;
}

QVariantMap gen_model::dump() const
{
    return data_;
}
