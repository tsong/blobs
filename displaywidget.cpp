#include <QDebug>
#include "displaywidget.h"
#include "utils/glutils.h"
#include "implicitsphere.h"

DisplayWidget::DisplayWidget(QWidget *parent, QUndoStack *undoStack) :
    QGLWidget(parent), m_undoStack(undoStack), m_selected(false)
{
    if (!undoStack) {
        undoStack = new QUndoStack(this);
    }

    //each action will redraw the scene
    connect(m_undoStack, SIGNAL(indexChanged(int)), this, SLOT(repaint()));
    m_polygonizer.widget = this;
}

DisplayWidget::~DisplayWidget() {
    for (uint i = 0; i < m_polygonizer.numSurfaces(); i++) {
        delete m_polygonizer.getSurface(i);
    }
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

    m_polygonizer.setBounds(0, 0, width, height);
}

void DisplayWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    m_polygonizer.polygonize();
    /*float data[vertices.size()*2];
    for (uint i = 0; i < vertices.size(); i++) {
        Vector2f v = vertices[i];
        data[2*i] = v[0];
        data[2*i+1] = v[1];
    }

    glColor3f(0,0,0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, data);

    // draw a cube
    glDrawArrays(GL_QUADS, 0, 24);
    glDrawArrays(GL_POINTS, 0, vertices.size());

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);*/
}

void DisplayWidget::mousePressEvent(QMouseEvent *event) {
    float x = (float)event->x();
    float y = (float)event->y();

    ImplicitSphere *sphere = new ImplicitSphere(Vector2f(x,y), 10);
    m_polygonizer.addSurface(sphere);
    repaint();
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
