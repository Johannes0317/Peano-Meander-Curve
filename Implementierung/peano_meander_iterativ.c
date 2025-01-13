#include <string.h>
#include "fast_power.h"

/**
 * functionality:
 * the next figure n+1, can be calculated by rotating/mirroring and moving
 * the current figure n, 8 times
 */

typedef int coord_t;

void transform_1(coord_t *x_values, coord_t *y_values, size_t length, coord_t move_by) {
    coord_t move_by_two = move_by * 2;
    coord_t j = (coord_t) length * 4;// Index offset for the fifth figure
    coord_t k = (coord_t) length * 5;// Index offset for the sixth figure
    coord_t l = (coord_t) length * 6;// Index offset for the seventh figure
    for (size_t i = 0; i < length; ++i) {
        coord_t x = x_values[i];
        coord_t y = y_values[i];

        x_values[j + i] = x;
        y_values[j + i] = y + move_by_two;

        x_values[k + i] = x + move_by;
        y_values[k + i] = y + move_by_two;

        x_values[l + i] = x + move_by_two;
        y_values[l + i] = y + move_by_two;
    }
}

void transform_2(coord_t *x_values, coord_t *y_values, size_t length, coord_t move_by) {

    size_t j = 2 * length;// Index offset for the third figure

    for (size_t i = 0; i < length; i++) {
        coord_t x_diff = x_values[i];
        coord_t y_diff = y_values[i];

        x_values[i + length] = y_diff + move_by;
        y_values[i + length] = x_diff;

        x_values[i + j] = y_diff + move_by;
        y_values[i + j] = x_diff + move_by;
    }
}


void transform_3(coord_t *x_values, coord_t *y_values, size_t length, coord_t move_by) {
    coord_t midpoint = (x_values[0] + x_values[length - 1]);
    size_t j = length * 7;// Index offset for the eighth figure
    size_t k = length * 8;// Index offset for the ninth figure
    coord_t move_by_two = 2 * move_by;

    for (size_t i = 0; i < length; i++) {
        coord_t x_temp = midpoint - y_values[i];// Reflect the figure along the y-axis
        coord_t y_temp = midpoint - x_values[i];// Reflect the figure along the x-axis

        x_values[i + j] = x_temp + move_by_two;
        y_values[i + j] = y_temp + move_by;

        x_values[i + k] = x_temp + move_by_two;
        y_values[i + k] = y_temp;
    }
}

void transform_4(coord_t *x_values, coord_t *y_values, size_t length, coord_t move_by) {
    coord_t center_xy = x_values[length - 1] / 2;// Calculate the center of the figure
    size_t j = 3 * length;// Index offset for the transformed coordinates
    for (size_t i = 0; i < length; i++) {
        x_values[i + j] = -(x_values[i] - center_xy) + center_xy;
        y_values[i + j] = -(y_values[i] - center_xy) + center_xy + move_by;
    }
}

void peano_meander(unsigned degree, coord_t *x, coord_t *y) {
    x[0] = 0;
    y[0] = 0;
    // Iterate through each degree of the Peano curve
    for (int i = 1; i <= degree; ++i) {
        size_t one_ninth = (size_t) fast_power(9, i - 1);// Calculate the length of one-ninth of the curve
        coord_t move_by = (coord_t) fast_power(3, i - 1);// Calculate the distance to move for each transformation

        transform_1(x, y, one_ninth, move_by);
        transform_2(x, y, one_ninth, move_by);
        transform_3(x, y, one_ninth, move_by);
        transform_4(x, y, one_ninth, move_by);

    }
}