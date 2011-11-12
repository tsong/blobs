#include "polygonizer.h"
#include <QDebug>
#include <QGLWidget>
#include "utils/glutils.h"

Polygonizer::Polygonizer(uint rows, uint columns, float x, float y, float width, float height)
    : m_rows(rows), m_columns(columns), m_origin(x,y), m_dimensions(width, height),
    m_isPolygonized(false), m_numVertices(0), m_maxVertices(0)
{
    m_grid = new float[rows*columns];
    m_interiorPoints = new bool[rows*columns];

    //theoretically, there may be more than 2 times the number of cells in triangles,
    //but would be a very unlikely occurance
    m_maxVertices = 2*rows*columns;

    //initialize buffers using malloc (we may need to use realloc later)
    m_vertexBuffer = (float*)malloc(2*m_maxVertices*sizeof(float));
    m_colorBuffer = (float*)malloc(3*m_maxVertices*sizeof(float));
    m_normalBuffer = (float*)malloc(3*m_maxVertices*sizeof(float));

    //can't continue if we couldn't allocate the memory we wanted
    if (m_vertexBuffer == NULL || m_colorBuffer == NULL || m_normalBuffer == NULL)
        exit(1);
}

Polygonizer::~Polygonizer() {
    delete[] m_grid;
    delete[] m_interiorPoints;

    free(m_vertexBuffer);
    free(m_colorBuffer);
    free(m_normalBuffer);
}

void Polygonizer::setBounds(float x, float y, float width, float height) {
    m_origin = Vector2f(x,y);
    m_dimensions = Vector2f(width, height);
    m_isPolygonized = false;
}


uint Polygonizer::numSurfaces() {
    m_isPolygonized = false;
    return m_surfaces.size();
}

void Polygonizer::addSurface(ImplicitSurface *surface) {
    m_isPolygonized = false;
    m_surfaces.push_back(surface);
}

void Polygonizer::removeSurface(uint position) {
    if (position < m_surfaces.size()) {
        m_surfaces.erase(m_surfaces.begin() + position);
        m_isPolygonized = false;
    }
}

ImplicitSurface *Polygonizer::getSurface(uint position) {
    if (position < m_surfaces.size()) {
        m_isPolygonized = false;
        return m_surfaces[position];
    }
    return 0;
}

const ImplicitSurface *Polygonizer::readSurface(uint position) const {
    if (position < m_surfaces.size()) {
        return m_surfaces[position];
    }
    return 0;
}

void Polygonizer::polygonize() {
    if (m_isPolygonized) return;
    m_numVertices = 0;

    //zero the grid and interior points matrix
    memset(m_grid, 0, m_rows*m_columns*sizeof(float));
    memset(m_interiorPoints, 0, m_rows*m_columns*sizeof(bool));

    //blend the grid using all the implicit surfaces
    for (uint i = 0; i < m_surfaces.size(); i++) {
        m_surfaces[i]->blendGrid(m_grid, m_rows, m_columns, m_origin, m_dimensions);
    }

    //calculate interior points matrix
    for (uint i = 0; i < m_rows; i++) {
        for (uint j = 0; j < m_columns; j++) {
            m_interiorPoints[i*m_columns+j] = m_grid[i*m_columns+j] >= ISOVALUE;
        }
    }

    //convert each rectangular cell to triangles
    for (uint i = 0; i < m_rows-1; i++) {
        for (uint j = 0; j < m_columns-1; j++) {
            polygonizeCell(i,j);
        }
    }

    m_isPolygonized = true;
}

const float *Polygonizer::getVertices(uint &numVertices) {
    if (m_isPolygonized) {
        numVertices = m_numVertices;
        return m_vertexBuffer;
    }

    return 0;
}

const float *Polygonizer::getNormals(uint &numVertices) {
    if (m_isPolygonized) {
        numVertices = m_numVertices;
        return m_normalBuffer;
    }

    return 0;
}

const float *Polygonizer::getColors(uint &numVertices) {
    if (m_isPolygonized) {
        numVertices = m_numVertices;
        return m_colorBuffer;
    }

    return 0;
}



