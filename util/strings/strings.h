<<<<<<< Updated upstream
/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 04/04/2021
*/

#ifndef RACE_SIMULATOR_C_STRINGS_H
#define RACE_SIMULATOR_C_STRINGS_H

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
char * append(char * first, char * second);

char * trim_string(char * buffer, int size);

// region public functions prototypes

#endif //RACE_SIMULATOR_C_STRINGS_H
=======
/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 04/04/2021
*/

#ifndef RACE_SIMULATOR_C_STRINGS_H
#define RACE_SIMULATOR_C_STRINGS_H

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
char * append(char * first, char * second);

char * trim_string(char * buffer, int size);

// region public functions prototypes

#endif //RACE_SIMULATOR_C_STRINGS_H
>>>>>>> Stashed changes
