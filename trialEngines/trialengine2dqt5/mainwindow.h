#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtWidgets/QMainWindow>
#include <QtDeclarative/QSGView>

class MainWindow : public QWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QSGView *view;
};

#endif // MAINWINDOW_H
