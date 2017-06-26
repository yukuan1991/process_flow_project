﻿#include "processflow_main.h"
#include <QApplication>
#include "interface_control/ribbon.h"
#include <variant>
#include <memory>
#include <QFile>
#include <base/io/file/file.hpp>
#include <QComboBox>
//#include <boost/filesystem.hpp>
#include <boost/range/adaptors.hpp>
#include <QPixmap>
#include <QStyle>
#include <QIcon>
#include <QMdiArea>
#include <QWidget>
#include <QMdiSubWindow>


//void set_style ()
//{
//    using namespace boost::filesystem;

//    auto rng = boost::make_iterator_range (directory_iterator ("."), directory_iterator ());

//    std::string qss;
//    for (auto & it : rng)
//    {
//        if (it.path ().extension ().string () == ".css")
//        {
//            auto res = file::read_all (it.path ().string ().data ());
//            if (res)
//            {
//                qss += res.value ();
//            }
//        }
//    }

//    qApp->setStyleSheet (QString::fromStdString (qss));
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qssfile("style.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    qApp->setStyleSheet(qss);

    processflow_main w;
    w.resize(1366, 768);
    w.show();

    return a.exec();
}
