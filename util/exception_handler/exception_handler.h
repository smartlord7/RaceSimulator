<<<<<<< Updated upstream
/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACESIMULATOR_C_EXCEPTION_HANDLER_H
#define RACESIMULATOR_C_EXCEPTION_HANDLER_H

// region dependencies

#include <setjmp.h>

// endregion dependencies

// region constants

// region parsing exceptions msgs

#define ERROR_AT_LINE "ERROR WHILE PARSING LINE %d OF FILE %s!"
#define INSUFFICIENT_NUM_LINES_EXCEPTION "INSUFFICIENT_NUM_LINES_EXCEPTION: INSUFFICIENT NUMBER OF LINES AT FILE %s! IT MUST BE %d!"
#define MIN_VALUE_EXCEPTION "MIN_VALUE_EXCEPTION: FIELD %s MUST BE HIGHER OR EQUAL THAN %.2f!"
#define MAX_VALUE_EXCEPTION "MAX_VALUE_EXCEPTION: FIELD %s MUST BE LOWER OR EQUAL THAN %.2f!"
#define TOKENIZING_EXCEPTION "TOKENIZING_EXCEPTION: COULDN'T TOKENIZE FIELD %s!"
#define BUFFER_SIZE_EXCEEDED_EXCEPTION "BUFFER_SIZE_EXCEEDED_EXCEPTION: BUFFER MAX SIZE (%d) EXCEEDED!"
#define INT_CONVERSION_EXCEPTION "INT_CONVERSION_EXCEPTION: FIELD %s COULDN'T BE CONVERTED TO INT"
#define FLOAT_SIZE_EXCEEDED_EXCEPTION "FLOAT_SIZE_EXCEEDED_EXCEPTION: FIELD %s EXCEEDS FLOAT MAX SIZE!"
#define FLOAT_CONVERSION_EXCEPTION "FLOAT_CONVERSION_EXCEPTION: FIELD %s COULDN'T BE CONVERTED TO FLOAT!"
#define STRCAT_EXCEPTION "STRCAT_EXCEPTION: COULDN'T CONCATENATE STRINGS!"

// endregion parsing exceptions msgs

// region resources exceptions msgs

