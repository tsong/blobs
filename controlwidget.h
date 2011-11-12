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
    //emitted when ui controls are changed
    void animate(bool);
    void radiusChanged(float);
    void weightChanged(float);
    void velocityChanged(Vector2f);
    void colorChanged(Vector3f);

public slots:
    //change the values of the ui elements
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

private slots:
    //update the text of each label and emit changed signal
    void updateRadius();
    void updateWeight();
    void updateColor();
    void updateVelocity();

    //animate button clicked
    void animateClicked();

private:
    Ui::ControlWidget *ui;
    bool m_buttonDown;

};

#endif // CONTROLWIDGET_H
