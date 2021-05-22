/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef MESSAGE_QEUE_C_MEMORY_MAPPED_FILE_H
#define MESSAGE_QEUE_C_MEMORY_MAPPED_FILE_H

// region public functions prototypes

/**
 * @def create_mmap_file
 * @brief Function that maps a file into memory.
 *
 * @param fd
 * File descriptor to the file to be mapped into memory.
 *
 * @param size
 * Size of the file. It is obtained in this function.
 *
 * @return
 * Pointer to the location of the mapping if the mapping is successful.
 *
 * @throws FileStatsException if it is not possible to obtain data about the file to be mapped.
 *         MemoryMapCreateException if it is not possible to map the file into memory.
 */
extern char * create_mmap_file(int fd, size_t * size);

/**
 * @def write_mmap
 * @brief Function that writes on memory mapped file.
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
 * @return EXIT_SUCCESS if the writing operation was successful.
 *
 * @throws FileTruncateException if it is not possible to truncate the memory mapped file when expanding the needed size.
 *         MemoryMapRemapException if it is not possible to remap the current memory mapped file.
 */
extern int write_mmap(char * mmap_addr, char * buffer, int fd, size_t * file_size);

/**
 * @def destroy_mmap
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
 * @return EXIT_SUCESS if the unmmaping is successful.
 *
 * @throws MemoryMapException if it is not possible to unmap the memory mapped file.
 *         FileTruncateException if it is not possible to truncate the file size to the real needed size.
 */
extern int destroy_mmap(char * mmap, int fd, size_t file_size);

// endregion public functions prototypes

#endif //MESSAGE_QEUE_C_MEMORY_MAPPED_FILE_H
