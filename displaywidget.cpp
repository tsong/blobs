#include <QDebug>
#include "displaywidget.h"
#include "utils/glutils.h"
#include "implicitsphere.h"

DisplayWidget::DisplayWidget(QWidget *parent) :
    QGLWidget(parent), m_selected(false), m_animate(false)
{
    m_polygonizer.widget = this;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateScene()));
    //m_timer->start(1000.0 / FPS);
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

    uint numVertices;
    const float *vertices = m_polygonizer.getVertices(numVertices);
    const float *colors = m_polygonizer.getColors(numVertices);
    if (vertices == 0 || colors == 0) return;

    //draw triangles
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glColorPointer(3, GL_FLOAT, 0, colors);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);


    //draw selected
    if (m_selected) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0xCCCC);

        Vector3f color = m_spheres[m_selectedIndex]->getColor();
        Vector2f pos = m_spheres[m_selectedIndex]->getPosition();
        float rad = m_spheres[m_selectedIndex]->getRadius();

        glColor3f(color[0], color[1], color[2]);
        glDrawDottedCircle(pos[0], pos[1], rad*3/2);

        glDisable(GL_LINE_STIPPLE);
    }
}

void DisplayWidget::mousePressEvent(QMouseEvent *event) {
    float x = (float)event->x();
    float y = (float)event->y();
    Vector2f v(x,y);

    //used to determine if current object should be deselected
    bool pressed = false;
    uint lastIndex = m_selectedIndex;

    //check if existing sphere has been clicked
    for (uint i = 0; i < m_spheres.size(); i++) {
        Vector2f c = m_spheres[i]->getPosition();
        float r = m_spheres[i]->getRadius();
        if ((v-c).magnitude() <= r) {
            m_selected = true;
            pressed = true;
            m_selectedIndex = i;
            break;
        }
    }

    if (m_selected && !pressed) {
        m_selected = false;
    } else if (m_selected && event->button() & Qt::RightButton) {
        m_spheres.erase(m_spheres.begin() + m_selectedIndex);
        m_polygonizer.removeSurface(m_selectedIndex);
        m_selected = false;
    } else if (!m_selected && event->button() & Qt::LeftButton) {
        Vector3f color = randColor3f(rand());
        float r1 = rand() / (float)RAND_MAX;
        float r2 = rand() / (float)RAND_MAX;
        Vector2f velocity = Vector2f(501*r1, 501*r2);

        ImplicitSphere *sphere = new ImplicitSphere(v, 40, color, velocity);
        m_spheres.push_back(sphere);
        m_polygonizer.addSurface(sphere);
    }

    repaint();
}

void DisplayWidget::mouseMoveEvent(QMouseEvent *event) {
    //issue move command if a control point is currently selected
    if (m_selected) {
        Vector2f pos(event->x(), event->y());
        m_spheres[m_selectedIndex]->setPosition(pos);
        m_polygonizer.invalidateCache();
        repaint();
    }
}

void DisplayWidget::mouseReleaseEvent(QMouseEvent *) {
}

void DisplayWidget::updateScene() {
    for (uint i = 0; i < m_spheres.size(); i++) {
        m_spheres[i]->update(Vector2f(0,0), Vector2f(width(), height()), 1.0/FPS);
    }

    m_polygonizer.invalidateCache();
    repaint();
}