Vector2f Polygonizer::toWorldCoord(float row, float col) {
    float dx = m_dimensions[0] / (m_columns-1);
    float dy = m_dimensions[1] / (m_rows-1);
    return Vector2f(m_origin[0] + dx*col, m_origin[1] + dy*row);
}

Vector2f Polygonizer::toWorldCoord(uint idx) {
    uint row = idx / m_columns;
    uint col = idx % m_columns;
    return toWorldCoord(row, col);
}

float Polygonizer::fieldValue(float i, float j) {
    float f = 0;
    Vector2f v = toWorldCoord(i,j);

    for (uint i = 0; i < m_surfaces.size(); i++) {
        f += m_surfaces[i]->blend(v[0], v[1]);
    }

    return f;
}

vector<Vector2f> Polygonizer::interopolateEdges(uint row, uint col) {
    //indices of top-left, top-right, bottom-left, and bottom-right points respectively
    uint topLeft = (row+0)*m_columns + col+0;
    uint topRight = (row+0)*m_columns + col+1;
    uint botLeft = (row+1)*m_columns + col+0;
    uint botRight = (row+1)*m_columns + col+1;

    uint edges[4][2] = {
                        {topLeft,topRight},  //top edge
                        {topLeft, botLeft},  //left edge
                        {botLeft, botRight}, //bottom edge
                        {topRight, botRight} //right edge
                     };

    Vector2f startingPositions[4] = {
                                     toWorldCoord(topLeft),
                                     toWorldCoord(topLeft),
                                     toWorldCoord(botLeft),
                                     toWorldCoord(topRight)
                                    };

    float dx = m_dimensions[0] / (m_columns-1);
    float dy = m_dimensions[1] / (m_rows-1);
    Vector2f directions[4] =  {
                               Vector2f(dx,0),
                               Vector2f(0,dy),
                               Vector2f(dx,0),
                               Vector2f(0,dy)
                              };

    vector<Vector2f> points;
    for (uint i = 0; i < 4; i++) {
       //check if the edge has one point inside surface and one outside surface
       if (m_interiorPoints[edges[i][0]] ^ m_interiorPoints[edges[i][1]]) {
            //linear interpolation ratio
            float a = m_grid[edges[i][0]];
            float b = m_grid[edges[i][1]];
            float m = ISOVALUE;
            float r = abs(m-a)/abs(b-a);

            //calculate and add the interpolated point (in world coordinates)
            points.push_back(startingPositions[i] + (directions[i]*r));
       }
    }


    /*Vector2f v;
    glColor3f(0,0,0);
    glBegin(GL_LINE_STRIP);
    v = toWorldCoord(topLeft);
    glVertex2f(v[0], v[1]);
    v = toWorldCoord(topRight);
    glVertex2f(v[0], v[1]);
    v = toWorldCoord(botRight);
    glVertex2f(v[0], v[1]);
    v = toWorldCoord(botLeft);
    glVertex2f(v[0], v[1]);
    glEnd();*/


    return points;
}

