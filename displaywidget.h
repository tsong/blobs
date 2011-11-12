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

#define WEIGHT_MIN 0.6
#define WEIGHT_MAX 5

#define VELOCITY_MIN -200
#define VELOCITY_MAX 200

using namespace std;

class DisplayWidget : public QGLWidget {
    Q_OBJECT
public:
    DisplayWidget(QWidget *parent = 0);
    ~DisplayWidget();

signals:
    //emitted when a sphere is selected
    //if a sphere is deselected, null is passed as an argument
    void sphereSelected(const ImplicitSphere*);

public slots:
    //change attributes of the currently selected surface
    void animate(bool on);
    void changeRadius(float radius);
    void changeWeight(float weight);
    void changeVelocity(Vector2f velocity);
    void changeColor(Vector3f color);

protected:
    /*OpenGL events*/
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    /*mouse events*/
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    //creates an implicit sphere with random attributes
    ImplicitSphere *createSphere(Vector2f pos);

protected slots:
    //updates the positions of all objects for animation
    void updateScene();

protected:
    //converts the implicit surfaces to triangles to render using OpenGL
    Polygonizer m_polygonizer;

    //a collection of the implicit spheres in the scene
    vector<ImplicitSphere*> m_spheres;

    //tracks selected surface for movement
    bool m_selected;
    uint m_selectedIndex;

    //variables for animation
    QTimer *m_timer; //triggers scene updates
    QTime m_time;    //determines timestep from last update
    bool m_animate;  //tracks if scene should be animated
};

#endif // DISPLAYWIDGET_H
