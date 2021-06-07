#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define R 50
#define LINE 3000
#define INFINITY 99999999
#define HEAP_PARENT(i) (i/2)
#define HEAP_LEFT(i) (2*i)
#define HEAP_RIGHT(i) (2*i+1)
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


void relax( int heap[][2],  int distance[][2],int u,  int v,  int w,  int n);
void min_heapify( int A[][2],  int i,  int n);
void heap_decrease_key( int heap[][2],  int u,  int weight,  int n);
int heap_extract_min( int A[][2],  int n,  int *node_index);
void build_min_heap( int A[][2],  int n);


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
    int  all_equal=0, no_path=0;
    int sumOfPath=0;

    for(int i=0; i<nNodi;i++) {
        int cont=0;
        if(fgets(read,LINE,stdin)!=NULL) {
            char *token = strtok(read, ",");
            while (token) {
                int tk = (int)strtol(token,NULL,10);
                if(i!=0 || cont!=0) {
                    if(tk == matrix[0]){ //se sono tutti uguali
                        all_equal++;
                    }
                }
                if(i==0 && cont > 0) { //se sono tutti 0 sulla prima riga -> nessun nodo raggiungibile
                    if(tk == 0) {
                        no_path++;
                    }
                }
                if(tk ==0) {
                    matrix[i * nNodi + cont] = INFINITY;
                } else {
                    matrix[i * nNodi + cont] = tk;
                }
                token = strtok(NULL, ",");
                cont++;
            }
        }
    }

    if(no_path != nNodi-1) {
        if(all_equal == nNodi*nNodi-1) {
            sumOfPath = matrix[1]*(nNodi-1);
        } else {
            sumOfPath = dijkstra_sum_path(matrix, nNodi);
        }
    }

    add_to_rank(k,id_grafo,sumOfPath, lista_grafi);

    //fprintf(stdout,"Sum of paths is: %d\n", sumOfPath);



    free(read);
    free(matrix);
}

//---------------------------------------------//

void min_heapify( int A[][2],  int i,  int n) {
    int l = HEAP_LEFT(i);
    int r = HEAP_RIGHT(i);
    int smallest = 0;

    if(l <= n && A[l-1][0] < A[i-1][0]) {
        smallest = l;
    } else {
        smallest = i;
    }
    if(r <= n && A[r-1][0] < A[smallest-1][0]) {
        smallest = r;
    }
    if(smallest != i) {
        int temp1 = A[i-1][0];
        int temp2 = A[i-1][1];

        A[i-1][0] = A[smallest-1][0];
        A[i-1][1] = A[smallest-1][1];
        A[smallest-1][0] = temp1;
        A[smallest-1][1] = temp2;

        min_heapify(A, smallest, n);
    }
}

void build_min_heap( int A[][2],  int n) {
    for(int i = (n/2); i > 0; i--) {
        min_heapify(A, i, n);
    }
}

int heap_extract_min( int A[][2],  int n,  int *node_index) {
    if (n < 1) {
        printf("error: underflow\n");
        return -1;
    }

    int min = A[0][0];
    int u = A[0][1];

    if (n > 1) {
        A[0][0] = A[n - 1][0];
        A[0][1] = A[n - 1][1];
        build_min_heap(A, n - 1);
        //min_heapify(A, 1, n - 1);
    }
    *node_index = u;
    return min;
}

void heap_decrease_key( int heap[][2],  int u,  int weight,  int n) {
    for(int i = 0; i < n; i++) {
        if(heap[i][1] == u) {
            if(weight < heap[i][0]) {
                heap[i][0] = weight;
                int parent = HEAP_PARENT(i);
                while(i > 0 && heap[parent][0] > heap[i][0]) {
                    //swap
                    int temp1 = heap[parent][0];
                    int temp2 = heap[parent][1];
                    heap[parent][0] = heap[i][0];
                    heap[parent][1] = heap[i][1];
                    heap[i][0] = temp1;
                    heap[i][1] = temp2;
                    i = parent;
                    parent = HEAP_PARENT(i);
                }
            }
            break;
        }
    }
}

void relax( int heap[][2],  int distance[][2],int u,  int v,  int w,  int n) {

    int weight = distance[u][0] + w;

    if(distance[v][0] > weight) {
        distance[v][0] = weight;
        distance[v][1] = u;
        heap_decrease_key(heap, v, weight, n);
    }
}

int dijkstra_sum_path(int * graph, int nNodi) { //chiamata 32 volte

    int heap[nNodi][2];
    int S[nNodi];
    int distance[nNodi][2];

    for (int i = 0; i < nNodi; i++) {
        distance[i][0] = INFINITY;
        distance[i][1] = INFINITY;
        heap[i][0] = graph[i];
        heap[i][1] = i;
        S[i] = 0;
    }

    distance[START_NODE][0] = 0;
    distance[START_NODE][1] = 0;
    heap[START_NODE][0] = 0;
    int heap_size = nNodi;
    build_min_heap(heap, heap_size);

    while (heap_size > 0) {
        int u = -1;
        int min_val = heap_extract_min(heap, heap_size, &u); //chiamata nnodi volte

        if (u == -1) {
            printf("unexpected case\n");
            break;
        }

        S[u] = 1;
        heap_size--;
        if (min_val == (int) -1) {
            break;
        } else {
            for (int v = 0; v < nNodi; v++) {
                if(u!=v) { //ignoro autoanelli
                    int w = graph[u * nNodi + v];
                    if(w < INFINITY) {
                        if (w != (int) -1 && S[v] != 1) {
                            relax(heap, distance, u, v, w, heap_size); //chiamata al più per ogni nodo
                        }
                    }
                }
            }
        }
    }

    int sumOfpath = 0;
    for (int i = 0; i < nNodi; i++) {
        if(distance[i][0] < INFINITY) {
            sumOfpath = sumOfpath + distance[i][0];
        }
    }
    return sumOfpath;

}
