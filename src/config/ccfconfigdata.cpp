#include "ccfconfigdata.h"

CcfConfigData::CcfConfigData()
{
}

void CcfConfigData::insert(const QString &newKey, const QString &newValue)
{
    insert(newKey, newValue, false);
}

void CcfConfigData::insert(const QString &newKey, const QString &newValue, bool state)
{
    if (contains(newKey)) {
        replace(newKey, newValue);
        changeState(newKey, state);
    } else {
        m_configKeys.append(newKey);
        m_configValues.append(newValue);
        m_configStates.append(state);
    }
}

void CcfConfigData::changeState(const QString &key, bool wasChanged)
{
    int index = indexOf(key);

    if (index != -1)
        m_configStates.replace(index, wasChanged);
}

void CcfConfigData::changeState(int index, bool wasChanged)
{
    if (isIndexValid(index))
        m_configStates.replace(index, wasChanged);
}

void CcfConfigData::replace(const QString &key, const QString &newValue)
{
    int index = indexOf(key);

    if (index != -1)
        m_configValues.replace(index, newValue);
}

void CcfConfigData::replace(int index, const QString &newValue)
{
    if (isIndexValid(index))
        m_configValues.replace(index, newValue);
}

QString CcfConfigData::key(int index)
{
    if (isIndexValid(index))
        return m_configKeys.at(index);
    else
        return QString();
}

QString CcfConfigData::value(const QString &key)
{
    int index = indexOf(key);

    if (index != -1)
        return m_configValues.at(index);
    else
        return QString();
}

QString CcfConfigData::value(int index)
{
    if (isIndexValid(index))
        return m_configValues.at(index);
    else
        return QString();
}

bool CcfConfigData::state(const QString &key)
{
    int index = indexOf(key);

    if (index != -1)
        return m_configStates.at(index);
    else
        return false;
}

bool CcfConfigData::state(int index)
{
    if (isIndexValid(index))
        return m_configStates.at(index);
    else
        return false;
}

int CcfConfigData::indexOf(const QString &key)
{
    return m_configKeys.indexOf(key);
}

bool CcfConfigData::contains(const QString &key)
{
    if (indexOf(key) != -1)
        return true;
    else
        return false;
}

int CcfConfigData::size()
{
    return m_configKeys.size();
}

QString CcfConfigData::toString()
{
    QString result;
    for (int i = 0; i < m_configKeys.size(); ++i) {
        result.append(m_configKeys.at(i) + " " + m_configValues.at(i)
                      + ", " + m_configStates.at(i) + "\n");
    }
    return result;
}

bool CcfConfigData::isIndexValid(int indexToCheck)
{
    if ((indexToCheck >= 0) && (indexToCheck < m_configStates.size()))
        return true;
    else
        return false;
}
