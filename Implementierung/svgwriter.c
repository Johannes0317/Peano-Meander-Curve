#include <stdio.h>
//#include <string.h>
//#include <math.h>

typedef int coord_t;

void writeCoordinatesToSVG(coord_t *coords_X, coord_t *coords_Y, size_t size, char *filename) {


    /**
     *  as coords are flipped we are flipping x and y coords before writing to svg file
     */



    coord_t x_diff = coords_X[size - 1] - coords_X[0]; // Gets max width on x axix
    coord_t movto = coords_X[0]; //start at 0,0

    for (size_t i = 0; i < size; i++) {

        coords_X[i] = (-coords_X[i] + 2 * movto + x_diff) * 2; //moving x coords and multiplying by 3 to make it bigger
        coords_Y[i] = (-coords_Y[i] + 2 * movto + x_diff) * 2; //moving y coords and multiplying by 3 to make it bigger
    }


    FILE *svgFile = fopen(filename, "w");
    if (svgFile == NULL) {
        printf("Failed to open the SVG file for writing.\n");
        return;
    }
    fprintf(svgFile, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n", 1980,
            1080);
    for (int i = 0; i < size - 1; i++) {
        fprintf(svgFile, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n", coords_X[i],
                coords_Y[i], coords_X[i + 1], coords_Y[i + 1]);
    }
    fprintf(svgFile, "</svg>");

    fclose(svgFile);
}