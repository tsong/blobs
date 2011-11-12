#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_displayWidget(0)
{
    m_displayWidget = new DisplayWidget(this);
    m_controlWidget = new ControlWidget(this);
    connect(m_displayWidget, SIGNAL(sphereSelected(const ImplicitSphere*)),
            m_controlWidget, SLOT(changeSphere(const ImplicitSphere*)));

    connect(m_controlWidget, SIGNAL(animate(bool)), m_displayWidget, SLOT(animate(bool)));
    connect(m_controlWidget, SIGNAL(radiusChanged(float)), m_displayWidget, SLOT(changeRadius(float)));
    connect(m_controlWidget, SIGNAL(weightChanged(float)), m_displayWidget, SLOT(changeWeight(float)));
    connect(m_controlWidget, SIGNAL(colorChanged(Vector3f)), m_displayWidget, SLOT(changeColor(Vector3f)));
    connect(m_controlWidget, SIGNAL(velocityChanged(Vector2f)), m_displayWidget, SLOT(changeVelocity(Vector2f)));


    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(m_displayWidget);
    splitter->addWidget(m_controlWidget);
    this->setCentralWidget(splitter);

    resize(MAIN_WINDOW_DEFAULT_WIDTH, MAIN_WINDOW_DEFAULT_HEIGHT);
}

MainWindow::~MainWindow()
{
    delete m_displayWidget;
}
