// src\include\lib\string\string.h
// a simple string library header

#ifndef LIB_STRING_H
#define LIB_STRING_H

typedef struct{
    const char* value;
    int size;
}string;

void reverse(string*);

void str_cmp(string*,string*);

#endif