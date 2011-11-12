#ifndef POLYGONIZER_H
#define POLYGONIZER_H

#include <QGLWidget>
#include <vector>
#include "utils/vector.h"
#include "implicitsurface.h"

#define DEFAULT_ROWS 200
#define DEFAULT_COLUMNS 200
#define DEFAULT_X 0
#define DEFAULT_Y 0
#define DEFAULT_WIDTH 1
#define DEFAULT_HEIGHT 1


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
    const float *getVertices(uint &numVertices);
    const float *getNormals(uint &numVertices);
    const float *getColors(uint &numVertices);

    bool isCached();
    void invalidateCache();

protected:
    void resetGrid();
    void addTriangle(Vertex a, Vertex b, Vertex c);
    Vector2f toWorldCoord(float row, float col);
    Vector2f toWorldCoord(uint idx);
    float fieldValue(float i, float j);
    void polygonizeCell(uint row, uint col);
    vector<Vertex> interopolateEdges(uint row, uint col);

protected:
    uint m_rows, m_columns;
    Vector2f m_origin, m_dimensions;

    vector<ImplicitSurface*> m_surfaces;

    Vertex *m_grid;
    bool m_isPolygonized;

    float *m_vertexBuffer;
    float *m_colorBuffer;
    float *m_normalBuffer;
    uint m_numVertices;
    uint m_maxVertices;
public:
    QGLWidget *widget;
};

#endif // POLYGONIZER_H