#define FILE_OPEN_EXCEPTION "FILE_OPEN_EXCEPTION: COULDN'T OPEN FILE WITH PATH %s!"
#define FILE_WRITE_EXCEPTION "FILE_WRITE_EXCEPTION: COULDN'T WRITE ON FILE WITH PATH %s!"
#define FILE_CLOSE_EXCEPTION "FILE_CLOSE_EXCEPTION: COULDN'T CLOSE FILE WITH PATH %s!"
#define MEMORY_ALLOCATE_EXCEPTION "MEMORY_ALLOCATE_EXCEPTION: COULDN'T PERFORM MEMORY ALLOCATION FOR %s!"
#define SHM_CREATE_EXCEPTION "SHM_CREATE_EXCEPTION: COULDN'T CREATE SHARED MEMORY!"
#define SHM_ATTACH_EXCEPTION "SHM_ATTACH_EXCEPTION: COULDN'T ATTACH MEMORY WITH ID %d!"
#define SHM_DETACH_EXCEPTION "SHM_DETACH_EXCEPTION: COULDN'T DETACH MEMORY WITH ID %d!"
#define SHM_REMOVE_EXCEPTION "SHM_REMOVE_EXCEPTION: COULDN'T REMOVE MEMORY WITH ID %d!"
#define SEM_CREATE_EXCEPTION "SEM_CREATE_EXCEPTION: COULDN'T CREATE SEMAPHORE NAMED %s!"
#define SEM_WAIT_EXCEPTION "WAIT_SEM_EXCEPTION: COULDN'T PERFORM WAIT OPERATION ON SEMAPHORE NAMED %s!"
#define SEM_POST_EXCEPTION "WAIT_SEM_EXCEPTION: COULDN'T PERFORM POST OPERATION ON SEMAPHORE NAMED %s!"
#define SEM_CLOSE_EXCEPTION "SEM_CLOSE_EXCEPTION: COULDN'T CLOSE SEMAPHORE NAMED %s!"
#define SEM_UNLINK_EXCEPTION "SEM_UNLINK_EXCEPTION: COULDN'T UNLINK SEMAPHORE NAMED %s!"
#define MUTEX_SHARE_EXCEPTION "MUTEX_SHARE_EXCEPTION: COULDN'T SHARE MUTEX BETWEEN PROCESSES!"
#define MUTEX_INITIALIZE_EXCEPTION "MUTEX_INITIALIZE_EXCEPTION: COULDN'T INITIALIZE MUTEX!"
#define MUTEX_LOCK_EXCEPTION "MUTEX_LOCK_EXCEPTION: COULDN'T LOCK MUTEX!"
#define MUTEX_UNLOCK_EXCEPTION "MUTEX_UNLOCK_EXCEPTION: COULDN'T UNLOCK MUTEX!"
#define MUTEX_CLOSE_EXCEPTION "MUTEX_CLOSE_EXCEPTION: COULDN'T DESTROY MUTEX!"
#define COND_VAR_SHARE_EXCEPTION "COND_VAR_SHARE_EXCEPTION: COULDN'T SHARE CONDITION VARIABLE BETWEEN PROCESSES!"
#define COND_VAR_INITIALIZE_EXCEPTION "COND_VAR_INITIALIZE_EXCEPTION: COULDN'T INITIALIZE CONDITION VARIABLE!"
#define COND_VAR_WAIT_EXCEPTION "COND_VAR_WAIT_EXCEPTION: COULDN'T PERFORM WAIT OPERATION ON CONDITION VARIABLE!"
#define COND_VAR_BROADCAST_EXCEPTION "COND_VAR_BROADCAST_EXCEPTION: COULDN'T BROADCAST CONDITION VARIABLE!"
#define COND_VAR_DESTROY_EXCEPTION "COND_VAR_DESTROY_EXCEPTION: COULDN'T DESTROY CONDITION VARIABLE!"
#define MSG_QUEUE_CREATE_EXCEPTION "MSG_QUEUE_CREATE_EXCEPTION: COULDN'T CREATE MESSAGE QUEUE %d!"
#define MSG_QUEUE_RECEIVE_EXCEPTION "MSG_QUEUE_RECEIVE_EXCEPTION: COULDN'T RECEIVE MESSAGE FROM MESSAGE QUEUE %d!"
#define MSG_QUEUE_SEND_EXCEPTION "MSG_QUEUE_SEND_EXCEPTION: COULDN'T SEND MESSAGE TO MESSAGE QUEUE %d!"
#define MSG_QUEUE_DESTROY_EXCEPTION "MSG_QUEUE_DESTROY_EXCEPTION: COULDN'T DESTROY MESSAGE QUEUE %d!"
#define UNNAMED_PIPE_CREATE_EXCEPTION "UNNAMED_PIPE_CREATE_EXCEPTION: COULDN'T CREATE UNNAMED PIPE!'"
#define NAMED_PIPE_CREATE_EXCEPTION "NAMED_PIPE_CREATE_EXCEPTION: COULDN'T CREATE NAMED PIPE %s!'"
#define NAMED_PIPE_OPEN_EXCEPTION "NAMED_PIPE_OPEN_EXCEPTION: COULDN'T OPEN NAMED PIPE %s!"
#define FILE_DESCRIPTOR_CLOSE_EXCEPTION "FILE_DESCRIPTOR_CLOSE_EXCEPTION: COULDN'T CLOSE FILE DESCRIPTOR %d FOR %s!"
#define PIPE_FAILED_WRITE "PIPE_FAILED_WRITE: COULDN'T WRITE ON %s PIPE!"

// endregion resources exceptions msgs

// region misc exceptions msgs

#define NOT_IMPLEMENTED_EXCEPTION "NOT_IMPLEMENTED_EXCEPTION: %s FUNCTIONALITY IS NOT IMPLEMENTED!"
#define LOG_MODE_NOT_SUPPORTED_EXCEPTION "LOG_MODE_NOT_SUPPORTED_EXCEPTION: %s LOG MODE NOT SUPPORTED!"

// endregion misc exceptions msgs

// region proc/threads exceptions msgs

#define PROCESS_CREATE_EXCEPTION "PROCESS_CREATE_EXCEPTION: COULDN'T CREATE PROCESS %s!"
#define THREAD_CREATE_EXCEPTION "THREAD_CREATE_EXCEPTION: COULDN'T CREATE THREAD %s!"
#define THREAD_JOIN_EXCEPTION "THREAD_JOIN_EXCEPTION: COULDN'T JOIN THREAD!"

// endregion proc/threads exceptions msgs

// endregion constants

// region macros

/**
 * @macro throw_and_exit
 * @brief Macro that calls the function throw_exception with the line number and the name of the file which it's called in. Also, the current process will terminate since exit_process == true.
 *
 * @param exception_msg
 * The exception message to be presented (might be formatted).
 *
 * @param __VA_ARGS__
 * The formatting arguments.
 *
 */
