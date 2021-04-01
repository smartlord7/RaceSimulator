#include <stdio.h>

int read_line(char * buffer, FILE * file, int buffer_size) {
    int i = 0; 
    char chr;
    
    while ((chr = (char) fgetc(file)) != '\n' && chr != '\r' && chr != (char) EOF) {
        if (i >= buffer_size) {
            buffer[i] = '\0'; 
            
            while ((chr = (char) fgetc(file)) != '\n') {
                if (chr == EOF) break; 
            }
            
            return -2;
        }
        
        buffer[i++] = chr; 
    }

    buffer[i] = '\0';

    if (chr == (char) EOF) {
        return chr;
    }

    while ((chr = (char) fgetc(file)) == '\r' || chr == '\n');

    ungetc(chr, file);

    return 1;
}

