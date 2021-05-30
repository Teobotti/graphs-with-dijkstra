#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grafo {
    int id;
    int camMin;
    struct grafo *next;
} grafo;

void fun() {
    fputc('5',stdout);
}

int main(int argc, char * argv[]) {
    int d,k;
    int check = 0;
    int id_grafo=0;
    fscanf(stdin, "%d,%d", &d, &k);
    grafo *head = NULL; //dichiaro la lista che sarà la classifica dei grafi

    while(check == 0){
        
        check = 1;
    }

    return 0;
}


