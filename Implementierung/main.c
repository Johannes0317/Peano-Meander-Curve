#define _POSIX_C_SOURCE 199309L

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "peano_meander.h"
#include "peano_meander_rec_opt.h"
#include "peano_meander_iterativ.h"
#include "svgwriter.h"
#include "fast_power.h"

void print_Usage(const char *programName) {
    // Print the program's usage information
    printf("Usage: %s\n", programName);
    printf("Options:\n");
    printf("  -V <value>    The implementation to use. Correct values:\n");
    printf("                 0 - Iterative Method\n");
    printf("                 1 - Recursive Optimized Method\n");
    printf("                 2 - Recursive Naive Method\n");
    printf("  -B <value>    Measure the runtime of the implementation with the specified number of repetitions\n");
    printf("                Number of repetitions is an optional argument\n");
    printf("  -n <value>    Degree of the Peano Meander curve to construct\n");
    printf("  -o <filename> Output file\n");
    printf("  -h            Display this help message\n");
    printf("  --help        Display this help message\n");
}


bool hasIllegalCharacters(const char *str, const char *illegal_Chars) {
    return strpbrk(str, illegal_Chars) != NULL;
}


int main(int argc, char *argv[]) {
    bool set_B = false;
    int implementation = 2; //Standard implementation used, should be 2
    size_t repetitions = 1;
    size_t curveDegree = 3;
    char outputFile[255] = "peano_meander.svg";
    int opt;
    int option_Index;
    char *endptr = NULL;

    struct option long_Options[] = {
            {"help", no_argument, 0, 'h'},
            {0,      0,           0, 0}
    };

    errno = 0;
    // Checking if string includes "-"
    //opterr = 0; // To suppress default getopt error messages

    const char *illegal_Chars = "/?<>\\:*|\"."; //List of illegal file name characters in Linux

    while ((opt = getopt_long(argc, argv, "V:n:o:hB::", long_Options, &option_Index)) != -1) {

        switch (opt) {
            case 'V':
                implementation = strtol(optarg, &endptr, 10);
                if (endptr == optarg || *endptr != '\0') {
                    fprintf(stderr, "Invalid input for -V: %s\n", optarg);
                    print_Usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'B':
                set_B = true;
                if (optarg == NULL && optind < argc
                    && argv[optind][0] != '-'){
                    optarg = argv[optind++];
                }
                if(optarg != NULL){
                    // Handle is not present
                    repetitions = strtol(optarg, &endptr, 10);
                    if (endptr == optarg || *endptr != '\0' || repetitions <= 0) {
                        fprintf(stderr, "Invalid input for -B: %s (Repetitions must be greater than 0 or invalid input)\n", optarg);
                        print_Usage(argv[0]);
                        return EXIT_FAILURE;
                    }
                }
                break;
            case 'n':
                int tmp;
                tmp = strtol(optarg, &endptr, 10);
                curveDegree = strtol(optarg, &endptr, 10);
                if (endptr == optarg || *endptr != '\0' || tmp <= 0) {
                    if (tmp <= 0) {
                        fprintf(stderr, "Invalid input for -n: %s (Curve degree must be greater than 0 and a number)\n", optarg);
                    } else {
                        fprintf(stderr, "Invalid input for -n: %s (Invalid characters)\n", optarg);
                    }
                    print_Usage(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'o':
                if ((int) strlen(optarg) > 250) { // 5 Byte reserved for .svg
                    printf("Name of file is too long, max String Length of %d allowed\n", 250);
                    print_Usage(argv[0]);
                    return EXIT_FAILURE;
                }
                strcpy(outputFile, optarg);
                if (hasIllegalCharacters(outputFile, illegal_Chars)) {
                    fprintf(stderr, "Do not include file type at the end of output file or use illegal chars\n");
                    print_Usage(argv[0]);
                    return EXIT_FAILURE;
                }
                strcat(outputFile, ".svg");
                break;
            case 'h':
                print_Usage(argv[0]);
                return EXIT_SUCCESS;
            case '?':
                if (optopt == 'B') { // If -B option is given without a value
                    set_B = true;
                    break;
                }
            default:
                print_Usage(argv[0]);
                return EXIT_FAILURE;
        }
    }


    int *coord_X;
    int *coord_Y;

    size_t array_Size = fast_power(9, (int) curveDegree);
    coord_X = (int *) malloc(array_Size * sizeof(int));
    coord_Y = (int *) malloc(array_Size * sizeof(int));

    // Checking for NULL again seems to not work properly, checking again
    if (coord_Y == NULL || coord_X == NULL) {
        fprintf(stderr, "too big n, couldn't allocate enough memory\n");
        free(coord_X);
        free(coord_Y);
        return EXIT_FAILURE;
    }

    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t i = 0; i < repetitions; i++) {
        switch (implementation) {
            case 0:
                peano_meander(curveDegree, coord_X, coord_Y);
                break;
            case 1:
                peano_meander_V1(curveDegree, coord_X, coord_Y);
                break;
            case 2:
                peano_meander_V2(curveDegree, coord_X, coord_Y);
                break;
            default:
                printf("%i is not a viable Option for an implementation, choose between 0, 1, 2\n", implementation);
                print_Usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time = (double) (end.tv_sec) - (double) (start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
    double avg_time = time / (double) repetitions
            ;
    if (set_B == true) {
        printf("Time taken to execute in seconds : %f\n", time);
    }
    if (set_B == true && repetitions > 1) {
        printf("Average Time taken to execute in seconds : %f\n", avg_time);
    }

    printf("Implementation: %d\n", implementation);
    printf("Repetitions: %zu\n", repetitions);
    printf("Curve Degree: %zu\n", curveDegree);
    printf("Output File: %s\n", outputFile);


    writeCoordinatesToSVG(coord_X, coord_Y, array_Size, outputFile);

    free(coord_X);
    free(coord_Y);

    return EXIT_SUCCESS;
}
