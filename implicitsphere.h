#ifndef IMPLICITSPHERE_H
#define IMPLICITSPHERE_H

#include "utils/vector.h"
#include "implicitsurface.h"

class ImplicitSphere : public ImplicitSurface {
public:
    ImplicitSphere(Vector2f position, float radius, Vector3f color = Vector3f(0,0,0));
    ~ImplicitSphere();
    float fieldValue(float x, float y);
    void blendGrid(Vertex *grid, int rows, int columns, Vector2f origin, Vector2f dimensions);

    void setColor(Vector3f &color);
    const Vector3f &getColor();

protected:
    Vector2f m_position;
    float m_radius;

    Vector3f m_color;
};

#endif // IMPLICITSPHERE_H
