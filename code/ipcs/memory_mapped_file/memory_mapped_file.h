/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef IPCS_MAPPED_FILE_H
#define IPCS_MAPPED_FILE_H

// region constants

#define true 1
#define false 0
#define MMAP_FILE_RESIZE_FACTOR 2

// endregion constants

// region public functions prototypes

/**
 * @def create_mmap_file
 * @brief Function that maps a file into memory.
 *
 * @param fd
 * File descriptor to the file to be mapped into memory.
 *
 * @param size
 * Pointer to the size of the file. It is obtained in this function.
 *
 * @return
 * Pointer to the location of the file mapping if successful.
 *
 * @throws FileStatsException if it is not possible to obtain the data about the file to be mapped.
 *         MemoryMapCreateException if it is not possible to map the file into memory.
 */
extern char * create_mmap_file(int fd, size_t * size);

/**
 * @def write_mmap
 * @brief Function that writes on a memory mapped file.
 *
 * @param mmap_addr
 * Pointer to the location of the memory mapped file.
 *
 * @param buffer
 * Contents to be written on the file.
 *
 * @param fd
 * File descriptor of the mapped file.
 *
 * @param file_size
 * Size of the file. It is obtained in this function.
 *
 * @throws FileTruncateException if it is not possible to truncate the memory mapped file when expanding to the needed.
 *         MemoryMapRemapException if it is not possible to remap the current memory mapped file.
 */
void write_mmap(char * mmap_addr, size_t * curr_size, size_t * max_size, int fd,  char * buffer, size_t buf_size) ;

/**
 * @def destroy_mmap_file
 * @brief Function that destroys a memory mapped file.
 *
 * @param mmap
 * Pointer to the location of the memory mapped file.
 *
 * @param fd
 * File descriptor of the file that is mapped on memory.
 *
 * @param file_size
 * Size of the memory mapping and needed for the physical file.
 *
 * @throws MemoryMapException if it is not possible to unmap the memory mapped file.
 *         FileTruncateException if it is not possible to truncate the file size to the real needed size.
 */
extern void destroy_mmap_file(char * mmap, int fd, size_t file_size);

// endregion public functions prototypes

#endif //IPCS_MAPPED_FILE_H
