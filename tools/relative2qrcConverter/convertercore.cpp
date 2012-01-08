#include "convertercore.h"
#include "iostream"

ConverterCore::ConverterCore(const QStringList &args, QObject *parent) :
    QObject(parent), CcfError()
{
    helpMode = false;
    if (args.contains("-h") || args.contains("--help")) {
        displayHelp();
        helpMode = true;
    } else {
        flags = new ConverterFlags(this);
        setFlags(args);
    }
}

void ConverterCore::setFlags(const QStringList &args)
{
    bool result = false;

    // If first arg is not a flag - it's probably the app name.
    // In such a case, that arg will be discarded.
    if (!args.at(0).startsWith(QChar('-')) && !args.at(0).startsWith(QLatin1String("--"))) {
        result = flags->setFlags(args.mid(1));
    } else {
        result = flags->setFlags(args);
    }

    if (!result) {
        enterErrorState(flags->errorMessage());
    }
}

void ConverterCore::convert()
{
    if (helpMode)
        return;
}

void ConverterCore::displayHelp()
{
    std::cout << "This help message is still a WIP." << std::endl;
}
