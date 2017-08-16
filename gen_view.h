#pragma once
#include <QTableView>
#include <base/lang/not_null.h>
#include <QStandardItemModel>
#include <memory>

class gen_model;

class gen_view : public QTableView
{
public:
    gen_view (QWidget * parent = null);
    ~gen_view () override;
private:
    std::unique_ptr<gen_model> model_;
};

