#ifndef IMPLICITSPHERE_H
#define IMPLICITSPHERE_H

#include "utils/vector.h"
#include "implicitsurface.h"

class ImplicitSphere : public ImplicitSurface {
public:
    ImplicitSphere(Vector2f position, float radius);
    ~ImplicitSphere();
    float blend(float x, float y);
    void blendGrid(float *grid, int rows, int columns, Vector2f origin, Vector2f dimensions);

protected:
    Vector2f m_position;
    float m_radius;
};

#endif // IMPLICITSPHERE_H
