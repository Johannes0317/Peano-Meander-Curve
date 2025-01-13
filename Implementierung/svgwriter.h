#ifndef TEAM128_SVGWRITER_H
#define TEAM128_SVGWRITER_H

#include <stddef.h>

typedef int coord_t;

void writeCoordinatesToSVG(coord_t *coords_X, coord_t *coords_Y, size_t size, char *filename);

#endif //TEAM128_SVGWRITER_H
