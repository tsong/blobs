#include <QDebug>
#include "displaywidget.h"
#include "utils/glutils.h"

DisplayWidget::DisplayWidget(QWidget *parent, QUndoStack *undoStack) :
    QGLWidget(parent), m_undoStack(undoStack),  m_selected(false)
{
    if (!undoStack) {
        undoStack = new QUndoStack(this);
    }

    //each action will redraw the scene
    connect(m_undoStack, SIGNAL(indexChanged(int)), this, SLOT(repaint()));
}

DisplayWidget::~DisplayWidget() {
}

void DisplayWidget::initializeGL() {
    //set background color to white
    glClearColor(1.0, 1.0, 1.0, 1.0);

    //nice looking anti-aliased lines
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(1.25);

}

void DisplayWidget::resizeGL(int width, int height) {
    //change viewport to new dimensions
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);

    //use an orthographic projection in the same dimensions as the draw surface
    glOrtho(0, width, height, 0, -1, 0);

    glMatrixMode(GL_MODELVIEW);
}

void DisplayWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

void DisplayWidget::mousePressEvent(QMouseEvent *event) {
    float x = (float)event->x();
    float y = (float)event->y();
}

void DisplayWidget::mouseMoveEvent(QMouseEvent *event) {
    //issue move command if a control point is currently selected
    if (m_selected) {
    }
}

void DisplayWidget::mouseReleaseEvent(QMouseEvent *) {
    //unselect control point
    if (m_selected) {
        m_selected = false;
        repaint();
    }
}
