// src\lib\string.c
// a simple string library source file

#include <lib/string/string.h>

void reverse(string* target){
    int x = 0;
    while(x <= target->size/2){
        char c = target->value[x];
        //target->value[x] = target->value[target->size-(x+1)];
        //target->value[target->size-(x+1)] = ;
    }
}