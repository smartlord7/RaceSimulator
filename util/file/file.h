/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef UTIL_FILE_H
#define UTIL_FILE_H

//region constants

#define true 1
#define false 0

//endregion constants

//region public functions prototypes

/**
 * @def open_file
 * @brief Function that opens a given file.
 *
 * @param file_name
 * Name of the file to open.
 *
 * @param mode
 * Mode of the file opening.
 *
 * @return file descriptor for the opened file.
 *
 * @throws FileOpenException if it is not possible to open the file.
 */
extern int open_file(const char * file_name, int mode);

/**
 * @def close_fd
 * @brief Function that closes a given file descriptor.
 *
 * @param fd
 * File descriptor to be closed.
 *
 * @throws FileDescriptorCloseException if it is not possible to close the file descriptor.
 */
extern void close_fd(int fd);

/**
 * @def read_stream
 * @brief Function that reads from a given stream.
 *
 * @param fd
 * File descriptor for the stream.
 *
 * @param buffer
 * Buffer to store the contents read.
 *
 * @param size
 * Amount of bytes to be read from stream.
 *
 * @throws StreamReadException if it is not possible to read from the stream.
 */
extern void read_stream(int fd, void * buffer, size_t size);

/**
 * @def write_stream
 * @brief Function that writes on a stream.
 *
 * @param fd
 * File descriptor for the stream.
 *
 * @param buffer
 * Buffer with the contents to be written.
 *
 * @param size
 * Amount of bytes to be written on the stream.
 *
 * @throws StreamWriteException if it is not possible to write on the stream.
 */
extern void write_stream(int fd, void * buffer, size_t size);

//endregion public functions prototypes

#endif //UTIL_FILE_H
