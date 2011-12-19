#ifndef CCFMAIN_H
#define CCFMAIN_H

#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QSize>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QDebug>
#include "config/ccfconfig.h"

class CcfMain : public QDeclarativeView, public CcfError
{
    Q_OBJECT
public:
    explicit CcfMain(QWidget *parent = 0);
    bool isConfigMaximised();
    void resizeView(QSize newSize);

public slots:
    void quit();

private:
    bool initConfiguration();

    CcfConfig *configuration;
    QString pwd;
};

#endif // CCFMAIN_H