#define throw_and_exit(exception_msg, ...) throw_exception(__FILE__, __LINE__, true, exception_msg, __VA_ARGS__)

/**
 * @macro throw_and_stay
 * @brief Macro that calls the function throw_exception with the line number and the name of the file which it's called in. The current process will not terminate since exit_process == false.
 *
 * @param exception_msg
 * The exception message to be presented (might be formatted).
 *
 * @param __VA_ARGS__
 * The formatting arguments.
 *
 */
#define throw_and_stay(exception_msg, ...) throw_exception(__FILE__, __LINE__, false, exception_msg, __VA_ARGS__)

#define throw_if_stay(cond, exception_msg, ...) if (cond) throw_exception(__FILE__, __LINE__, false, exception_msg, __VA_ARGS__)

#define throw_if_exit(cond, exception_msg, ...) if (cond) throw_exception(__FILE__, __LINE__, true, exception_msg, __VA_ARGS__)

// endregion macros

// region global variables

extern void (* clean_func)(void *);
extern void * clean_func_params;

// endregion global variables

// region public functions prototypes

/**
 * @def exc_handler_init
 * @brief Function that initializes the exception handling mechanisms a cleanup function and its parameters.
 * if the exception implies the program to exit.
 *
 * @param clean_function
 * The cleanup function to be executed before exiting (process/thread killing, allocation freeing, etc.).
 *
 * @param clean_function_params
 * A pointer to the cleanup function params.
 *
 */
void exc_handler_init(void (* clean_function)(void *), void * clean_function_params);

/**
 * @def throw_exception
 * @brief Function that presents an exception message and exits the current process, if needed.
 *
 * @param file_name
 * The name of the file from which this function is called.
 *
 * @param line
 * The line of the file from which this function is called.
 *
 * @param exit_process
 * Specifies if the process should be exited after the exception message is shown.
 *
 * @param exception_msg
 * The exception message to be presented (might be formatted).
 *
 * @param ...
 * The formatting arguments.
 *
 */
void throw_exception(const char * file_name, int line, int exit_process, const char * exception_msg, ...);

// endregion public functions prototypes

=======
/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACESIMULATOR_C_EXCEPTION_HANDLER_H
#define RACESIMULATOR_C_EXCEPTION_HANDLER_H

// region dependencies

#include <setjmp.h>

// endregion dependencies

// region constants

// region parsing exceptions msgs

#define ERROR_AT_LINE "ERROR WHILE PARSING LINE %d OF FILE %s!"
#define INSUFFICIENT_NUM_LINES_EXCEPTION "INSUFFICIENT_NUM_LINES_EXCEPTION: INSUFFICIENT NUMBER OF LINES AT FILE %s! IT MUST BE %d!"
#define MIN_VALUE_EXCEPTION "MIN_VALUE_EXCEPTION: FIELD %s MUST BE HIGHER OR EQUAL THAN %.2f!"
#define MAX_VALUE_EXCEPTION "MAX_VALUE_EXCEPTION: FIELD %s MUST BE LOWER OR EQUAL THAN %.2f!"
#define TOKENIZING_EXCEPTION "TOKENIZING_EXCEPTION: COULDN'T TOKENIZE FIELD %s!"
#define BUFFER_SIZE_EXCEEDED_EXCEPTION "BUFFER_SIZE_EXCEEDED_EXCEPTION: BUFFER MAX SIZE (%d) EXCEEDED!"
#define INT_CONVERSION_EXCEPTION "INT_CONVERSION_EXCEPTION: FIELD %s COULDN'T BE CONVERTED TO INT"
#define FLOAT_SIZE_EXCEEDED_EXCEPTION "FLOAT_SIZE_EXCEEDED_EXCEPTION: FIELD %s EXCEEDS FLOAT MAX SIZE!"
#define FLOAT_CONVERSION_EXCEPTION "FLOAT_CONVERSION_EXCEPTION: FIELD %s COULDN'T BE CONVERTED TO FLOAT!"
#define STRCAT_EXCEPTION "STRCAT_EXCEPTION: COULDN'T CONCATENATE STRINGS!"

// endregion parsing exceptions msgs

// region resources exceptions msgs

