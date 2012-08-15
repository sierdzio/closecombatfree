#ifndef CCFCONFIGDATA_H
#define CCFCONFIGDATA_H

#include <QtCore/QVector>
#include <QtCore/QString>

class CcfConfigData
{
public:
    explicit CcfConfigData();

    void insert(const QString &newKey, const QString &newValue);
    void insert(const QString &newKey, const QString &newValue, bool state);
    void changeState(const QString &key, bool wasChanged);
    void changeState(int index, bool wasChanged);
    void replace(const QString &key, const QString &newValue);
    void replace(int index, const QString &newValue);

    QString key(int index);
    QString value(const QString &key);
    QString value(int index);
    bool state(const QString &key);
    bool state(int index);
    int indexOf(const QString &key);
    bool contains(const QString &key);
    int size();

    QString toString();
    
private:
    bool isIndexValid(int indexToCheck);

    // Those params are divided to avoid monster QMaps.
    // All indexes are equal.
    QVector<QString> m_configKeys;
    QVector<QString> m_configValues;
    // TODO: change bool into an enum, to get error capabilities
    QVector<bool>    m_configStates;
};

#endif // CCFCONFIGDATA_H
