#include "perlins.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "shuffle.h"
#include "tic80.h"

void iota(int* first, int* last, int value)
{
    for (; first != last; ++first, ++value)
        *first = value;
}

int p[512] = {
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
    8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
    35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
    134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
    55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
    18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
    250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
    189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
    43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
    97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
    107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

HPERLIN generatePerlin(int seed){
    HPERLIN perlin = (HPERLIN)malloc(sizeof(PERLIN));
    if (!perlin) {
        trace("Broken memory", 2);
        return NULL;
    }
    perlin->weigths = (int*)malloc(512*sizeof(int));
    if (!perlin->weigths) {
        trace("Broken memory", 2);
        return NULL;
    }
    iota(perlin->weigths, perlin->weigths + 256, 0);
    shuffle(perlin->weigths, 256, seed);
    memcpy(perlin->weigths + 256, perlin->weigths, 256);
    return perlin;
}

double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(double t, double a, double b) {
    return a + t * (b - a);
}

double grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    // Convert lower 4 bits of hash into 12 gradient directions
    double u = h < 8 ? x : y,
        v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double noise(HPERLIN perlin, double x, double y, double z){
    int X = ((int) floor(x)) & 255;
    int Y = ((int) floor(y)) & 255;
    int Z = ((int) floor(z)) & 255;

    x-=X;
    y-=Y;
    z-=Z;

    double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	int A = perlin->weigths[X] + Y;
	int AA = perlin->weigths[A] + Z;
	int AB = perlin->weigths[A + 1] + Z;
	int B = perlin->weigths[X + 1] + Y;
	int BA = perlin->weigths[B] + Z;
	int BB = perlin->weigths[B + 1] + Z;

	double res = lerp(w, lerp(v, lerp(u, grad(perlin->weigths[AA], x, y, z), grad(perlin->weigths[BA], x-1, y, z)), lerp(u, grad(perlin->weigths[AB], x, y-1, z), grad(perlin->weigths[BB], x-1, y-1, z))),	lerp(v, lerp(u, grad(perlin->weigths[AA+1], x, y, z-1), grad(perlin->weigths[BA+1], x-1, y, z-1)), lerp(u, grad(perlin->weigths[AB+1], x, y-1, z-1),	grad(perlin->weigths[BB+1], x-1, y-1, z-1))));
	return (res + 1.0)/2.0;
}

double* noiseMap(HPERLIN perlin, int width, int height, double x_aplifier, double y_aplifier, double noise_aplifier)
{
    double* map = (double*)malloc(width * height * sizeof(double));
    if (!map) {
        trace("Broken memory", 2);
        return NULL;
    }
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            double y = (double)j / ((double)height);
            double x = (double)i / ((double)width);

            // Typical Perlin noise
            double n = noise_aplifier * noise(perlin, y_aplifier * x, y_aplifier * y, 0);
            map[i * width + j] = n - floor(n);
        }
    }
    return map;
}



