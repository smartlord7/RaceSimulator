#ifndef RACESIMULATOR_C_TO_FLOAT_H
#define RACESIMULATOR_C_TO_FLOAT_H

// region constants

#define FLOAT_SIZE_EXCEEDED -1
#define FLOAT_CONVERSION_FAILURE 0
#define FLOAT_CONVERSION_SUCCESS 1

// endregion constants

// region public functions prototypes

/**
 * @def to_float
 * @brief Function that attempts to convert a string to a flot.
 *
 * @param num_string
 * The string to be converted to float.
 *
 * @param num_float
 * The pointer to the float variable that will hold the converted value.
 *
 * @return FLOAT_SIZE_EXCEEDED if the float size is exceeded.
 *         FLOAT_CONVERSION_FAILURE if an internal error occurred whilst converting the string.
 *         FLOAT_CONVERSION_SUCCESS if the float conversion was successful.
 */
int to_float(char * num_string, float * num_float);

// endregion public functions prototypes

#endif //RACESIMULATOR_C_TO_FLOAT_H
