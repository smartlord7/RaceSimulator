/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 18/05/2021
*/

#ifndef IPCS_NAMED_PIPE_H
#define IPCS_NAMED_PIPE_H

// region constants

#define true 1
#define false 0

// endregion constants

// region public functions prototypes

/**
 * @def create_unn_pipe
 * @brief Function that creates an unnamed pipe.
 *
 * @param fds
 * Array with size 2 that receives the R/W file descriptors of the pipe.
 *
 * @throws UnnamedPipeCreateException if it is not possible to create the unnamed pipe.
 *
 */
extern void create_unn_pipe(int fds[2]);

/**
 * @def create_named_pipe
 * @brief Function that creates a named pipe.
 *
 * @param pipe_name
 * Name to assign to named pipe.
 *
 * @throws NamedPipeCreateException if it is not possible to create the named pipe.
 *
 */
extern void create_named_pipe(const char * pipe_name);

/**
 * @def destroy_named_pipe
 * @brief Function that destroys a named pipe;
 *
 * @param pipe_name
 * The system name assigned to named pipe.
 *
 */
extern void destroy_named_pipe(const char * pipe_name);

// endregion public functions prototypes

#endif //IPCS_NAMED_PIPE_H