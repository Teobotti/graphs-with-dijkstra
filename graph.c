#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define R 50
#define LINE 5000
#define INFINITY 9999999
#define START_NODE 0
/*
 *  OPEN TESTS
 *  input_1 ---> output_1 YES
 *  input_2 ---> output_2 YES
 *  input_3 ---> output_3 NO
 *  input_4 ---> output_4 YES
 *  input_5 ---> output_5 YES
 *  input_6 ---> output_6 NO
 */

typedef struct path_node {
    int id;
    int camMin;
    struct path_node * next;
} path_node;

void aggiungiGrafo(int nNodi, int k, int id_grafo, struct path_node **lista_grafi);
int dijkstra_sum_path(int *G, int nNodi);
void topK(struct path_node* head, int k);
void add_to_rank(int k,int id_grafo,int sumOfPath, struct path_node **head);
void push(struct path_node** head, int id_grafo, int sumOfPath);
void append(struct path_node** head_ref, int id_grafo, int sumOfPath);
void insert(struct path_node** prev_node, int id_grafo, int sumOfPath);


int main(int argc, char * argv[]) {
    int d,k;
    int check = 0;
    int id_grafo=0;
    char *read = malloc(sizeof(char)*R);

    if(fscanf(stdin, "%d %d\n", &d, &k)!=0) { // salvo d = nnodi e k=lunghezza del rank

        struct path_node *head = NULL; //dichiaro la lista che sarà la classifica dei grafi

        while (check == 0) {
            if (fgets(read, R, stdin) != NULL) {
                if (read[0] == 'A' && read[1] == 'g') {
                    aggiungiGrafo(d, k, id_grafo, &head);
                    id_grafo++;
                } else if (read[0] == 'T' && read[1] == 'o') {
                    topK(head, k);
                }
            } else {
                check = 1;
            }
        }
    }

    free(read);
    return 0;
}

void topK(struct path_node* head, int k) {
    //fputs("Ranking is...\n", stdout);

    for(int i=0; i<k; i++) {
        if(head!=NULL) {
            fprintf(stdout, "%d ", head->id);
            head = head->next;
        }
    }
    fputc('\n',stdout);

}

void push(struct path_node** head, int id_grafo, int sumOfPath) {
    struct path_node* new_node = (struct path_node*) malloc(sizeof(struct path_node));
    new_node->id  = id_grafo;
    new_node->camMin = sumOfPath;
    new_node->next = (*head);
    (*head) = new_node;
}

void append(struct path_node** head_ref, int id_grafo, int sumOfPath) {
    struct path_node* new_node = (struct path_node*) malloc(sizeof(struct path_node));
    struct path_node *last = *head_ref;
    new_node->id  = id_grafo;
    new_node->camMin = sumOfPath;
    new_node->next = NULL;
    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
}

void insert(struct path_node** prev_node, int id_grafo, int sumOfPath) {
    struct path_node* new_node =(struct path_node*) malloc(sizeof(struct path_node));
    new_node->id  = id_grafo;
    new_node->camMin = sumOfPath;
    new_node->next = (*prev_node)->next;

    (*prev_node)->next = new_node;
}


void add_to_rank(int k,int id_grafo,int sumOfPath, struct path_node **head) { //soluzione naive aggiungo al rank nella posizione corretta

    struct path_node *traveler = *head;

    if (id_grafo == 0) {
        push(head, id_grafo, sumOfPath);
        return;
    }

    if (sumOfPath < (*head)->camMin) {
        push(head, id_grafo, sumOfPath);
        return;
    } else if (sumOfPath > (*head)->camMin && (*head)->next == NULL) {
        append(head, id_grafo, sumOfPath);
        return;
    }

    while ((traveler->next != NULL)) {
        if (traveler->next->camMin >= sumOfPath) {
            //traveler = next per inserirlo dopo
            insert(&traveler, id_grafo, sumOfPath);
            return;
        } else {
            traveler = traveler->next;
        }
    }

    append(&traveler,id_grafo,sumOfPath);
}

void aggiungiGrafo(int nNodi, int k, int id_grafo, struct path_node **lista_grafi) {
    //fputs("Entro funzione aggiungi...\n", stdout);
    char *read = malloc(sizeof(char)*LINE);
    int *matrix = malloc(sizeof(int)*nNodi*nNodi);

    for(int i=0; i<nNodi;i++) {
        int cont=0;
        if(fgets(read,LINE,stdin)!=NULL) {
            char *token = strtok(read, ",");
            while (token) {
                int tk = atoi(token);
                matrix[i * nNodi + cont] = tk;
                token = strtok(NULL, ",");
                cont++;
            }
        }
    }

    int sumOfPath = dijkstra_sum_path(matrix,nNodi);
    add_to_rank(k,id_grafo,sumOfPath, lista_grafi);

    //fprintf(stdout,"Sum of paths is: %d\n", sumOfPath);



    free(read);
    free(matrix);
}

int dijkstra_sum_path(int *G, int nNodi) {

    int cost[nNodi][nNodi],distance[nNodi];
    int visited[nNodi],count,mindistance,i,j,sumOfPath=0;
    int nextnode = 0;

    for(i=0; i < nNodi; i++) {
        for (j = 0; j < nNodi; j++) {
            if (G[i * nNodi + j] == 0)
                cost[i][j] = INFINITY;
            else
                cost[i][j] = G[i * nNodi + j];
        }
    }

    for(i=0; i < nNodi; i++) {
        distance[i]=cost[START_NODE][i]; //lascio 0 i nodi non raggiungibili come da specifica
        visited[i]=0;
    }
    distance[START_NODE]=0;
    visited[START_NODE]=1;
    count=1;
    while(count < nNodi - 1)
    {
        mindistance=INFINITY;
        for(i=0; i < nNodi; i++) {
            if (distance[i] < mindistance && !visited[i]) {
                mindistance = distance[i];
                nextnode = i;
            }
        }
        visited[nextnode]=1;
        for(i=0; i < nNodi; i++) {
            if (!visited[i])
                if (mindistance + cost[nextnode][i] < distance[i]) {
                    distance[i] = mindistance + cost[nextnode][i];
                }
        }
        count++;
    }
    for(i=0; i<nNodi;i++) {
        if(distance[i]< INFINITY) {
            sumOfPath = sumOfPath + distance[i];
        }
    }
    //fprintf(stdout,"The sum of paths from 0 to nodes is %d\n", sumOfPath);

    return sumOfPath;
}
