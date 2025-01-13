#include <math.h>
#include <stddef.h>
#include "fast_power.h"

typedef int coord_t;

// index_X and index_Y are two global variables used to track the index of the current coordinate in the x and y arrays respectively.

size_t index_X = 0;
size_t index_Y = 0;

/**
 *  Implementation Source: https://new.eurasip.org/Proceedings/Eusipco/Eusipco1998/sessions/T%20A/TA%20P-7/487/spacefil1.pdf
 *  calculates each line given a start and endpoint
 *  as there are nine quadrants, every quadrant needs 9 calculations
 */


void peano_not_Optimized(int coord_X, int coord_Y, unsigned width, int startPoint, int endPoint, coord_t *x, coord_t *y) {
    /**
     *  function calls itself until width is 1, which is the smallest pattern
     *  stores coords generated in function call in array
     */

    if (width == 1) {
        x[index_X++] = coord_X;
        y[index_Y++] = coord_Y;
        return;
    }

    width /= 3;
    peano_not_Optimized(coord_X + (2 * startPoint * width), coord_Y + (2 * startPoint * width), width, startPoint,
                       endPoint, x, y); // calls itself and calculating new coords for X and Y
    peano_not_Optimized(coord_X + ((startPoint - endPoint + 1) * width), coord_Y + ((startPoint + endPoint) * width),
                       width, startPoint, 1 - endPoint, x, y);
    peano_not_Optimized(coord_X + width, coord_Y + width, width, startPoint, 1 - endPoint, x, y);
    peano_not_Optimized(coord_X + ((startPoint + endPoint) * width), coord_Y + ((startPoint - endPoint + 1) * width),
                       width, 1 - startPoint, 1 - endPoint, x, y);
    peano_not_Optimized(coord_X + (2 * endPoint * width), coord_Y + (2 * (1 - endPoint) * width), width, startPoint,
                       endPoint, x, y);
    peano_not_Optimized(coord_X + ((1 + endPoint - startPoint) * width), coord_Y + ((2 - startPoint - endPoint) * width),
                       width, startPoint, endPoint, x, y);
    peano_not_Optimized(coord_X + (2 * (1 - startPoint) * width), coord_Y + (2 * (1 - startPoint) * width), width,
                       startPoint, endPoint, x, y);
    peano_not_Optimized(coord_X + ((2 - startPoint - endPoint) * width), coord_Y + ((1 + endPoint - startPoint) * width),
                       width, 1 - startPoint, endPoint, x, y);
    peano_not_Optimized(coord_X + (2 * (1 - endPoint) * width), coord_Y + (2 * endPoint * width), width, 1 - startPoint,
                       endPoint, x, y);
}

/**
 *  peano_meander is a function used to generate the Peano space-filling curve pattern
 *  takes three parameters - the degree, which is the size of the area to generate the pattern in
 *  and two arrays of coord_t to store the generated pattern
 *  calls peano_Notoptimized which also takes in start for x and y coord, the size of curve, the start and endpoint and the array for x and y
 */

void peano_meander_V2(unsigned degree, coord_t *x, coord_t *y) {
    index_X = 0;
    index_Y = 0;
    size_t degree_Power = fast_power(3, degree);
    peano_not_Optimized(0, 0, degree_Power, 0, 0, x, y);
}

