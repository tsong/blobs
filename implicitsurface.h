#ifndef IMPLICITMODEL_H
#define IMPLICITMODEL_H

class ImplicitSurface {
public:
    virtual ~ImplicitSurface() {}
    virtual float blend(float x, float y) = 0;
protected:
    ImplicitSurface() {}
};

#endif // IMPLICITMODEL_H
