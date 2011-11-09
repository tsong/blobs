#ifndef IMPLICITMODEL_H
#define IMPLICITMODEL_H

#include "utils/vector.h"

class ImplicitSurface {
public:
    virtual ~ImplicitSurface() {}
    virtual float blend(float x, float y) = 0;
    virtual void blendGrid(float *grid, int rows, int columns, Vector2f origin, Vector2f dimensions) = 0;
protected:
    ImplicitSurface() {}
};

#endif // IMPLICITMODEL_H
