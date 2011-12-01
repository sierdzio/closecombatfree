#ifndef CCFMAIN_H
#define CCFMAIN_H

#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QDebug>
#include "ccfconfig.h"

class CcfMain : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit CcfMain(QWidget *parent = 0);

public slots:
    void quit();

private:
    CcfConfig *configuration;
};

#endif // CCFMAIN_H
