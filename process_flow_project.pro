#-------------------------------------------------
#
# Project created by QtCreator 2017-06-26T08:57:42
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG -= c++11
QMAKE_CXXFLAGS += -std=c++1z

RC_ICONS = png/myappico.ico

TARGET = process_flow_project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cc\
    interface_control/ribbon.cc \
    processflow_main.cc \
    interface_control/canvas_scene.cc \
    interface_control/canvas_body.cc \
    interface_control/canvas_view.cc \
    interface_control/about_us_dlg.cc \
    item/raw_material.cc \
    item/item.cc \
    item/machining.cc \
    item/checkout.cc \
    item/finished_product.cc \
    item/broken_line.cc \
    item/straight_line.cc \
    interface_control/attribute.cc \
    interface_control/draw.cc \
    interface_control/draw_graph.cc \
    interface_control/draw_line.cc

HEADERS  += \
    interface_control/ribbon.h \
    processflow_main.h \
    interface_control/canvas_scene.h \
    interface_control/canvas_body.h \
    interface_control/canvas_view.h \
    interface_control/about_us_dlg.h \
    item/raw_material.h \
    item/item.h \
    item/machining.h \
    item/checkout.h \
    item/finished_product.h \
    item/broken_line.h \
    item/straight_line.h \
    interface_control/attribute.h \
    interface_control/draw.h \
    interface_control/ribbon_tool.hpp \
    interface_control/draw_graph.h \
    interface_control/draw_line.h

FORMS    += \
    processflow_main.ui

DISTFILES +=

LIBS += -lboost_locale
LIBS += -liconv
LIBS += -lboost_filesystem
LIBS += -lboost_system
