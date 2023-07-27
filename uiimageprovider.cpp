#include "uiimageprovider.h"
#include<QFile>
#include<QPixmap>


UIImageProvider::UIImageProvider():QQuickImageProvider{QQuickImageProvider::Pixmap}
{

}


QPixmap UIImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize){


       auto iter=images.find(id);
       if(iter==images.end()){
           qDebug()<<"error finding image:"<<id;
       }
       int width = 100;
       int height = 50;

       if (size)
          *size = QSize(width, height);
       QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                      requestedSize.height() > 0 ? requestedSize.height() : height);
       //pixmap.fill(QColor(id).rgba());
       pixmap=QPixmap::fromImage(iter.value()
                                 //.scaled(pixmap.size(),Qt::KeepAspectRatio)
                                 );

       return pixmap;

}
