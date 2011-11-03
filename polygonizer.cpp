#include "polygonizer.h"
#include <QDebug>
#include <QGLWidget>

Polygonizer::Polygonizer(uint rows, uint columns, float x, float y, float width, float height)
    : m_rows(rows), m_columns(columns), m_origin(x,y), m_width(width), m_height(height)
{
    m_grid = new bool[rows*columns];
    m_vertices.reserve(rows*columns);
}

Polygonizer::~Polygonizer() {
    delete[] m_grid;
}

void Polygonizer::setBounds(float x, float y, float width, float height) {
    m_origin = Vector2f(x,y);
    m_width = width;
    m_height = height;
}


uint Polygonizer::numSurfaces() {
    return m_surfaces.size();
}

void Polygonizer::addSurface(ImplicitSurface *surface) {
    m_surfaces.push_back(surface);
}

void Polygonizer::removeSurface(uint position) {
    if (position < m_surfaces.size())
        m_surfaces.erase(m_surfaces.begin() + position);
}

ImplicitSurface *Polygonizer::getSurface(uint position) {
    if (position < m_surfaces.size())
        return m_surfaces[position];
    return 0;
}

const vector<Vector2f> &Polygonizer::polygonize() {
    m_vertices.clear();
    if (m_rows == 0 || m_columns == 0) {
        return m_vertices;
    }

    glBegin(GL_POINTS);

    //distance traveled in one grid step
    float dx = (m_width - m_origin[0]) / m_columns;
    float dy = (m_height - m_origin[1]) / m_rows;

    //compute the grid of points inside and outside of isosurface
    for (uint i = 0; i < m_rows; i++) {
        float y = m_origin[1] + i*dy;

        for (uint j = 0; j < m_columns; j++) {
            float x = m_origin[0] + j*dx;

            //sum the contribution of the blending functions of all surfaces
            float sum = 0;
            for (uint k = 0; k < m_surfaces.size(); k++) {
                sum += m_surfaces[k]->blend(x,y);
            }

            float c = sum > 1 ? 1 : sum;
            glColor3f(c,c,c);
            //glVertex2d(x,y);

            m_grid[i*m_columns + j] = sum >= ISOVALUE;
            if (m_grid[i*m_columns + j]) {
                m_vertices.push_back(Vector2f(x,y));
            }
        }
    }
    glEnd();

    //qDebug() << m_vertices.size();
    return m_vertices;
}
