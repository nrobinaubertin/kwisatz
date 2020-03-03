#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int coord2int(char coord[static 2]) {
    int c = 0;
    switch (coord[0]) {
        case 'a':
            c += 1;
            break;
        case 'b':
            c += 2;
            break;
        case 'c':
            c += 3;
            break;
        case 'd':
            c += 4;
            break;
        case 'e':
            c += 5;
            break;
        case 'f':
            c += 6;
            break;
        case 'g':
            c += 7;
            break;
        case 'h':
            c += 8;
            break;
        default:
            printf("ERROR %s\n", coord);
            exit(1);
    }
    c += 20 + (atoi(&coord[1]) - 1) * 10;
    return c;
}

char* int2coord(int k) {
    int i = (k - 21)/10;
    int j = k - 21 - 10 * i;
    char c[8] = "abcdefgh";
    char* coord = calloc(1, sizeof(char) * 2);
    coord[0] = c[j];
    coord[1] = i + 49;
    return coord;
}
