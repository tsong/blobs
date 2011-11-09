#ifndef POLYGONIZER_H
#define POLYGONIZER_H

#include <vector>
#include "utils/vector.h"
#include "implicitsurface.h"

#define DEFAULT_ROWS 300
#define DEFAULT_COLUMNS 300
#define DEFAULT_X 0
#define DEFAULT_Y 0
#define DEFAULT_WIDTH 1
#define DEFAULT_HEIGHT 1

#define ISOVALUE 0.5

using namespace std;

class Polygonizer {

public:
    Polygonizer(uint rows=DEFAULT_ROWS, uint columns=DEFAULT_COLUMNS, float x = DEFAULT_X, float y = DEFAULT_Y,
                float width = DEFAULT_WIDTH, float height = DEFAULT_HEIGHT);
    ~Polygonizer();

    void setBounds(float x, float y, float width, float height);

    uint numSurfaces();
    void addSurface(ImplicitSurface *surface);
    void removeSurface(uint position);
    ImplicitSurface *getSurface(uint position);
    const ImplicitSurface *readSurface(uint position) const;

    void polygonize();
    const float *getVertices(uint &size);
    const float *getNormals(uint &size);
    const float *getColors(uint &size);

protected:
    uint m_rows, m_columns;
    Vector2f m_origin, m_dimensions;

    vector<ImplicitSurface*> m_surfaces;

    float *m_grid;
    bool m_isPolygonized;

};

#endif // POLYGONIZER_H
