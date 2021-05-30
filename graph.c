#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE 10000

typedef struct grafo {
    int id;
    int camMin;
    struct grafo *next;
} grafo;

void fun() {
    fputc('5',stdout);
}

void aggiungiGrafo(int nNodi) {
    char *read = malloc(sizeof(char)*10000);
    int *matrix = malloc(sizeof(int)*nNodi*nNodi);

    for(int i=0; i<nNodi;i++) {
        int cont=0;
        fgets(read,sizeof read,stdin);
        char *token = strtok(read, ",");
        while(token) {
            int tk = atoi(token);
            matrix[i*nNodi+cont] = tk;
            token = strtok(NULL, ",");
            cont++;
        }
    }
    /* qui stampo la mia matrice di adiacenza adattata ad una dimensione.
    for(int j=0; j<nNodi*nNodi;j++) {
        fprintf(stdout, "%d ", matrix[j]);
    }
    fputc('\n', stdout);
    */

    free(read);
    free(matrix);
    //fputs("Aggiungo un grafo\n", stdout);
}

void topK() {
    fputs("Ranking is...\n", stdout);
}

int main(int argc, char * argv[]) {
    int d,k;
    int check = 0;
    int id_grafo=0;
    char *read = malloc(sizeof(char)*10000);


    fscanf(stdin, "%d,%d\n", &d, &k); // salvo d = nnodi e k=lunghezza del rank
    grafo *head = NULL; //dichiaro la lista che sarà la classifica dei grafi
    while(check == 0){
        if(fgets(read,sizeof read,stdin)!=NULL) {
            if (read[0] == 'A' && read[1] == 'g') {
                aggiungiGrafo(d);
                id_grafo++;
            } else if (read[0] == 'T' && read[1] == 'o') {
                topK();
            }
        } else {
            check = 1;
        }
    }

    free(read);
    return 0;
}


