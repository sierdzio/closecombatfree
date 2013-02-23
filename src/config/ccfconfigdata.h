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

#ifndef CCFCONFIGDATA_H
#define CCFCONFIGDATA_H

#include <QVector>
#include <QString>

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

/*!
 \brief The CcfConfigData class acts as a Data Transfer Object, storing current game config.

 CcfConfigData can be used to easily pass around all the information regarding current
 configuration, or parts of it. All entries have: an index, a string key, a string value,
 and a boolean state (useful for making, which property value was changed).

 I would recommend using this class as a pointer, since it can store quite a lot
 of data, and references would not be very optimal in that case. Also, this class
 is NOT implicitly shared.

 It's basically a wrapper around what used to be a huge and complicated QMap. Now
 the way the data is stored is an implementation detail, only the public API is
 important. It's designed to be very similar to that of Qt's container classes, and
 sports a large number of convenience methods.
 */
class CcfConfigData
{
public:
    explicit CcfConfigData();

    void insert(const QString &newKey, const QString &newValue);
    void insert(const QString &newKey, const QString &newValue, bool state);
    void changeState(const QString &key, bool state);
    void changeState(int index, bool state);
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
    QVector<QString> mConfigKeys;
    QVector<QString> mConfigValues;
    // TODO: change bool into an enum, to get error capabilities
    QVector<bool>    mConfigStates;
};

/*! @}*/

#endif // CCFCONFIGDATA_H
