#ifndef GEN_DLG_H
#define GEN_DLG_H

#include <QDialog>
#include <memory>
#include <base/lang/not_null.h>

class gen_model;
class gen_delegate;

namespace Ui {
class gen_dlg;
}

class gen_dlg : public QDialog
{
    Q_OBJECT
public:
    explicit gen_dlg (QWidget *parent = null);
    ~gen_dlg () override;
    QVariantList dump() const;
    void load (const QVariantList & data);
private:
    Ui::gen_dlg *ui;
    std::unique_ptr<gen_model>  model_;
    std::unique_ptr<gen_delegate>  delegate_;
};

#endif // GEN_DLG_H
