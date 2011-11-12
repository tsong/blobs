#include "implicitsphere.h"
#include <QGLWidget>

#include "utils/glutils.h"

#define EPS 1E-6
inline int clampUp(float v) {
    int u = v;
    if (abs(u-v) < EPS)
        return u;
    else
        return (int)(v+1);
}


ImplicitSphere::ImplicitSphere(Vector2f position, float radius)
    : m_position(position), m_radius(2*radius)
{
}

ImplicitSphere::~ImplicitSphere() {}


float ImplicitSphere::blend(float x, float y) {
    float dx = x - m_position[0];
    float dy = y - m_position[1];
    float dist = sqrt(dx*dx + dy*dy);

    //if (dist <= m_radius) return 1;
    //else return 0;

    float f = pow(1 - (dist*dist)/(m_radius*m_radius), 3);
    return f > 0 ? f : 0;
}

void ImplicitSphere::blendGrid(float *grid, int rows, int columns,
                           Vector2f origin, Vector2f dimensions)
{

    //glBegin(GL_POLYGON);
    //Vector3f c = randColor3f(m_position[0] * columns + m_position[1]);
    //glColor3f(c[0],c[1],c[2]);

    /*glVertex2f(m_position[0] - m_radius, m_position[1] - m_radius);
    glVertex2f(m_position[0] + m_radius, m_position[1] - m_radius);
    glVertex2f(m_position[0] + m_radius, m_position[1] + m_radius);
    glVertex2f(m_position[0] - m_radius, m_position[1] + m_radius);*/

    //size of each cell in the grid
    float dx = dimensions[0] / (columns-1);
    float dy = dimensions[1] / (rows-1);

    //bounding box of grid cells to blend
    int boundCol = clampUp((m_position[0] - m_radius - origin[0])/dx);
    int boundRow = clampUp((m_position[1] - m_radius - origin[1])/dy);
    int boundWidth = (m_position[0] + m_radius - origin[0])/dx - boundCol;
    int boundHeight = (m_position[1] + m_radius - origin[1])/dy - boundRow;
    //int boundWidth = m_radius*2 / dx;
    //int boundHeight = m_radius*2 / dy;

    /*glVertex2f(origin[0] + (boundCol)*dx, origin[1] + (boundRow)*dy);
    glVertex2f(origin[0] + (boundCol+boundWidth)*dx, origin[1] + (boundRow)*dy);
    glVertex2f(origin[0] + (boundCol+boundWidth)*dx, origin[1] + (boundRow+boundHeight)*dy);
    glVertex2f(origin[0] + (boundCol)*dx, origin[1] + (boundRow+boundHeight)*dy);*/

    if (boundRow >= rows || boundCol >= columns) return;

    //clamp the bounding box
    boundRow = boundRow < 0 ? 0 : boundRow;
    boundCol = boundCol < 0 ? 0 : boundCol;
    boundWidth = (boundCol + boundWidth) >= columns ? columns-boundCol : boundWidth;
    boundHeight = (boundRow + boundHeight) >= rows ? rows-boundRow : boundHeight;

    if (boundWidth <= 0 || boundHeight <= 0) return;

    //glPointSize(10);
    /*glVertex2f(origin[0] + (boundCol)*dx, origin[1] + (boundRow)*dy);
    glVertex2f(origin[0] + (boundCol+boundWidth)*dx, origin[1] + (boundRow)*dy);
    glVertex2f(origin[0] + (boundCol+boundWidth)*dx, origin[1] + (boundRow+boundHeight)*dy);
    glVertex2f(origin[0] + (boundCol)*dx, origin[1] + (boundRow+boundHeight)*dy);*/


    //update the grid
    for (int i = boundRow; i < boundRow + boundHeight; i++) {
        for (int j = boundCol; j < boundCol + boundWidth; j++) {
            grid[i*columns + j] += blend(origin[0] + dx*j, origin[1] + dy*i);
        }
    }

    //glEnd();

    //glColor3f(0,0,0);
    //glDrawCircle(m_position[0], m_position[1], 5);

}

