#include "implicitsphere.h"
#include <QGLWidget>

#include "utils/glutils.h"

#define EPS 1E-6
inline int ceiling(float v) {
    int u = v;
    if (abs(u-v) < EPS)
        return u;
    else
        return (int)(v+1);
}


ImplicitSphere::ImplicitSphere(Vector2f position, float radius,
                               Vector3f color, Vector2f velocity, float weight)
    : m_position(position), m_radius(2*radius),
      m_weight(weight), m_color(color), m_velocity(velocity)
{
}

ImplicitSphere::~ImplicitSphere() {}

float ImplicitSphere::fieldValue(float x, float y) {
    //calculate distance
    float dx = x - m_position[0];
    float dy = y - m_position[1];
    float dist = sqrt(dx*dx + dy*dy);

    //use the Wyvill function as the field function
    float f = pow(1 - (dist*dist)/(m_radius*m_radius), 3);
    return f > 0 ? f : 0;
}

void ImplicitSphere::blendGrid(Vertex *grid, int rows, int columns,
                           Vector2f origin, Vector2f dimensions)
{
    //size of each cell in the grid
    float dx = dimensions[0] / (columns-1);
    float dy = dimensions[1] / (rows-1);

    //bounding box of grid cells to blend
    int boundCol = ceiling((m_position[0] - m_radius - origin[0])/dx);
    int boundRow = ceiling((m_position[1] - m_radius - origin[1])/dy);
    int boundWidth = (m_position[0] + m_radius - origin[0])/dx - boundCol;
    int boundHeight = (m_position[1] + m_radius - origin[1])/dy - boundRow;

    //we don't have to do anything if the bounding box is not within the scene boundaries
    if (boundRow >= rows || boundCol >= columns) return;

    //clamp the bounding box
    boundRow = boundRow < 0 ? 0 : boundRow;
    boundCol = boundCol < 0 ? 0 : boundCol;
    boundWidth = (boundCol + boundWidth) >= columns ? columns-boundCol : boundWidth;
    boundHeight = (boundRow + boundHeight) >= rows ? rows-boundRow : boundHeight;

    //we don't have to do anything if the dimensions of the bounding box are degenerate
    if (boundWidth <= 0 || boundHeight <= 0) return;

    //update cell in the grid
    for (int i = boundRow; i < boundRow + boundHeight; i++) {
        for (int j = boundCol; j < boundCol + boundWidth; j++) {
            float f = fieldValue(origin[0] + dx*j, origin[1] + dy*i);

            //update grid vertex
            Vertex &v = grid[i*columns + j];
            v.color = m_color*f + v.color;
            v.fieldValue += f*m_weight;
            v.isInterior = v.fieldValue >= ISOVALUE;
        }
    }
}

void ImplicitSphere::update(Vector2f origin, Vector2f dimensions, float timestep) {
    //calculate new position from velocity
    m_position = m_position + m_velocity*timestep;

    //bounce off walls
    for (uint i = 0; i < 2; i++) {
        if (m_position[i] - m_radius/2 < origin[i]) {
            m_position[i] = origin[i] + m_radius/2;
            m_velocity[i] *= -1;
        } else if (m_position[i] + m_radius/2 > dimensions[i]) {
            m_position[i] = dimensions[i] - m_radius/2;
            m_velocity[i] *= -1;
        }
    }
}

