#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R 50
#define LINE 5000
#define INFINITY 9999999

typedef struct grafo { //utilizzare una lista per la classifica
    int id;
    int camMin;
} grafo;

typedef struct path_list {
    grafo grafo1;
    struct path_list * next;
} path_list;

void aggiungiGrafo(int nNodi, int k, int id_grafo);
int dijkstra_sum_path(int *G, int nNodi, int startnode);
void topK();
void add_to_rank(int k,int id_grafo,int sumOfPath, grafo **rank);



int main(int argc, char * argv[]) {
    int d,k;
    int check = 0;
    int id_grafo=0;
    char *read = malloc(sizeof(char)*R);



    fscanf(stdin, "%d %d\n", &d, &k); // salvo d = nnodi e k=lunghezza del rank



    struct path_list *head = NULL; //dichiaro la lista che sarà la classifica dei grafi

    while(check == 0){
        if(fgets(read,R,stdin)!=NULL) {
            if (read[0] == 'A' && read[1] == 'g') {
                aggiungiGrafo(d, k, id_grafo);
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

void topK() {
    fputs("Ranking is...\n", stdout);
}

void add_to_rank(int k,int id_grafo,int sumOfPath, grafo **rank) { //soluzione naive aggiungo al rank nella posizione corretta

}



void aggiungiGrafo(int nNodi, int k, int id_grafo) {
    fputs("Entro funzione aggiungi...\n", stdout);
    char *read = malloc(sizeof(char)*LINE);
    int *matrix = malloc(sizeof(int)*nNodi*nNodi);

    for(int i=0; i<nNodi;i++) {
        int cont=0;
        fgets(read,LINE,stdin);
        char *token = strtok(read, ",");
        while(token) {
            int tk = atoi(token);
            matrix[i * nNodi + cont] = tk;
            token = strtok(NULL, ",");
            cont++;
        }
    }

    int sumOfPath = dijkstra_sum_path(matrix,nNodi,0);
    add_to_rank(k,id_grafo,sumOfPath);

    free(read);
    free(matrix);
}

int dijkstra_sum_path(int *G, int nNodi, int startnode) {

    int cost[nNodi][nNodi],distance[nNodi],pred[nNodi];
    int visited[nNodi],count,mindistance,nextnode,i,j,sumOfPath=0;
    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen so far
    //create the cost matrix
    for(i=0; i < nNodi; i++)
        for(j=0; j < nNodi; j++)
            if(G[i * nNodi + j]==0)
                cost[i][j]=INFINITY;
            else
                cost[i][j]=G[i * nNodi + j];
    //initialize pred[],distance[] and visited[]
    for(i=0; i < nNodi; i++) {
        distance[i]=G[startnode*0+i]; //lascio 0 i nodi non raggiungibili come da specifica
        pred[i]=startnode;
        visited[i]=0;
    }
    distance[startnode]=0;
    visited[startnode]=1;
    count=1;
    while(count < nNodi - 1)
    {
        mindistance=INFINITY;
        //nextnode gives the node at minimum distance
        for(i=0; i < nNodi; i++)
            if(distance[i]<mindistance && !visited[i]) {
                mindistance=distance[i];
                nextnode=i;
            }
        //check if a better path exists through nextnode
        visited[nextnode]=1;
        for(i=0; i < nNodi; i++)
            if(!visited[i])
                if(mindistance+cost[nextnode][i]<distance[i])
                {
                    distance[i]=mindistance+cost[nextnode][i];
                    pred[i]=nextnode;
                }
        count++;
    }
    for(i=0; i<nNodi;i++) {
        sumOfPath = sumOfPath + distance[i];
    }
    fprintf(stdout,"The sum of paths from 0 to nodes is %d\n", sumOfPath);

    return sumOfPath;
}
