#ifndef IMPLICITMODEL_H
#define IMPLICITMODEL_H

#include "utils/vector.h"

//values greater than the isovalue are inside the surface
//values less than the isovalue are outside the surface
#define ISOVALUE 0.5

//represents a vertex used for polygonization
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

    //returns the field value at position (x,y)
    virtual float fieldValue(float x, float y) = 0;

    //blends this surface with a grid of vertices
    virtual void blendGrid(Vertex *grid, int rows, int columns, Vector2f origin, Vector2f dimensions) = 0;

protected:
    ImplicitSurface() {}
};

#endif // IMPLICITMODEL_H
