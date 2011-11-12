#ifndef IMPLICITSPHERE_H
#define IMPLICITSPHERE_H

#include "utils/vector.h"
#include "implicitsurface.h"

class ImplicitSphere : public ImplicitSurface {
public:
    ImplicitSphere(Vector2f position, float radius,
                   Vector3f color = Vector3f(0,0,0),
                   Vector2f velocity = Vector2f(0,0));
    ~ImplicitSphere();
    float fieldValue(float x, float y);
    void blendGrid(Vertex *grid, int rows, int columns, Vector2f origin, Vector2f dimensions);

    void setColor(Vector3f &color);
    const Vector3f &getColor();

    void setPosition(Vector2f &position);
    const Vector2f &getPosition();

    void setRadius(float radius);
    float getRadius();

    void update(Vector2f origin, Vector2f dimensions, float timestep);

protected:
    Vector2f m_position;
    float m_radius;

    Vector3f m_color;
    Vector2f m_velocity;
};

#endif // IMPLICITSPHERE_H
