#ifndef CCFCOMMANDLINEPARSER_H
#define CCFCOMMANDLINEPARSER_H

#include <QtCore/QStringList>

class CcfCommandLineParser
{
public:
    CcfCommandLineParser(const QStringList &args);

    bool isDebug();
    bool wasHelpRequested();
    QString helpMessage();

private:
    void init(const QStringList &args);

    QStringList switches;
};

#endif // CCFCOMMANDLINEPARSER_H