#define FILE_OPEN_EXCEPTION "FILE_OPEN_EXCEPTION: COULDN'T OPEN FILE WITH PATH %s!"
#define FILE_WRITE_EXCEPTION "FILE_WRITE_EXCEPTION: COULDN'T WRITE ON FILE WITH PATH %s!"
#define FILE_CLOSE_EXCEPTION "FILE_CLOSE_EXCEPTION: COULDN'T CLOSE FILE WITH PATH %s!"
#define MEMORY_ALLOCATE_EXCEPTION "MEMORY_ALLOCATE_EXCEPTION: COULDN'T PERFORM MEMORY ALLOCATION FOR %s!"
#define SHM_CREATE_EXCEPTION "SHM_CREATE_EXCEPTION: COULDN'T CREATE SHARED MEMORY!"
#define SHM_ATTACH_EXCEPTION "SHM_ATTACH_EXCEPTION: COULDN'T ATTACH MEMORY WITH ID %d!"
#define SHM_DETACH_EXCEPTION "SHM_DETACH_EXCEPTION: COULDN'T DETACH MEMORY WITH ID %d!"
#define SHM_REMOVE_EXCEPTION "SHM_REMOVE_EXCEPTION: COULDN'T REMOVE MEMORY WITH ID %d!"
#define SEM_CREATE_EXCEPTION "SEM_CREATE_EXCEPTION: COULDN'T CREATE SEMAPHORE NAMED %s!"
#define SEM_WAIT_EXCEPTION "WAIT_SEM_EXCEPTION: COULDN'T PERFORM WAIT OPERATION ON SEMAPHORE NAMED %s!"
#define SEM_POST_EXCEPTION "WAIT_SEM_EXCEPTION: COULDN'T PERFORM POST OPERATION ON SEMAPHORE NAMED %s!"
#define SEM_CLOSE_EXCEPTION "SEM_CLOSE_EXCEPTION: COULDN'T CLOSE SEMAPHORE NAMED %s!"
#define SEM_UNLINK_EXCEPTION "SEM_UNLINK_EXCEPTION: COULDN'T UNLINK SEMAPHORE NAMED %s!"
#define MUTEX_SHARE_EXCEPTION "MUTEX_SHARE_EXCEPTION: COULDN'T SHARE MUTEX BETWEEN PROCESSES!"
#define MUTEX_INITIALIZE_EXCEPTION "MUTEX_INITIALIZE_EXCEPTION: COULDN'T INITIALIZE MUTEX!"
#define MUTEX_LOCK_EXCEPTION "MUTEX_LOCK_EXCEPTION: COULDN'T LOCK MUTEX!"
#define MUTEX_UNLOCK_EXCEPTION "MUTEX_UNLOCK_EXCEPTION: COULDN'T UNLOCK MUTEX!"
#define MUTEX_CLOSE_EXCEPTION "MUTEX_CLOSE_EXCEPTION: COULDN'T DESTROY MUTEX!"
#define COND_VAR_SHARE_EXCEPTION "COND_VAR_SHARE_EXCEPTION: COULDN'T SHARE CONDITION VARIABLE BETWEEN PROCESSES!"
#define COND_VAR_INITIALIZE_EXCEPTION "COND_VAR_INITIALIZE_EXCEPTION: COULDN'T INITIALIZE CONDITION VARIABLE!"
#define COND_VAR_WAIT_EXCEPTION "COND_VAR_WAIT_EXCEPTION: COULDN'T PERFORM WAIT OPERATION ON CONDITION VARIABLE!"
#define COND_VAR_BROADCAST_EXCEPTION "COND_VAR_BROADCAST_EXCEPTION: COULDN'T BROADCAST CONDITION VARIABLE!"
#define COND_VAR_DESTROY_EXCEPTION "COND_VAR_DESTROY_EXCEPTION: COULDN'T DESTROY CONDITION VARIABLE!"
#define MSG_QUEUE_CREATE_EXCEPTION "MSG_QUEUE_CREATE_EXCEPTION: COULDN'T CREATE MESSAGE QUEUE %d!"
#define MSG_QUEUE_RECEIVE_EXCEPTION "MSG_QUEUE_RECEIVE_EXCEPTION: COULDN'T RECEIVE MESSAGE FROM MESSAGE QUEUE %d!"
#define MSG_QUEUE_SEND_EXCEPTION "MSG_QUEUE_SEND_EXCEPTION: COULDN'T SEND MESSAGE TO MESSAGE QUEUE %d!"
#define MSG_QUEUE_DESTROY_EXCEPTION "MSG_QUEUE_DESTROY_EXCEPTION: COULDN'T DESTROY MESSAGE QUEUE %d!"
#define UNNAMED_PIPE_CREATE_EXCEPTION "UNNAMED_PIPE_CREATE_EXCEPTION: COULDN'T CREATE UNNAMED PIPE!'"
#define NAMED_PIPE_CREATE_EXCEPTION "NAMED_PIPE_CREATE_EXCEPTION: COULDN'T CREATE NAMED PIPE %s!'"
#define NAMED_PIPE_OPEN_EXCEPTION "NAMED_PIPE_OPEN_EXCEPTION: COULDN'T OPEN NAMED PIPE %s!"
#define FILE_DESCRIPTOR_CLOSE_EXCEPTION "FILE_DESCRIPTOR_CLOSE_EXCEPTION: COULDN'T CLOSE FILE DESCRIPTOR %d FOR %s!"
#define PIPE_FAILED_WRITE "PIPE_FAILED_WRITE: COULDN'T WRITE ON %s PIPE!"

