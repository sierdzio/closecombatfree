#ifndef CCFQMLBASEMAP_H
#define CCFQMLBASEMAP_H

#include <QObject>
#include <QPainter>
#include <QQuickPaintedItem>

#include "logic/ccfobjectbase.h"

/*!
  \ingroup CloseCombatFree
  @{
 */

/*!
  Base class for in-game Map objects.
  */
class CcfQmlBaseMap : public QQuickPaintedItem // TODO: consider using QQuickPaintedItem!
{
    Q_OBJECT
public:

    /*!
      Stores background image. Preferably in PNG, although all
      QML formats are supported.
      */
    Q_PROPERTY(QString backgroundImage READ getBackgroundImage WRITE setBackgroundImage NOTIFY backgroundImageChanged)

    /*!
      Holds path to hipsometric map of the terrain.
      Terrain defined by intensity of color in the image.
      Should be the same size as backgroundImage. If it's not,
      it will be stretched to match background.
      */
    Q_PROPERTY(QString hipsometricImage READ getHipsometricImage WRITE setHipsometricImage NOTIFY hipsometricImageChanged)

    /*!
      Defines global opacity value.
      Useful for temporarily revealing hidden props
      (like, water objects etc.)
      */
    Q_PROPERTY(qreal propOpacity READ getPropOpacity WRITE setPropOpacity NOTIFY propOpacityChanged)

    explicit CcfQmlBaseMap(QObject *parent = 0);
    void toggleBackgroundImage();
    void paint(QPainter *painter);

signals:
    void backgroundImageChanged() const;
    void hipsometricImageChanged() const;
    void propOpacityChanged() const;

public slots:
    QString getBackgroundImage() const;
    QString getHipsometricImage() const;
    qreal getPropOpacity() const;

    void setBackgroundImage(const QString &path);
    void setHipsometricImage(const QString &path);
    void setPropOpacity(qreal propOpacity);

private:
    bool mHipsometricMapInFront;
    QString mBackgroundImage;
    QString mHipsometricImage;
    qreal mPropOpacity;
};

/*! @}*/

#endif // CCFQMLBASEMAP_H
