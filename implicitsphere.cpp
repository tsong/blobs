#include "implicitsphere.h"

ImplicitSphere::ImplicitSphere(Vector2f position, float radius)
    : m_position(position), m_radius(radius)
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