void Polygonizer::polygonizeCell(uint row, uint col) {
    vector<Vector2f> interp = interopolateEdges(row, col);

    //flat indices of each corner point of the cell
    uint cornerIndices[4] = {
        (row+0)*m_columns + col+0,
        (row+0)*m_columns + col+1,
        (row+1)*m_columns + col+0,
        (row+1)*m_columns + col+1
    };

    //corners which are interior points
    uint interiorCorners[4];
    uint numInterior = 0;

    //calculate binary representation of the current cell
    uint combination = 0;
    for (uint i = 0; i < 4; i++) {
            float v = m_grid[cornerIndices[i]];
            combination = (combination << 1) | (v >= ISOVALUE ? 1 : 0);

            if (v >= ISOVALUE)
                interiorCorners[numInterior++] = i;
    }

    /*Vector2f p = toWorldCoord(row, col);
    float dx = m_dimensions[0] / (m_columns-1);
    float dy = m_dimensions[1] / (m_rows-1);
    glColor3f(1,0,0);
    uint pad = 6;
    widget->renderText(p[0]+dx/2-pad, p[1]+dy/2+pad, 0, QString("%1").arg(combination));*/

    /*glColor3f(0,0,1);
    for (uint i = 0; i < interp.size(); i++) {
        Vector2f v = interp[i];
        glDrawCircle(v[0],v[1],3);
    }*/


    Vector2f corner[4] = {toWorldCoord(cornerIndices[0]),
                          toWorldCoord(cornerIndices[1]),
                          toWorldCoord(cornerIndices[2]),
                          toWorldCoord(cornerIndices[3])};

    //glColor3f(0,1,0);
    //glBegin(GL_TRIANGLES);
    switch(combination) {
    case 1:  //0001
    case 2:  //0010
    case 4:  //0100
    case 8:  //1000
         addTriangle(corner[interiorCorners[0]], interp[0], interp[1]);
        break;

    case 3:  //0011
    case 5:  //0101
    case 10: //1010
    case 12: //1100
        addTriangle(corner[interiorCorners[0]], interp[1], corner[interiorCorners[1]]);
        addTriangle(corner[interiorCorners[0]], interp[0], interp[1]);
        break;

    case 6:  //0110
        addTriangle(corner[1], interp[0], interp[3]);
        addTriangle(corner[2], interp[1], interp[2]);
        if (fieldValue(col+0.5, row+0.5) >= ISOVALUE) {
            addTriangle(interp[0], interp[1], interp[2]);
            addTriangle(interp[0], interp[2], interp[3]);
        }
        break;
    case 9:  //1001
        addTriangle(corner[0], interp[0], interp[1]);
        addTriangle(corner[3], interp[2], interp[3]);
        if (fieldValue(col+0.5, row+0.5) >= ISOVALUE) {
            addTriangle(interp[0], interp[2], interp[3]);
            addTriangle(interp[0], interp[1], interp[2]);
        }
        break;

    case 7:  //0111
        addTriangle(corner[3], interp[0], interp[1]);
        addTriangle(corner[3], interp[0], corner[1]);
        addTriangle(corner[3], interp[1], corner[2]);
        break;
    case 11: //1011
        addTriangle(corner[2], interp[0], interp[1]);
        addTriangle(corner[2], interp[0], corner[0]);
        addTriangle(corner[2], interp[1], corner[3]);
        break;
    case 13: //1101
        addTriangle(corner[1], interp[0], interp[1]);
        addTriangle(corner[1], interp[0], corner[0]);
        addTriangle(corner[1], interp[1], corner[3]);
        break;
    case 14: //1110
        addTriangle(corner[0], interp[0], interp[1]);
        addTriangle(corner[0], interp[0], corner[2]);
        addTriangle(corner[0], interp[1], corner[1]);
        break;

    case 15: //1111
        addTriangle(corner[0], corner[1], corner[2]);
        addTriangle(corner[1], corner[2], corner[3]);
        break;

    case 0:  //0000
    default:
        break;
    }
    //glEnd();
}

void Polygonizer::addTriangle(Vector2f a, Vector2f b, Vector2f c) {
    //glVertex2f(a[0],a[1]);
    //glVertex2f(b[0],b[1]);
    //glVertex2f(c[0],c[1]);

    //allocate more memory if we somehow reached the limit
    if (m_numVertices >= m_maxVertices - 3) {
        m_maxVertices *= 2;
        m_vertexBuffer = (float*)realloc(m_vertexBuffer, 2*m_maxVertices*sizeof(float));
        m_colorBuffer = (float*)realloc(m_colorBuffer, 3*m_maxVertices*sizeof(float));
        m_normalBuffer = (float*)realloc(m_normalBuffer, 3*m_maxVertices*sizeof(float));

        if (m_vertexBuffer == NULL || m_colorBuffer == NULL || m_normalBuffer == NULL)
            exit(1);
    }

    //add the triangle to the buffers
    Vector2f *V[] = {&a, &b, &c};
    for (uint i = 0; i < 3; i++) {
        for (uint j = 0; j < 2; j++) {
            m_vertexBuffer[m_numVertices*2 + 2*i+j] = V[i]->get(j);
        }
    }

    /*glBegin(GL_TRIANGLES);
    glVertex2f(a[0],a[1]);
    glVertex2f(b[0],b[1]);
    glVertex2f(c[0],c[1]);
    glEnd();*/

    m_numVertices += 3;
}
