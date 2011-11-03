#ifndef IMPLICITSPHERE_H
#define IMPLICITSPHERE_H

#include "utils/vector.h"
#include "implicitsurface.h"

class ImplicitSphere : public ImplicitSurface {
public:
    ImplicitSphere(Vector2f position, float radius);
    ~ImplicitSphere();
    float blend(float x, float y);

protected:
    Vector2f m_position;
    float m_radius;
};

#endif // IMPLICITSPHERE_H
