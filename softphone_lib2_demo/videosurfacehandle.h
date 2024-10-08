#ifndef VideoSurfaceHandle_H
#define VideoSurfaceHandle_H

#include <QObject>
#include<QQuickItem>
#include<QList>

#include <QVideoFrame>
#include<QAbstractVideoSurface>

class VideoSurfaceHandle : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(WindowRole)
    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ getVideoSurface WRITE setVideoSurface)
    Q_PROPERTY(WindowRole windowRole READ getWindowRole WRITE setWindowRole NOTIFY windowRoleChanged())

public:
    VideoSurfaceHandle(QQuickItem* parent = 0);
    enum WindowRole{
             OUTGOING,
             INCOMING
    };
    QSize getSize() const;
    void setSize(const QSize &value);
 signals:
 void windowRoleChanged();
public slots:
    /**
     * @brief Gets the video surface associated with this camera
     *
     * @return The video surface associated with this camera
     */
   QAbstractVideoSurface *getVideoSurface() const;

    /**
     * @brief Gives a video surface for this camera to draw on
     *
     * @param surface The new surface to draw on
     */
    void setVideoSurface(QAbstractVideoSurface* videoSurface);

private :
    QSize size;
    QAbstractVideoSurface* videoSurface = NULL;             ///< Object that presents the videoFrame to the outside world

   bool videoDisplaying=false;
   QImage *backGroundImage;
   WindowRole windowRole;
public:
    void componentComplete();
    WindowRole getWindowRole() const;
    void setWindowRole(const WindowRole &value);
};

#endif // VideoSurfaceHandle_H
