#ifndef CCFGAMEMANAGER_H
#define CCFGAMEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDeclarativeListReference>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "ccferror.h"

class CcfGameManager : public QObject, public CcfError
{
    Q_OBJECT
public:
    explicit CcfGameManager(QObject *parent = 0);
    Q_INVOKABLE QStringList scenariosList();
    Q_INVOKABLE QString scenarioPath(int index);
    Q_INVOKABLE void saveGame(const QDeclarativeListReference &unitsList,
                              const QString &mapFile,
                              const QString &saveFileName = "saves/save1.qml");
    Q_INVOKABLE QStringList savedGamesList();
    
private:
    QString addSavePropertyIfExists(const QObject *object,
                                    const QString &propertyName,
                                    bool useQuotes = false);

    QStringList m_scenariosList;
    QString tab;
};

#endif // CCFGAMEMANAGER_H
