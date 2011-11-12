#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>

#include "utils/vector.h"
#include "polygonizer.h"
#include "implicitsphere.h"

#define FPS 60

#define RADIUS_MIN 20
#define RADIUS_MAX 100

#define WEIGHT_MIN 0
#define WEIGHT_MAX 10

#define VELOCITY_MIN 5
#define VELOCITY_MAX 60

using namespace std;

class DisplayWidget : public QGLWidget {
    Q_OBJECT
public:
    DisplayWidget(QWidget *parent = 0);
    ~DisplayWidget();

protected:
    /*OpenGL events*/
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    /*mouse events*/
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

protected slots:
    void updateScene();

protected:
    //All interactions are pushed to this stack

    Polygonizer m_polygonizer;

    vector<ImplicitSphere*> m_spheres;

    //tracks selected surface for movement
    bool m_selected;
    uint m_selectedIndex;
    int m_moveCommandId;

    QTimer *m_timer;
    bool m_animate;
};

#endif // DISPLAYWIDGET_H
