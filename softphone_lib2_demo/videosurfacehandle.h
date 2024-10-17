#ifndef VideoSurfaceHandle_H
#define VideoSurfaceHandle_H

#include <QObject>
#include <QTimer>
#include <QVideoFrame>
#include <QAbstractVideoSurface>
class VideoRenderInstance;
#ifdef QML_MOD
#include <QQuickItem>
#define DISP_OBJECT QQuickItem
class VideoSurfaceHandle : public QQuickItem
#else
#define DISP_OBJECT QObject
class VideoSurfaceHandle : public QObject
#endif
{
    Q_OBJECT

    Q_ENUMS(WindowRole)

    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ getVideoSurface WRITE setVideoSurface)

    Q_PROPERTY(WindowRole m_windowRole READ getWindowRole WRITE setWindowRole NOTIFY windowRoleChanged())

    Q_PROPERTY(VideoRenderInstance *renderInstace READ renderInstace WRITE setRenderInstace NOTIFY renderInstaceChanged FINAL)

public:

    VideoSurfaceHandle(DISP_OBJECT* parent = 0);

    enum WindowRole{
        OUTGOING,
        INCOMING
    };
signals:
    void windowRoleChanged();
    void startSignal(unsigned w, unsigned h, QVideoFrame::PixelFormat format);
    void fristFrame();
    void renderInstaceChanged();

public slots:
    void timeout();

    QAbstractVideoSurface *getVideoSurface() const;

    void setVideoSurface(QAbstractVideoSurface* videoSurface);

    void presentSlot(const QVideoFrame &frame);

    void startSlot(unsigned w, unsigned h, QVideoFrame::PixelFormat format);

private :
    QAbstractVideoSurface* videoSurface{nullptr};
    QImage *backGroundImage;
    WindowRole m_windowRole;
    QTimer m_timer;
    VideoRenderInstance *m_renderInstace;

public:
    void componentComplete();
    WindowRole getWindowRole() const;
    void setWindowRole(const WindowRole &windowRole);

    VideoRenderInstance *renderInstace() const;
    void setRenderInstace(VideoRenderInstance *newRenderInstace);

protected:
    void windowDeactivateEvent();
};

#endif // VideoSurfaceHandle_H
