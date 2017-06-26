#pragma once
#include <QWidget>

class QMdiArea;

namespace Ui {
class processflow_main;
}

class processflow_main final : public QWidget
{
    Q_OBJECT

public:
    explicit processflow_main(QWidget *parent = 0);
    ~processflow_main();
    QMdiArea* area();
private:
    void init_conn();
private:
    Ui::processflow_main *ui;
};

