#include "processflow_main.h"
#include "ui_processflow_main.h"

processflow_main::processflow_main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::processflow_main)
{
    ui->setupUi(this);
}

processflow_main::~processflow_main()
{
    delete ui;
}
