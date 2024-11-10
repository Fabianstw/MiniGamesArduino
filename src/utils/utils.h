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

/**
 * @brief Generates a random integer within a specified range.
 *
 * This function generates a random integer between the specified minimum
 * and maximum values, inclusive. It uses the standard library's rand()
 * function to generate the random number.
 *
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @return A random integer between min and max, inclusive.
 */
int randInt(int min, int max);

#endif // RANDOM_HELPER_H