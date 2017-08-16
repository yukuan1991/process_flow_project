#include "gen_model.h"
#include <assert.h>
#include <base/lang/scope.hpp>

gen_model::gen_model(QObject *parent)
    :QAbstractTableModel (parent)
{
    headers_ << "名称" << "类型" << "前置操作序列号";
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

    if (index.row () >= rowCount () or index.column () >= columnCount ())
    {
        return {};
    }

    if (role != Qt::DisplayRole)
    {
        return {};
    }

    const auto header = headerData (index.column (), Qt::Horizontal, Qt::DisplayRole).toString ();

    return data_.at (index.row ()).toMap () [header];
}

bool gen_model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (not index.isValid ())
    {
        return {};
    }

    if (role != Qt::EditRole)
    {
        return false;
    }
    const auto header = headerData (index.column (), Qt::Horizontal, Qt::DisplayRole).toString ();

    auto map = data_.at (index.row ()).toMap ();
    map [header] = value;
    data_[index.row ()] = map;

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
    return data_.size ();
}

void gen_model::set_row(int row)
{
    beginResetModel ();
    SCOPE_EXIT { endResetModel (); };

    auto vec = data_.toVector ();
    vec.resize (row);
    data_ = QVariantList::fromVector (vec);
}

Qt::ItemFlags gen_model::flags (const QModelIndex &index) const
{
    return QAbstractTableModel::flags (index) | Qt::ItemIsEditable;
}

QVariantList gen_model::dump() const
{
    return data_;
}

void gen_model::load(const QVariantList &data)
{
    beginResetModel ();
    SCOPE_EXIT { endResetModel (); };

    data_ = data;
}
