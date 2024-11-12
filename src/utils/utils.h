/**
 * @file random_helper.h
 * @brief Header file for random number generation helper functions.
 *
 * This file contains the declaration of helper functions for generating
 * uniform random numbers within a specified range.
 */

#ifndef RANDOM_HELPER_H
#define RANDOM_HELPER_H

#include <cstdlib>
#include <ctime>

void initRandomSeed();
int randInt(int min, int max);
void lightLEDForTime(int pin, int time);

#endif  // RANDOM_HELPER_H