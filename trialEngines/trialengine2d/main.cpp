#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/trialengine2d/main.qml"));
    viewer->showExpanded();

    return app->exec();
}
