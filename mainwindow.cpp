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
    //this->setCentralWidget(m_displayWidget);

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
