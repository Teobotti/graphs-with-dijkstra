#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define R 50
#define LINE 5000
#define INFINITY 9999999
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


typedef struct graph {
    int id_grafo, camMin;
} graph;



void aggiungiGrafo(int nNodi, int k, int id_grafo, struct graph *heap, int *size);
void insert_max_heap(struct graph *array, int id_grafo, int camMin, int * size, int maxSize);
void max_heapify(struct graph *array, int *size, int i);
void swap(struct graph *a, struct graph *b);
void topKheap(struct graph *heap, int k);

void relax( int heap[][2],  int distance[][2],int u,  int v,  int w,  int n);
void min_heapify( int A[][2],  int i,  int n);
int run_dijkstra(int * graph, int nNodi);
void heap_decrease_key( int heap[][2],  int u,  int weight,  int n);
int heap_extract_min( int A[][2],  int n,  int *node_index);
void build_min_heap( int A[][2],  int n);

int main(int argc, char * argv[]) {
    int d,k;
    int check = 0;
    int id_grafo=0;
    char *read = malloc(sizeof(char)*R);

    if(fscanf(stdin, "%d %d\n", &d, &k)!=0) { // salvo d = nnodi e k=lunghezza del rank

        //struct path_node *head = NULL; //dichiaro la lista che sarà la classifica dei grafi
        struct graph *heap = (struct graph*)malloc(sizeof(struct graph)*k);
        int size = 0;


        while (check == 0) {
            if (fgets(read, R, stdin) != NULL) {
                if (read[0] == 'A' && read[1] == 'g') {
                    aggiungiGrafo(d, k, id_grafo, heap, &size);
                    id_grafo++;
                } else if (read[0] == 'T' && read[1] == 'o') {
                    //topK(head, k);
                    topKheap(heap,size);
                }
            } else {
                check = 1;
            }
        }

        free(heap);
    }


    free(read);
    return 0;
}






void topKheap(struct graph *heap, int k) {
    for(int i=0; i<k;i++){
        if(i==k-1){
            fprintf(stdout, "%d", heap[i].id_grafo);
        } else {
            fprintf(stdout, "%d ", heap[i].id_grafo);
        }
    }
    fputc('\n',stdout);
}


void insert_max_heap(struct graph *array, int id_grafo, int camMin, int * size, int maxSize) {

    if (*size == 0) {
        (array[0]).camMin = camMin;
        (array[0]).id_grafo = id_grafo;
        *size = *size + 1;
    } else if(*size == maxSize) {
        if((array[*size]).camMin > camMin) { //caso in cui nuovo grafo è maggiore di tutti i precedenti
            (array[*size]).camMin = camMin;
            (array[*size]).id_grafo = id_grafo;
        } else if((array[0]).camMin < camMin) {
            return; //caso in cui nuovo sia già maggiore del maggiore
        } else if((array[0]).camMin > camMin) {
            (array[0]).camMin = camMin;
            (array[0]).id_grafo = id_grafo;
            for (int i = *size / 2 - 1; i >= 0; i--) {
                max_heapify(array, size, i);
            }
        }
    } else {
        (array[*size]).camMin = camMin;
        (array[*size]).id_grafo = id_grafo;
        *size = *size + 1;
        for (int i = *size / 2 - 1; i >= 0; i--) {
            max_heapify(array, size, i);
        }
    }
}

void swap(struct graph *a, struct graph *b) {
    struct graph temp = *b;
    *b = *a;
    *a = temp;
}

void max_heapify(struct graph *array, int *size, int i) {
    if(*size > 1) {
        {
            int largest = i;
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            if (l < *size && (array[l]).camMin > (array[largest]).camMin)
                largest = l;
            if (r < *size && (array[r]).camMin > (array[largest]).camMin)
                largest = r;
            if (largest != i) {
                swap(&array[i], &array[largest]);
                max_heapify(array, size, largest);
            }
        }
    }
}




void aggiungiGrafo(int nNodi, int k, int id_grafo, struct graph *heap, int *size) {
    //fputs("Entro funzione aggiungi...\n", stdout);
    char *read = malloc(sizeof(char)*LINE);
    int *matrix = malloc(sizeof(int)*nNodi*nNodi);
    int signal=0;
    int sumOfPath=0;

    for(int i=0; i<nNodi;i++) {
        int cont=0;
        if(fgets(read,LINE,stdin)!=NULL) {
            char *token = strtok(read, ",");
            while (token) {
                int tk = atoi(token);
                if(tk ==0) {
                    matrix[i * nNodi + cont] = INFINITY;
                } else {
                    matrix[i * nNodi + cont] = tk;
                }
                if(cont ==0) { //voglio controllare la prima riga, se son tutti zero non sto neanche a calcolarlo, sarà 0
                    int c=0;
                    for (int j = 0; j < nNodi; j++) {
                        if(matrix[j]==0)
                            c++;
                    }
                    if(c==nNodi) {
                        signal=1;
                    }
                }

                token = strtok(NULL, ",");
                cont++;
            }
        }
    }

    //int sumOfPath = dijkstra_sum_path(matrix,nNodi);
    if(signal==0) {
        sumOfPath = run_dijkstra(matrix, nNodi);
    }

    insert_max_heap(heap, id_grafo, sumOfPath, size, k);

    fprintf(stdout,"Sum of paths is: %d for graph : %d\n", sumOfPath, id_grafo);
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

int run_dijkstra(int * graph, int nNodi) {

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
        int min_val = heap_extract_min(heap, heap_size, &u);

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
                int w = graph[u * nNodi + v];
                if (w != (int) -1 && S[v] != 1) {
                    relax(heap, distance, u, v, w, heap_size);
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

