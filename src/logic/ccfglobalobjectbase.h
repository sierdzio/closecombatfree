#ifndef CCFGLOBALOBJECTBASE_H
#define CCFGLOBALOBJECTBASE_H

#include <QObject>

/*
  Before you complain: yes, we know macros are the root of considerate amount
  of evil. Those few are extra useful, though, and make our code shiny all over
  the place. Take a look at what some parts of the code would look like without
  them. Once you do that, and then spend a while contemplating the universe,
  your life, this code, and everything, you can start complaining.

  Not that anyone will listen, anyway.
*/
#define set setProperty
#define get property
#define getReal(arg) property(arg).toReal()
#define getInt(arg) property(arg).toInt()
#define getString(arg) property(arg).toString()
#define getBool(arg) property(arg).toBool()

///*!
//  Invokes a method (has to be defined in MOC). Shortened syntax of metaObject::invokeMehtod().
//  */
//static bool invoke(QObject *obj, const char *member,
//                   QGenericArgument val0 = QGenericArgument(0),
//                   QGenericArgument val1 = QGenericArgument(),
//                   QGenericArgument val2 = QGenericArgument(),
//                   QGenericArgument val3 = QGenericArgument(),
//                   QGenericArgument val4 = QGenericArgument(),
//                   QGenericArgument val5 = QGenericArgument(),
//                   QGenericArgument val6 = QGenericArgument(),
//                   QGenericArgument val7 = QGenericArgument(),
//                   QGenericArgument val8 = QGenericArgument(),
//                   QGenericArgument val9 = QGenericArgument())
//{
//    return obj->metaObject()->invokeMethod(obj, member, val0, val1, val2, val3,
//                                           val4, val5, val6, val7, val8, val9);
//}

#endif // CCFGLOBALOBJECTBASE_H
