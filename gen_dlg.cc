#include "gen_dlg.h"
#include "ui_gen_dlg.h"
#include "gen_model.h"
#include "gen_delegate.h"
#include <QIntValidator>

gen_dlg::gen_dlg(QWidget *parent)
    :QDialog (parent)
    ,ui(new Ui::gen_dlg)
    ,model_ (std::make_unique<gen_model> (this))
    ,delegate_ (std::make_unique<gen_delegate> (this))
{
    ui->setupUi(this);
    ui->view->setModel (model_.get ());


    ui->edit_row->setValidator (new QIntValidator (0, 99999, ui->edit_row));
    ui->view->setItemDelegate (delegate_.get ());

    connect (ui->button_row, &QPushButton::clicked, [model = model_.get (), edit = ui->edit_row] {
        const auto rowCount = edit->text ().toInt ();
        model->set_row (rowCount);
    });

    ui->edit_row->setText ("10");
    ui->button_row->click ();

    resize (500, 700);
}

gen_dlg::~gen_dlg()
{
    delete ui;
}

QVariantMap gen_dlg::dump() const
{
    return model_->dump ();
}
