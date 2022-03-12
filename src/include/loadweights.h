#pragma once
#ifndef LOADWEIGHTS_H
#define LOADWEIGHTS_H

#include <stdint.h>

// loads floats from a file
// p is float array pointer
// n is array length pointer
uint32_t loadVals(float *p, uint32_t *n, char *filename);

// resets all data in file to 0.000000
int resetVals(char *filename);

int saveVals(float *p, uint32_t n, char *filename);

#endif