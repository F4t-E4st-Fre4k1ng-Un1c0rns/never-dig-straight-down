#ifndef PERLINNOISE_H_INCLUDED
#define PERLINNOISE_H_INCLUDED

typedef struct{
    int* weigths;
}PERLIN, *HPERLIN;

HPERLIN generatePerlin(int seed);

double noise(HPERLIN perlin, double x, double y, double z);
double* noiseMap(HPERLIN perlin, int width, int height, double x_aplifier, double y_aplifier, double noise_aplifier);
#endif
