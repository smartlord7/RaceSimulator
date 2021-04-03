//
// Created by jfgar on 03/04/2021.
//
#include "str_concat.h"
#include <string.h>
#include "error_handler.h"

#define true 1
#define false 0

char * str_concat(char * stringA, char * stringB, char * error_type, char * error_context){

    if(strcat(stringA, stringB) == NULL){
        throw_error_end_stay(error_type, error_context);
    }

    return stringA;
}
