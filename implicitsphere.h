#ifndef IMPLICITSPHERE_H
#define IMPLICITSPHERE_H

#include "utils/vector.h"
#include "implicitsurface.h"

class ImplicitSphere : public ImplicitSurface {
public:
    ImplicitSphere(Vector2f position, float radius,
                   Vector3f color = Vector3f(0,0,0),
                   Vector2f velocity = Vector2f(0,0),
                   float weight = 1);
    ~ImplicitSphere();
    float fieldValue(float x, float y);
    void blendGrid(Vertex *grid, int rows, int columns, Vector2f origin, Vector2f dimensions);

    void setColor(Vector3f &color) { m_color = color; }
    const Vector3f &getColor() const { return m_color; }

    void setVelocity(Vector2f &velocity) { m_velocity = velocity; }
    const Vector2f &getVelocity() const { return m_velocity; }

    void setPosition(Vector2f &position) { m_position = position; }
    const Vector2f &getPosition() const { return m_position; }

    void setRadius(float radius) { m_radius = 2*radius; }
    float getRadius() const { return m_radius/2; }

    void setWeight(float weight) { m_weight = weight; }
    float getWeight() const { return m_weight; }

    void update(Vector2f origin, Vector2f dimensions, float timestep);

protected:
    Vector2f m_position;
    float m_radius;
    float m_weight;
    Vector3f m_color;
    Vector2f m_velocity;
};

#endif // IMPLICITSPHERE_H
