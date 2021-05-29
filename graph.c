#include <stdio.h>


void fun() {
    fputc(5,stdout);
}



int main(int argc, char * argv[]) {
    printf("Hello, World!\n");
    fun();
    return 0;
}


