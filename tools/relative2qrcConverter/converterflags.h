#ifndef CONVERTERFLAGS_H
#define CONVERTERFLAGS_H

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include "../../src/ccferror.h"

class ConverterFlags : public QObject, public CcfError
{
    Q_OBJECT
    Q_FLAGS(Options)

public:
    enum Option
    {
        TempResource     = 0x000001,
        NoOldVersions    = 0x000002,
        NoQrc            = 0x000004,
        NoCore           = 0x000008,
        NoImg            = 0x000010,
        NoSkin           = 0x000020
    };
    Q_DECLARE_FLAGS(Options, Option)

    explicit ConverterFlags(QObject *parent = 0);

    // Setters:
    bool setFlagFromString(const QString &flagString);

    // Getters:
    Options flags() const;
    QString inputDirectory() const;
    QString outputDirectory() const;
    QString suffix() const;

private:
    QString m_inputDirectory;
    QString m_outputDirectory;
    QString m_suffix;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ConverterFlags::Options)

#endif // CONVERTERFLAGS_H
