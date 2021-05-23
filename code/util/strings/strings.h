/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 04/04/2021
*/

#ifndef RACE_SIMULATOR_C_STRINGS_H
#define RACE_SIMULATOR_C_STRINGS_H

// region constants

#define BUF_SIZE 512

// endregion constants

// region public functions prototypes

/**
 * @def string
 * @brief Function that allocates memory for a string with the specified size.
 *
 * @param size
 * The size of the string.
 *
 * @return a pointer to the created string.
 *
 * @throws MemoryAllocationException if the malloc call for the string returns a NULL pointer.
 *
 */
char * string(int size);

/**
 * @def append
 * @brief Function that appends a given string to another.
 *
 * @param first
 * The first string.
 *
 * @param second
 * The second string.
 *
 * @return a pointer to the final string.
 *
 * @throws StrcatException if an internal error occurred whilst attempting to concatenate two strings.
 *
 */
extern char * append(char * first, char * second);

/**
 * @def append_f
 * @brief Function that appends a string to a already formatted string.
 *
 * @param first
 * String to store the result.
 *
 * @param fmt
 * Format of the string.
 *
 * @param ...
 * Variable parameters to append.
 *
 * @return the appended string.
 */
extern char * append_f(char * first, const char * fmt, ...);

/**
 * @def trim_string
 * @brief Function that removes white spaces at the beginning and at the end of a string.
 *
 * @param buffer
 * String to be trimmed.
 *
 * @param size
 * Size of the string.
 *
 * @return trimmed string.
 */
extern char * trim_string(char *buffer);

/**
 * @def starts_with
 * @brief Function that checks if a string is contained on another string.
 *
 * @param string
 * String who might contain the other one.
 *
 * @param string2
 * String who might be contained on the other one.
 *
 * @return false if the second string is not contained on the other one.
 *         true if the second string is contained on the first one.
 */
extern int starts_with(char * string, char * string2);

/**
 * @def starts_with_ignore_case
 * @brief Function that checks if a string is contained on another string ignoring the case.
 *
 * @param string
 * String who might contain the other one.
 *
 * @param string2
 * String who might be contained on the other one.
 *
 * @return false if the second string is not contained on the other one.
 *         true if the second string is contained on the first one.
 */
extern int starts_with_ignore_case(char * string, char * string2);

/**
 * @def repeat_str
 * @brief Function that repeats a string N times.
 *
 * @param str
 * String to repeated.
 *
 * @param times
 * Number of repetitions.
 *
 * @return a string containing the N repetitions of the original string.
 */
extern char * repeat_str(const char * str, int times);

/**
 * @def is_number
 * @brief Function that verifies if the given string consists of a number.
 *
 * @param str
 * String to be verified.
 *
 * @return false if the string does not consist of a number.
 *         true if the string consists of a number.
 */
extern int is_number(const char * str);

// region public functions prototypes

#endif //RACE_SIMULATOR_C_STRINGS_H
