#include "polygonizer.h"
#include <QDebug>
#include <QGLWidget>
#include "utils/glutils.h"

Polygonizer::Polygonizer(uint rows, uint columns, float x, float y, float width, float height)
    : m_rows(rows), m_columns(columns), m_origin(x,y), m_dimensions(width, height),
    m_isPolygonized(false)
{
    m_grid = new float[rows*columns];
}

Polygonizer::~Polygonizer() {
    delete[] m_grid;
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

    //memset(m_grid, 0, m_rows*m_columns);
    for (uint i = 0; i < m_rows*m_columns; i++)
        m_grid[i] = 0;


    for (uint i = 0; i < m_surfaces.size(); i++) {
        m_surfaces[i]->blendGrid(m_grid, m_rows, m_columns, m_origin, m_dimensions);
    }

    glBegin(GL_POINTS);

    //distance traveled in one grid step
    float dx = m_dimensions[0] / m_columns;
    float dy = m_dimensions[1] / m_rows;

    //compute the grid of points inside and outside of isosurface
    for (uint i = 0; i < m_rows; i++) {
        float y = m_origin[1] + i*dy;

        for (uint j = 0; j < m_columns; j++) {
            float x = m_origin[0] + j*dx;

            float v = m_grid[i*m_columns + j];
            //if (v >= ISOVALUE) {
                glColor3f(v,v,v);
                glVertex2f(x,y);
            //}
        }
    }
    glEnd();

    /*glBegin(GL_POINTS);

    //distance traveled in one grid step
    float dx = m_width / m_columns;
    float dy = m_height / m_rows;

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

            m_grid[i*m_columns + j] = sum;
            if (m_grid[i*m_columns + j] >= ISOVALUE) {
                m_vertices.push_back(Vector2f(x,y));
            }
        }
    }
    glEnd();*/

    //qDebug() << m_vertices.size();
}

const float *Polygonizer::getVertices(uint &size) {

}

const float *Polygonizer::getNormals(uint &size) {

}

const float *Polygonizer::getColors(uint &size) {

}
