#ifndef UIIMAGEPROVIDER_H
#define UIIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QObject>

class UIImageProvider : public QQuickImageProvider
{

public:
    UIImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
private:
    QMap<QString,QImage> images;
};

#endif // UIIMAGEPROVIDER_H
