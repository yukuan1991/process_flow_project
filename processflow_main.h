#ifndef PROCESSFLOW_MAIN_H
#define PROCESSFLOW_MAIN_H

#include <QMainWindow>

namespace Ui {
class processflow_main;
}

class processflow_main : public QMainWindow
{
    Q_OBJECT

public:
    explicit processflow_main(QWidget *parent = 0);
    ~processflow_main();

private:
    Ui::processflow_main *ui;
};

#endif // PROCESSFLOW_MAIN_H
