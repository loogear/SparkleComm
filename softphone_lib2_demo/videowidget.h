#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>

namespace Ui {
class VideoWidget;
}

class VideoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = nullptr);
    ~VideoWidget();

private:
    Ui::VideoWidget *ui;
};

#endif // VIDEOWIDGET_H
