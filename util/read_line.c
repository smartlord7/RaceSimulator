#include <stdio.h>

#define BUFFER_SIZE_EXCEEDED -2
#define LINE_READ_SUCCESSFULLY 1

int read_line(char * buffer, FILE * file, int buffer_size) {
    int i = 0; 
    char chr;
    
    while ((chr = (char) fgetc(file)) != '\n' && chr != '\r' && chr != (char) EOF) {
        if (i >= buffer_size) {
            buffer[i] = '\0'; 
            
            while ((chr = (char) fgetc(file)) != '\n') {
                if (chr == EOF) break; 
            }
            
            return BUFFER_SIZE_EXCEEDED;
        }
        
        buffer[i++] = chr; 
    }

    buffer[i] = '\0';

    if (chr == (char) EOF) {
        return chr;
    }

    while ((chr = (char) fgetc(file)) == '\r' || chr == '\n');

    ungetc(chr, file);

    return LINE_READ_SUCCESSFULLY;
}