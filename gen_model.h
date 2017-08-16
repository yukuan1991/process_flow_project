#pragma once
#include <QAbstractTableModel>
#include <base/lang/not_null.h>


class gen_model : public QAbstractTableModel
{
public:
    gen_model (QObject * parent = null);
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const override;
    bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::DisplayRole) override;
    int columnCount (const QModelIndex & parent = QModelIndex ()) const override;
    int rowCount (const QModelIndex & parent = QModelIndex ()) const override;
    void set_row (int row);
    Qt::ItemFlags flags (const QModelIndex & index) const override;

    QVariantMap dump () const;
private:
    QStringList headers_;
    mutable QVariantMap data_;
    int rows_ = 0;
};