// endregion resources exceptions msgs

// region misc exceptions msgs

#define NOT_IMPLEMENTED_EXCEPTION "NOT_IMPLEMENTED_EXCEPTION: %s FUNCTIONALITY IS NOT IMPLEMENTED!"
#define LOG_MODE_NOT_SUPPORTED_EXCEPTION "LOG_MODE_NOT_SUPPORTED_EXCEPTION: %s LOG MODE NOT SUPPORTED!"

// endregion misc exceptions msgs

// region proc/threads exceptions msgs

#define PROCESS_CREATE_EXCEPTION "PROCESS_CREATE_EXCEPTION: COULDN'T CREATE PROCESS %s!"
#define THREAD_CREATE_EXCEPTION "THREAD_CREATE_EXCEPTION: COULDN'T CREATE THREAD %s!"
#define THREAD_JOIN_EXCEPTION "THREAD_JOIN_EXCEPTION: COULDN'T JOIN THREAD!"

// endregion proc/threads exceptions msgs

// endregion constants

// region macros

/**
 * @macro throw_and_exit
 * @brief Macro that calls the function throw_exception with the line number and the name of the file which it's called in. Also, the current process will terminate since exit_process == true.
 *
 * @param exception_msg
 * The exception message to be presented (might be formatted).
 *
 * @param __VA_ARGS__
 * The formatting arguments.
 *
 */
#define throw_and_exit(exception_msg, ...) throw_exception(__FILE__, __LINE__, true, exception_msg, __VA_ARGS__)

/**
 * @macro throw_and_stay
 * @brief Macro that calls the function throw_exception with the line number and the name of the file which it's called in. The current process will not terminate since exit_process == false.
 *
 * @param exception_msg
 * The exception message to be presented (might be formatted).
 *
 * @param __VA_ARGS__
 * The formatting arguments.
 *
 */
#define throw_and_stay(exception_msg, ...) throw_exception(__FILE__, __LINE__, false, exception_msg, __VA_ARGS__)

#define throw_if_stay(cond, exception_msg, ...) if (cond) throw_exception(__FILE__, __LINE__, false, exception_msg, __VA_ARGS__)

#define throw_if_exit(cond, exception_msg, ...) if (cond) throw_exception(__FILE__, __LINE__, true, exception_msg, __VA_ARGS__)

// endregion macros

// region global variables

extern void (* clean_func)(void *);
extern void * clean_func_params;

// endregion global variables

// region public functions prototypes

/**
 * @def exc_handler_init
 * @brief Function that initializes the exception handling mechanisms a cleanup function and its parameters.
 * if the exception implies the program to exit.
 *
 * @param clean_function
 * The cleanup function to be executed before exiting (process/thread killing, allocation freeing, etc.).
 *
 * @param clean_function_params
 * A pointer to the cleanup function params.
 *
 */
void exc_handler_init(void (* clean_function)(void *), void * clean_function_params);

/**
 * @def throw_exception
 * @brief Function that presents an exception message and exits the current process, if needed.
 *
 * @param file_name
 * The name of the file from which this function is called.
 *
 * @param line
 * The line of the file from which this function is called.
 *
 * @param exit_process
 * Specifies if the process should be exited after the exception message is shown.
 *
 * @param exception_msg
 * The exception message to be presented (might be formatted).
 *
 * @param ...
 * The formatting arguments.
 *
 */
void throw_exception(const char * file_name, int line, int exit_process, const char * exception_msg, ...);

// endregion public functions prototypes

>>>>>>> Stashed changes
#endif //RACESIMULATOR_C_EXCEPTION_HANDLER_H