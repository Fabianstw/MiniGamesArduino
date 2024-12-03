/**
 * @file random_helper.h
 * @brief Header file for random number generation helper functions.
 *
 * This file contains the declaration of helper functions for generating
 * uniform random numbers within a specified range.
 */

#ifndef RANDOM_HELPER_H
#define RANDOM_HELPER_H

#include <stdlib.h>
#include <time.h>

#include "Arduino.h"
void initRandomSeed();
int randInt(int min, int max);

#endif  // RANDOM_HELPER_H