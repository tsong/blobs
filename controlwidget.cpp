#include "controlwidget.h"
#include "ui_controlwidget.h"

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget)
{
    ui->setupUi(this);
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::on_horizontalSlider_4_rangeChanged(int min, int max)
{

}
