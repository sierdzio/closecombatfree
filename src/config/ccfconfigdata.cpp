/****************************************************************************
** Close Combat Free is a free, easily moddable CC-like game.
** Copyright (C) 2011 Tomasz Siekierda
** Email: sierdzio@gmail.com, website: http://www.sierdzio.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program (in doc/ directory).
** If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
****************************************************************************/

#include "ccfconfigdata.h"

/*!
  Just a stnadard constructor. Does not do anything fancy.
  */
CcfConfigData::CcfConfigData()
{
}

/*!
  Inserts a \a newValue under \a newKey. This will set state for new entry to false.

  If an entry with given key already exists, it will be replaced with new one.

  \sa insert, replace
  */
void CcfConfigData::insert(const QString &newKey, const QString &newValue)
{
    insert(newKey, newValue, false);
}

/*!
  Inserts a \a newValue under \a newKey, and sets new entry's state to \a state.

  If an entry with given key already exists, it will be replaced with new one.

  \sa insert, replace
  */
void CcfConfigData::insert(const QString &newKey, const QString &newValue, bool state)
{
    if (contains(newKey)) {
        replace(newKey, newValue);
        changeState(newKey, state);
    } else {
        mConfigKeys.append(newKey);
        mConfigValues.append(newValue);
        mConfigStates.append(state);
    }
}

/*!
  Changes \a state of an entry identified by \a key.
  */
void CcfConfigData::changeState(const QString &key, bool state)
{
    int index = indexOf(key);

    if (index != -1)
        mConfigStates.replace(index, state);
}

/*!
  Changes \a state of an entry identified by \a index.
  */
void CcfConfigData::changeState(int index, bool state)
{
    if (isIndexValid(index))
        mConfigStates.replace(index, state);
}

/*!
  Replaces the value stored at \a key with \a newValue.

  Warning - state of the old entry is preserved!

  \sa insert
  */
void CcfConfigData::replace(const QString &key, const QString &newValue)
{
    int index = indexOf(key);

    if (index != -1)
        mConfigValues.replace(index, newValue);
}

/*!
  Replaces the value stored at \a index with \a newValue.

  Warning - state of the old entry is preserved!

  \sa insert
  */
void CcfConfigData::replace(int index, const QString &newValue)
{
    if (isIndexValid(index))
        mConfigValues.replace(index, newValue);
}

/*!
  Returns the key for a given \a index.

  \sa value, state, indexOf
  */
QString CcfConfigData::key(int index)
{
    if (isIndexValid(index))
        return mConfigKeys.at(index);
    else
        return QString();
}

/*!
  Returns the value of an entry with given \a key.

  \sa key, state, indexOf
  */
QString CcfConfigData::value(const QString &key)
{
    int index = indexOf(key);

    if (index != -1)
        return mConfigValues.at(index);
    else
        return QString();
}

/*!
  Returns the value of an entry with given \a index.

  \sa key, state, indexOf
  */
QString CcfConfigData::value(int index)
{
    if (isIndexValid(index))
        return mConfigValues.at(index);
    else
        return QString();
}

/*!
  Returns the state of an entry with given \a key.

  \sa key, value, indexOf
  */
bool CcfConfigData::state(const QString &key)
{
    int index = indexOf(key);

    if (index != -1)
        return mConfigStates.at(index);
    else
        return false;
}

/*!
  Returns the state of an entry with given \a index.

  \sa key, value, indexOf
  */
bool CcfConfigData::state(int index)
{
    if (isIndexValid(index))
        return mConfigStates.at(index);
    else
        return false;
}

/*!
  Returns the index of a given \a key, or a -1 if key is not present in config.

  \sa key, value, state, contains
  */
int CcfConfigData::indexOf(const QString &key)
{
    return mConfigKeys.indexOf(key);
}

/*!
  Returns true if the config contains a given \a key.

  \sa indexOf, key, value, state
  */
bool CcfConfigData::contains(const QString &key)
{
    if (indexOf(key) != -1)
        return true;
    else
        return false;
}

/*!
  Returns the number of entries in config.
  */
int CcfConfigData::size()
{
    return mConfigKeys.size();
}

/*!
  Returns a nice summary of all the contents of the configuration.

  Format: key value, state<new line>
  */
QString CcfConfigData::toString()
{
    QString result;
    for (int i = 0; i < mConfigKeys.size(); ++i) {
        result.append(mConfigKeys.at(i) + " " + mConfigValues.at(i)
                      + ", " + mConfigStates.at(i) + "\n");
    }
    return result;
}

/*!
  Returns true if the given index is valid (bigger or equal to 0, smaller that config size).
  */
bool CcfConfigData::isIndexValid(int indexToCheck)
{
    if ((indexToCheck >= 0) && (indexToCheck < mConfigStates.size()))
        return true;
    else
        return false;
}
