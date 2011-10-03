#include "mainwindow.h"

MainWindow::MainWindow()//QWidget *parent)
//    : QWindow(parent)
{
    view = new QSGView(this);
    view->setSource(QUrl::fromLocalFile("main.qml"));

//    this->setCentralWidget(view);
    view->show();
}

MainWindow::~MainWindow()
{
    delete view;
}
