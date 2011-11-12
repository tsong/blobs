#include "controlwidget.h"
#include "ui_controlwidget.h"

#include <QDebug>

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ControlWidget), m_buttonDown(false)
{
    ui->setupUi(this);
    updateRadius();
    updateWeight();
    updateColor();
    updateVelocity();

    connectSignals();

    disableControls();
}

ControlWidget::~ControlWidget() {
    delete ui;
}

void ControlWidget::disableControls() {
    ui->radiusGroupBox->setEnabled(false);
    ui->weightGroupBox->setEnabled(false);
    ui->velocityGroupBox->setEnabled(false);
    ui->colorGroupBox->setEnabled(false);
}

void ControlWidget::enableControls() {
    ui->radiusGroupBox->setEnabled(true);
    ui->weightGroupBox->setEnabled(true);
    ui->velocityGroupBox->setEnabled(true);
    ui->colorGroupBox->setEnabled(true);
}

void ControlWidget::connectSignals() {
    connect(ui->animateButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateRadius()));
    connect(ui->weightSlider, SIGNAL(valueChanged(int)), this, SLOT(updateWeight()));
    connect(ui->dxSlider, SIGNAL(valueChanged(int)), this, SLOT(updateVelocity()));
    connect(ui->dySlider, SIGNAL(valueChanged(int)), this, SLOT(updateVelocity()));
    connect(ui->redSlider, SIGNAL(valueChanged(int)), this, SLOT(updateColor()));
    connect(ui->greenSlider, SIGNAL(valueChanged(int)), this, SLOT(updateColor()));
    connect(ui->blueSlider, SIGNAL(valueChanged(int)), this, SLOT(updateColor()));
}

void ControlWidget::disconnectSignals() {
    disconnect(ui->animateButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    disconnect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateRadius()));
    disconnect(ui->weightSlider, SIGNAL(valueChanged(int)), this, SLOT(updateWeight()));
    disconnect(ui->dxSlider, SIGNAL(valueChanged(int)), this, SLOT(updateVelocity()));
    disconnect(ui->dySlider, SIGNAL(valueChanged(int)), this, SLOT(updateVelocity()));
    disconnect(ui->redSlider, SIGNAL(valueChanged(int)), this, SLOT(updateColor()));
    disconnect(ui->greenSlider, SIGNAL(valueChanged(int)), this, SLOT(updateColor()));
    disconnect(ui->blueSlider, SIGNAL(valueChanged(int)), this, SLOT(updateColor()));
}

void ControlWidget::changeSphere(const ImplicitSphere *sphere) {
    if (sphere == 0) {
        disableControls();
        return;
    } else {
        enableControls();
    }

    float radius = sphere->getRadius();
    float weight = sphere->getWeight();
    const Vector2f &velocity = sphere->getVelocity();
    const Vector3f &color = sphere->getColor();

    disconnectSignals();
    changeRadius(radius);
    changeWeight(weight);
    changeVelocity(velocity);
    changeColor(color);
    connectSignals();

    blockSignals(true);
    updateRadius();
    updateWeight();
    updateVelocity();
    updateColor();
    blockSignals(false);
}

void ControlWidget::changeRadius(float radius) {
    int v = (radius - RADIUS_MIN) / (RADIUS_MAX - RADIUS_MIN) * 100;
    ui->radiusSlider->setValue(v);
}

void ControlWidget::changeWeight(float weight) {
    int v = (weight - WEIGHT_MIN) / (WEIGHT_MAX - WEIGHT_MIN) * 100;
    ui->weightSlider->setValue(v);
}

void ControlWidget::changeVelocity(Vector2f velocity) {
    int v1 = (velocity[0] - VELOCITY_MIN) / (VELOCITY_MAX - VELOCITY_MIN) * 100;
    int v2 = (velocity[1] - VELOCITY_MIN) / (VELOCITY_MAX - VELOCITY_MIN) * 100;

    ui->dxSlider->setValue(v1);
    ui->dySlider->setValue(v2);
}

void ControlWidget::changeColor(Vector3f color) {
    ui->redSlider->setValue(color[0]*100);
    ui->greenSlider->setValue(color[1]*100);
    ui->blueSlider->setValue(color[2]*100);
}

void ControlWidget::updateRadius() {
    float v = ui->radiusSlider->value() / 100.0;
    float rad = RADIUS_MIN + v*(RADIUS_MAX - RADIUS_MIN);
    ui->radiusGroupBox->setTitle(QString("Radius: %1").arg((int)rad));

    emit radiusChanged(rad);
}

void ControlWidget::updateWeight() {
    float v = ui->weightSlider->value() / 100.0;
    float c = WEIGHT_MIN + v*(WEIGHT_MAX - WEIGHT_MIN);
    ui->weightGroupBox->setTitle(QString("Weight: %1").arg(c));

    emit weightChanged(c);
}

void ControlWidget::updateColor() {
    float r = ui->redSlider->value() / 100.0;
    float g = ui->greenSlider->value() / 100.0;
    float b = ui->blueSlider->value() / 100.0;

    int v1 = r*255;
    int v2 = g*255;
    int v3 = b*255;

    ui->colorGroupBox->setTitle(QString("Color: (%1,%2,%3)").arg(v1).arg(v2).arg(v3));

    emit colorChanged(Vector3f(r,g,b));
}

void ControlWidget::updateVelocity() {
    float vx = ui->dxSlider->value() / 100.0;
    float dx = VELOCITY_MIN + vx*(VELOCITY_MAX - VELOCITY_MIN);

    float vy = ui->dySlider->value() / 100.0;
    float dy = VELOCITY_MIN + vy*(VELOCITY_MAX - VELOCITY_MIN);

    ui->velocityGroupBox->setTitle(QString("Velocity: (%1,%2)").arg((int)dx).arg((int)dy));

    emit velocityChanged(Vector2f(dx,dy));
}

void ControlWidget::buttonClicked()
{
    m_buttonDown = !m_buttonDown;
    ui->animateButton->setDown(m_buttonDown);
    emit animate(m_buttonDown);
}
