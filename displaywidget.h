#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>

#include "utils/vector.h"
#include "polygonizer.h"
#include "implicitsphere.h"

#define RADIUS_MIN 20
#define RADIUS_MAX 100

#define WEIGHT_MIN 0
#define WEIGHT_MAX 10

#define VELOCITY_MIN -200
#define VELOCITY_MAX 200

using namespace std;

class DisplayWidget : public QGLWidget {
    Q_OBJECT
public:
    DisplayWidget(QWidget *parent = 0);
    ~DisplayWidget();

signals:
    void sphereSelected(const ImplicitSphere*);

public slots:
    void animate(bool on);
    void changeRadius(float radius);
    void changeWeight(float weight);
    void changeVelocity(Vector2f velocity);
    void changeColor(Vector3f color);

    void randomizeRadius(bool on) { m_randRadius = on; }
    void randomizeWeight(bool on) { m_randWeight = on; }
    void randomizeVelocity(bool on) { m_randVelocity = on; }
    void randomizeColor(bool on) { m_randColor = on; }

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
    Polygonizer m_polygonizer;

    vector<ImplicitSphere*> m_spheres;

    //tracks selected surface for movement
    bool m_selected;
    uint m_selectedIndex;
    int m_moveCommandId;

    QTimer *m_timer;
    QTime m_time;
    bool m_animate;

    bool m_randRadius;
    bool m_randWeight;
    bool m_randVelocity;
    bool m_randColor;
};

#endif // DISPLAYWIDGET_H
