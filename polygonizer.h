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

    //set the scene boundaries
    void setBounds(float x, float y, float width, float height);

    //add, remove, and read implicit surfaces in scene
    uint numSurfaces();
    void addSurface(ImplicitSurface *surface);
    void removeSurface(uint position);
    ImplicitSurface *getSurface(uint position);
    const ImplicitSurface *readSurface(uint position) const;

    //converts the scene into triangles
    void polygonize();

    //returns buffers needed for rendering
    const float *getVertices(uint &numVertices);
    const float *getNormals(uint &numVertices);
    const float *getColors(uint &numVertices);

    //polygonizations are cached so the scene does not have
    //to be repolygonized every time
    bool isCached();
    void invalidateCache();

protected:
    //resets the all vertices of the grid
    void resetGrid();

    //add a new triangle to the polygonization
    void addTriangle(Vertex a, Vertex b, Vertex c);

    //converts a grid coordinate to scene coordinates
    Vector2f toWorldCoord(float row, float col);
    Vector2f toWorldCoord(uint idx);

    //returns the field value of a position in grid coordinates
    float fieldValue(float i, float j);

    //converts a single cell of the grid into a triangle representation
    void polygonizeCell(uint row, uint col);

    //linearly interpolates points along each edge of a cell
    //which should be on the isosurface
    vector<Vertex> interopolateEdges(uint row, uint col);

protected:
    //size of the grid and dimensions of the scene
    uint m_rows, m_columns;
    Vector2f m_origin, m_dimensions;

    //implicit surfaces in the scene
    vector<ImplicitSurface*> m_surfaces;

    //vertices arranged in a grid forming many rectangular cells
    Vertex *m_grid;

    //true if the polygonization is currently cached
    bool m_isPolygonized;

    //buffers containing geometry and colors needed to render scene
    float *m_vertexBuffer;
    float *m_colorBuffer;
    float *m_normalBuffer;

    //number of elements in buffer and size of buffer
    uint m_numVertices;
    uint m_maxVertices;
};

#endif // POLYGONIZER_H
