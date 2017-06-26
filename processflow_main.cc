#include "processflow_main.h"
#include "ui_processflow_main.h"
#include <QMdiArea>

processflow_main::processflow_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::processflow_main)
{
    ui->setupUi(this);
    ui->mdiarea->setViewMode(QMdiArea::TabbedView);
    init_conn();
}

processflow_main::~processflow_main()
{
    delete ui;
}

QMdiArea *processflow_main::area()
{
    return ui->mdiarea;
}

void processflow_main::init_conn()
{

}
