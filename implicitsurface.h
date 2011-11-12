#ifndef IMPLICITMODEL_H
#define IMPLICITMODEL_H

#include "utils/vector.h"

#define ISOVALUE 0.5

typedef struct Vertex {
    float fieldValue;
    bool isInterior;
    Vector2f pos;
    Vector3f color;
    Vector3f normal;
} Vertex;

class ImplicitSurface {
public:
    virtual ~ImplicitSurface() {}
    virtual float fieldValue(float x, float y) = 0;
    virtual void blendGrid(Vertex *grid, int rows, int columns, Vector2f origin, Vector2f dimensions) = 0;
    virtual void setColor(Vector3f &color) = 0;
    virtual const Vector3f &getColor() = 0;
protected:
    ImplicitSurface() {}
};

#endif // IMPLICITMODEL_H
