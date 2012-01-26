#ifndef CONVERTERFLAGS_H
#define CONVERTERFLAGS_H

#include <QtCore/qobject.h>
#include <QtCore/qchar.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qdir.h>
#include <QtCore/QFlag>
#include <QtCore/QFlags>
#include <qdebug.h>
#include "../../ccferror.h"

class ConverterFlags : public QObject, public CcfError
{
    Q_OBJECT
    Q_FLAGS(Options)

public:
    enum Option
    {
        InputDirectory   = 0x000001,
        OutputDirectory  = 0x000002,
        Suffix           = 0x000004,
        TempResource     = 0x000008,
        Force            = 0x000010,
        Skip             = 0x000020,
        NoQrc            = 0x000040,
        NoCore           = 0x000080,
        NoImg            = 0x000100,
        NoSkin           = 0x000200
    };
    Q_DECLARE_FLAGS(Options, Option)

    explicit ConverterFlags(QObject *parent = 0);

    // Setters:
    bool setFlags(const QStringList &appArguments);

    // Getters:
    Options flags() const;
    QString inputDirectory() const;
    QString outputDirectory() const;
    QString suffix() const;
    QStringList skip() const;

private:
    QString checkPath(const QString &pathToCheck);
    QStringList parseSkipCommand(const QString &commandText);

    Options m_flags;
    QString m_inputDirectory;
    QString m_outputDirectory;
    QString m_suffix;
    QStringList m_skip;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ConverterFlags::Options)

#endif // CONVERTERFLAGS_H
