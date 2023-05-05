#ifndef GNUPLOTTER_H
#define GNUPLOTTER_H

#include "gnuplot-iostream.h"
#include <Shape.h>

class GnuPlotter {

    public:
    void Simulate();
    void Plot();

    private:
    Shape * m_shapeList;
    gnuplotio::Gnuplot m_gp;
}

#endif