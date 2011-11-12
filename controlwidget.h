#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include "utils/vector.h"
#include "displaywidget.h"
#include "implicitsphere.h"

namespace Ui {
    class ControlWidget;
}

class ControlWidget : public QWidget {
    Q_OBJECT

public:
    ControlWidget(QWidget *parent = 0);
    ~ControlWidget();

signals:
    void animate(bool);
    void radiusChanged(float);
    void weightChanged(float);
    void velocityChanged(Vector2f);
    void colorChanged(Vector3f);

public slots:
    void changeSphere(const ImplicitSphere *sphere);
    void changeRadius(float radius);
    void changeWeight(float weight);
    void changeVelocity(Vector2f velocity);
    void changeColor(Vector3f color);

private:
    void disableControls();
    void enableControls();
    void disconnectSignals();
    void connectSignals();
    Ui::ControlWidget *ui;
    bool m_buttonDown;


private slots:
    void updateRadius();
    void updateWeight();
    void updateColor();
    void updateVelocity();
    void buttonClicked();
};

#endif // CONTROLWIDGET_H
