#ifndef CCFOBJECTBASE_H
#define CCFOBJECTBASE_H

#include <QString>
#include <QObject>
#include <QVariant>
#include <QQuickItem>

/*!
  \defgroup CloseCombatFree Game code
  @{
 */

/*
  //! [CustomMacroNotice]
  Before you complain: yes, we know macros are the root of considerate amount
  of evil. Those few are extra useful, though, and make our code shiny all over
  the place. Take a look at what some parts of the code would look like without
  them. Once you do that, and then spend a while contemplating the universe,
  your life, this code, and everything, you can start complaining.

  Not that anyone will listen, anyway.
  //! [CustomMacroNotice]
*/

/*!
  Shortcut for QObject's setProperty.
  \code
  QObject *obj;
  obj->set("property", value);
  \endcode

  \snippet ccfobjectbase.h CustomMacroNotice
  */
#define set setProperty

/*!
  Shortcut for QObject's property getter.
  \code
  QVariant result;
  QObject *obj;
  result = obj->get("width");
  \endcode
  */
#define get property

/*!
  Shortcut for QObject's property getter, returning value converted to qreal.
  \code
  qreal result;
  QObject *obj;
  result = obj->getReal("width");
  \endcode
  */
#define getReal(arg) property(arg).toReal()

/*!
  Shortcut for QObject's property getter, returning value converted to int.
  \code
  int result;
  QObject *obj;
  result = obj->getInt("width");
  \endcode
  */
#define getInt(arg) property(arg).toInt()

/*!
  Shortcut for QObject's property getter, returning value converted to QString.
  \code
  QString result;
  QObject *obj;
  result = obj->getString("unitName");
  \endcode
  */
#define getString(arg) property(arg).toString()

/*!
  Shortcut for QObject's property getter, returning value converted to bool.
  \code
  bool result;
  QObject *obj;
  result = obj->getBool("visible");
  \endcode
  */
#define getBool(arg) property(arg).toBool()

class CcfQmlBaseUnit;

/*!
  QQuickItem extension class, used as a base for many QML items in Ccf. Contains
  some convenience defines (set, get, getReal, getInt, getString, getBool) for
  easy property manipulation. Also sports some particularly useful convenience
  methods.
  */
class CcfObjectBase : public QQuickItem
{
    Q_OBJECT

public:
    explicit CcfObjectBase(QQuickItem *parent = 0);
    
    CcfQmlBaseUnit *ccfUnit(QObject *unitObject) const;
    QQuickItem *item(const QString &objectName) const;
    QQuickItem *item(QObject *object);
    QObject *child(const QString &objectName) const;
    bool invoke(QObject *obj, const char *member,
                    QGenericArgument val0 = QGenericArgument(0), QGenericArgument val1 = QGenericArgument(),
                    QGenericArgument val2 = QGenericArgument(), QGenericArgument val3 = QGenericArgument(),
                    QGenericArgument val4 = QGenericArgument(), QGenericArgument val5 = QGenericArgument(),
                    QGenericArgument val6 = QGenericArgument(), QGenericArgument val7 = QGenericArgument(),
                    QGenericArgument val8 = QGenericArgument(), QGenericArgument val9 = QGenericArgument());
    QList<CcfQmlBaseUnit *> objectToUnitList(QObjectList list);
};

/*! @}*/

#endif // CCFOBJECTBASE_H
