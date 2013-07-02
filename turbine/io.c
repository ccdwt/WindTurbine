#include "io.h"
void printInHex(char* arry, int len){
        int i = 0;
        for (i = 0; i < len; i++){
                printf("%3.2x" , (unsigned char)arry[i]);
        }
}

void printOut(char* arry, int len){
        int i = 0;
        for (i = 0; i < len; i++){
                printf("%2c" , (unsigned char)arry[i]);
        }
}


