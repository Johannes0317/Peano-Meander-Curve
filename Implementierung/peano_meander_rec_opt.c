#include <stdio.h>
#include <string.h>
#include "fast_power.h"

/**
 * Functionality:
 * 1. Calculates the first ninth of the curve.
 * 2. With the use of mirroring, the next eighth parts of the current level are calculated.
 *    There are only 4 different types of mirroring.
 *    Each `transform_v` function writes all of its transformed figures in the arrays.
 *    The order of needed transformations, each is `len` long:
 *    1. v1
 *    2. v2
 *    3. v2
 *    4. v4
 *    5. v1
 *    6. v1
 *    7. v1
 *    8. v3
 *    9. v3
 * 3. The eight figures are moved recursively to their correct position.
 */

typedef int coord_t;

// Global variables are needed to keep track of the current index.
long long index_X2 = 0;
long long index_Y2 = 0;

// Saving the length of the last calculated figure for reusing it in later recursive calls.
long long last_calc_len = 0;

// Needed for termination condition: Peano Meander is fully calculated when `last_calc_len == final_size`.
long long final_size = 0;

// Copies coordinates from source to destination arrays.
void copy_coords(coord_t *x_values, coord_t *y_values, size_t dst, size_t src, size_t length) {
    size_t num_bytes = length * sizeof(coord_t);

    memcpy(&x_values[dst], &x_values[src], num_bytes);
    memcpy(&y_values[dst], &y_values[src], num_bytes);
}

// Transformation function v1.
void transform_v1(coord_t *x_values, coord_t *y_values, size_t length) {
    copy_coords(x_values, y_values, length * 4, 0, length);
    copy_coords(x_values, y_values, length * 5, 0, length);
    copy_coords(x_values, y_values, length * 6, 0, length);
}

// Transformation function v2.
void transform_v2(coord_t *x_values, coord_t *y_values, size_t length) {
    size_t j = 2 * length;

    for (size_t i = 0; i < length; i++) {
        coord_t x_diff = x_values[i];
        coord_t y_diff = y_values[i];

        x_values[i + length] = y_diff;
        y_values[i + length] = x_diff;

        x_values[i + j] = y_diff;
        y_values[i + j] = x_diff;
    }
}

// Transformation function v3.
void transform_v3(coord_t *x_values, coord_t *y_values, size_t length) {
    coord_t midpoint = (x_values[0] + x_values[length - 1]);
    size_t j = length * 7;
    size_t k = length * 8;

    for (size_t i = 0; i < length; i++) {
        coord_t x_temp = midpoint - y_values[i];
        coord_t y_temp = midpoint - x_values[i];

        x_values[i + j] = x_temp;
        y_values[i + j] = y_temp;

        x_values[i + k] = x_temp;
        y_values[i + k] = y_temp;
    }
}

// Transformation function v4.
void transform_v4(coord_t *x_values, coord_t *y_values, size_t length) {
    coord_t center_xy = x_values[length - 1] / 2;
    size_t from = 3 * length;

    for (size_t i = 0; i < length; i++) {
        x_values[i + from] = -(x_values[i] - center_xy) + center_xy;
        y_values[i + from] = -(y_values[i] - center_xy) + center_xy;
    }
}

// Prints the coordinates in the given range.
void print_coords(const coord_t *x_values, const coord_t *y_values, size_t from, size_t to) {
    for (size_t i = from; i < to; i++) {
        printf("x = %d, y = %d\n", x_values[i], y_values[i]);
    }
    printf("\n");
}

// Recursive Peano function.
void Peano(coord_t x, coord_t y, size_t degree, size_t width, int i1, int i2, coord_t *x_values, coord_t *y_values) {

    if (width == 1) {
        x_values[index_X2++] = x;
        y_values[index_Y2++] = y;
        return;
    }

    width /= 3;
    long long one_ninth = fast_power(9, (int) degree--);

    // Check if it was already calculated.
    if (last_calc_len == one_ninth) {
        // Move the already correctly mirrored/rotated figure to its correct position.
        for (long long i = 0; i < last_calc_len; ++i) {
            x_values[index_X2++] += x;
            y_values[index_Y2++] += y;
        }
    } else {
        // Recursive calls to calculate the next level of the Peano Meander curve.
        Peano((coord_t)(x + (2 * i1 * width)), (coord_t)(y + (2 * i1 * width)), degree, width, i1, i2, x_values, y_values);
        Peano((coord_t)(x + ((i1 - i2 + 1) * width)), (coord_t)(y + ((i1 + i2) * width)), degree, width, i1, 1 - i2, x_values, y_values);
        Peano((coord_t)(x + width), (coord_t)(y + width), degree, width, i1, 1 - i2, x_values, y_values);
        Peano((coord_t)(x + ((i1 + i2) * width)), (coord_t)(y + ((i1 - i2 + 1) * width)), degree, width, 1 - i1, 1 - i2, x_values, y_values);
        Peano((coord_t)(x + (2 * i2 * width)), (coord_t)(y + (2 * (1 - i2) * width)), degree, width, i1, i2, x_values, y_values);
        Peano((coord_t)(x + ((1 + i2 - i1) * width)), (coord_t)(y + ((2 - i1 - i2) * width)), degree, width, i1, i2, x_values, y_values);
        Peano((coord_t)(x + (2 * (1 - i1) * width)), (coord_t)(y + (2 * (1 - i1) * width)), degree, width, i1, i2, x_values, y_values);
        Peano((coord_t)(x + ((2 - i1 - i2) * width)), (coord_t)(y + ((1 + i2 - i1) * width)), degree, width, 1 - i1, i2, x_values, y_values);
        Peano((coord_t)(x + (2 * (1 - i2) * width)), (coord_t)(y + (2 * i2 * width)), degree, width, 1 - i1, i2, x_values, y_values);

        // Exit condition, prevents calculating unnecessary figures of the next n.
        if (one_ninth >= final_size) {
            return;
        }

        /**
         * Precalculating all needed rotations/mirrors of the current figure for later use.
         * There are 4 different kinds of transforming the current figure in order to calculate
         * the next level of the Peano Meander curve.
         * After this, the figures don't have the correct position; they still have to be moved,
         * which happens in other recursive calls.
         */
        transform_v1(x_values, y_values, one_ninth);
        transform_v2(x_values, y_values, one_ninth);
        transform_v3(x_values, y_values, one_ninth);
        transform_v4(x_values, y_values, one_ninth);

        // Saves what was the last fully calculated figure.
        last_calc_len = one_ninth;
    }
}

// Main function to calculate the Peano Meander curve.
void peano_meander_rec_opt(size_t curveDegree, size_t degree, coord_t *x, coord_t *y) {
    // Reset global variables, needed when executing more than one repetition.
    index_X2 = 0;
    index_Y2 = 0;

    last_calc_len = 0;
    final_size = fast_power(9, (int) curveDegree);

    Peano(0, 0, curveDegree, degree, 0, 0, x, y);
}

// Wrapper function to calculate the Peano Meander curve with version 1.
void peano_meander_V1(size_t degree, coord_t *x, coord_t *y) {
    size_t width = fast_power(3, (int) degree);
    peano_meander_rec_opt(degree, width, x, y);
}
