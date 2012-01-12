#include "converterflags.h"

ConverterFlags::ConverterFlags(QObject *parent) :
    QObject(parent), CcfError()
{
//    m_flags &= ConverterFlags::Suffix;
//    m_suffix = "_old";
}

bool ConverterFlags::setFlags(const QStringList &appArguments)
{
    for (int i = 0; i < appArguments.length(); ++i) {
        if (isErrorState()) {
            return false;
        }

        QString current = appArguments.at(i);

        // This is a flag.
        if (current.startsWith(QChar('-')) || current.startsWith(QLatin1String("--"))) {
            if (current == "-o") {
                m_flags |= ConverterFlags::OutputDirectory;
                m_outputDirectory = checkPath(appArguments.at(++i));
            } else if (current == "-i") {
                m_flags |= ConverterFlags::InputDirectory;
                m_inputDirectory = checkPath(appArguments.at(++i));
            } else if ((current == "-s") || (current == "--suffix")) {
                m_flags |= ConverterFlags::Suffix;
                m_suffix = appArguments.at(++i);
            } else if (current == "--skip") {
                m_flags |= ConverterFlags::Skip;
                m_skip = parseSkipCommand(appArguments.at(++i));
//            } else if ((current == "-n") || (current == "--no-suffix")) {
//                m_flags ^= ConverterFlags::Suffix;
//                m_suffix = QString();
//            } else if ((current == "-d") || (current == "--no-old-versions")) {
//                m_flags &= ConverterFlags::NoOldVersions;
            } else if ((current == "-f") || (current == "--force")) {
                m_flags |= ConverterFlags::Force;
            } else if ((current == "-t") || (current == "--temp-resource")) {
                m_flags |= ConverterFlags::TempResource;
            } else if (current == "--no-qrc") {
                m_flags |= ConverterFlags::NoQrc;
            } else if (current == "--no-core") {
                m_flags |= ConverterFlags::NoCore;
            } else if (current == "--no-img") {
                m_flags |= ConverterFlags::NoImg;
            } else if (current == "--no-skin") {
                m_flags |= ConverterFlags::NoSkin;
            } else {
                enterErrorState(current + " is not a valid flag.");
            }
        } else {
            enterErrorState(current + " is not a valid flag.");
        }
    }

    return !isErrorState();
}

ConverterFlags::Options ConverterFlags::flags() const
{
    return m_flags;
}

QString ConverterFlags::inputDirectory() const
{
    return m_inputDirectory;
}

QString ConverterFlags::outputDirectory() const
{
    return m_outputDirectory;
}

QString ConverterFlags::suffix() const
{
    return m_suffix;
}

QStringList ConverterFlags::skip() const
{
    return m_skip;
}

QString ConverterFlags::checkPath(const QString &pathToCheck)
{
    // TODO: Need to add true checking here!
    return QDir::fromNativeSeparators(pathToCheck);
}

QStringList ConverterFlags::parseSkipCommand(const QString &commandText)
{
    // TODO: check for wrong entries!
    return commandText.split(QChar(','));
}
