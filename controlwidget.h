#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>

namespace Ui {
    class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = 0);
    ~ControlWidget();

private:
    Ui::ControlWidget *ui;

private slots:
    void on_horizontalSlider_4_rangeChanged(int min, int max);
};

#endif // CONTROLWIDGET_H
