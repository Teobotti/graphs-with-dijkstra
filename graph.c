#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R 50
#define LINE 3000
#define INFINITY 99999999
#define HEAP_PARENT(i) (i/2)
#define HEAP_LEFT(i) (2*i)
#define HEAP_RIGHT(i) (2*i+1)
#define START_NODE 0
#define EXP 10

typedef struct graph {
    int id_grafo, camMin;
} graph;

typedef struct dist {
    int distanza, predecessor;
}dist;

typedef struct min_heap {
    int weight, node;
} min_heap;

void aggiungiGrafo(int nNodi, int k, int id_grafo, struct graph *heap, int *size);
void topKheap(struct graph *heap, int size);

void insert_max_heap(struct graph *array, int id_grafo, int camMin, int * size, int maxSize);
void max_heapify(struct graph *array, int *size, int i);
void swap(struct graph *a, struct graph *b);

int dijkstra_sum_path(int * graph, int nNodi);
void relax( struct  min_heap *heap,  struct dist * distance,int u,  int v,  int w,  int n);
void min_heapify( struct min_heap * A,  int i,  int n) ;
int heap_extract_min(struct min_heap * heap, int size, int *node_index) ;
void build_min_heap( struct min_heap * A,  int n);
void heap_decrease_key( struct  min_heap *heap,  int u,  int weight,  int n);

int main() {
    int d,k;
    int check = 0;
    int id_grafo=0;
    char *riga = malloc(sizeof(char) * R);

    if(fscanf(stdin, "%d %d\n", &d, &k)!=0) {
        struct graph *heap = (struct graph*)malloc(sizeof(struct graph)*k);
        int size = 0;
        while (check == 0) {
            if (fgets(riga, R, stdin) != NULL) {
                if (riga[0] == 'A' && riga[1] == 'g') {
                    aggiungiGrafo(d, k, id_grafo, heap, &size);
                    id_grafo++;
                } else if (riga[0] == 'T' && riga[1] == 'o') {
                    topKheap(heap,size);
                }
            } else {
                check = 1;
            }
        }
        free(heap);
    }

    free(riga);
    return 0;
}

void topKheap(struct graph *heap, int size) {

    for(int i=0; i<size;i++){
        if(i==size-1){
            fprintf(stdout, "%d", heap[i].id_grafo);
        } else {
            fprintf(stdout, "%d ", heap[i].id_grafo);
        }
    }
    fputc('\n',stdout);
}

void aggiungiGrafo(int nNodi, int k, int id_grafo, struct graph *heap, int *size) {
    char *read = malloc(sizeof(char)*LINE);
    int *matrix = malloc(sizeof(int)*nNodi*nNodi);
    int  all_equal=0;
    int no_path=0;
    int sumOfPath=0;

    for(int i = 0; i<nNodi; i++) {
        if(fgets(read,LINE,stdin)!=NULL) {
            int num = 0;
            int cont = 0;
            for(int j = 0; j< LINE;j++) {
                if((int)read[j]-'0' >= 0 && (int)read[j]-'0' <= 9) {
                    if(num ==0) {
                        num = num + (int)read[j]-'0';
                    } else {
                        num =  num * EXP;
                        num = num + (int)read[j]-'0';
                    }
                } else if(read[j] == ',') {
                    if(num == 0) {
                        matrix[i*nNodi + cont] = INFINITY;
                    } else {
                        matrix[i*nNodi + cont] = num;
                    }
                    if(i==0 && cont > 0) {
                        if(num==0) {
                            no_path++;
                        }
                    }
                    if(i!=cont) {
                        if(num == matrix[1]) {
                            all_equal++;
                        }
                    }
                    num = 0;
                    cont++;
                } else if(read[j]=='\n') {
                    if(num == 0) {
                        matrix[i*nNodi + cont] = INFINITY;
                    } else {
                        matrix[i*nNodi + cont] = num;
                    }
                    if(i==0 && cont > 0) {
                        if(num==0) {
                            no_path++;
                        }
                    }
                    if(i!=cont) {
                        if(num == matrix[1]) {
                            all_equal++;
                        }
                    }
                    break;
                }
            }
        }
    }

    if(all_equal == nNodi*nNodi-nNodi) {
        sumOfPath = matrix[1]*nNodi-1;
    } else if(no_path == nNodi-1){ //nessun nodo raggiungibile da 0 -> tutta la prima riga composta da 0
        sumOfPath = 0;
    } else {
        sumOfPath = dijkstra_sum_path(matrix,nNodi);
    }
    if(*size == k && heap[0].camMin < sumOfPath) {
        free(read);
        free(matrix);
        return;
    } else {
        insert_max_heap(heap,id_grafo,sumOfPath,size,k);
        free(read);
        free(matrix);
    }
}


void swappy (struct min_heap * a, struct min_heap * b) {
    struct min_heap temp = *b;
    *b = *a;
    *a = temp;
}

void min_heapify( struct min_heap * A,  int i,  int n) {
    int l = HEAP_LEFT(i);
    int r = HEAP_RIGHT(i);
    int smallest = 0;

    if(l <= n && A[l-1].weight < A[i-1].weight) {
        smallest = l;
    } else {
        smallest = i;
    }
    if(r <= n && A[r-1].weight < A[smallest-1].weight) {
        smallest = r;
    }
    if(smallest != i) {
        swappy(&A[i-1], &A[smallest -1]);
        min_heapify(A, smallest, n);
    }
}



void build_min_heap( struct min_heap * A,  int n) {
    for(int i = (n/2); i > 0; i--) {
        min_heapify(A, i, n);
    }
}

int heap_extract_min(struct min_heap * heap, int size, int *node_index) {
    if (size < 1) {
        printf("error: underflow\n");
        return -1;
    }
    int min = heap[0].weight;
    int u = heap[0].node;

    if (size > 1) {
        heap[0].weight = heap[size - 1].weight;
        heap[0].node = heap[size - 1].node;
        build_min_heap(heap, size - 1);
    }
    *node_index = u;
    return min;
}

void heap_decrease_key( struct  min_heap *heap,  int u,  int weight,  int n) {
    for(int i = 0; i < n; i++) {
        if(heap[i].node == u) {
            if(weight < heap[i].weight) {
                heap[i].weight = weight;
                int parent = HEAP_PARENT(i);
                while(i > 0 && heap[parent].weight > heap[i].weight) {
                    swappy(&heap[parent], &heap[i]);
                    i = parent;
                    parent = HEAP_PARENT(i);
                }
            }
            break;
        }
    }
}

void relax( struct  min_heap *heap,  struct dist * distance,int u,  int v,  int w,  int n) {

    int weight = distance[u].distanza + w;
    if(distance[v].distanza > weight) {
        distance[v].distanza = weight;
        distance[v].predecessor = u;
        heap_decrease_key(heap, v, weight, n);
    }
}


int dijkstra_sum_path(int * graph, int nNodi) {

    int *visited = malloc(sizeof (int )*nNodi);
    struct dist *distance = (struct dist *)malloc(sizeof (struct dist)*nNodi);
    struct min_heap *heap = (struct min_heap *)malloc(sizeof (struct min_heap)*nNodi);

    //Inizialize single source
    for (int i = 0; i < nNodi; i++) {
        distance[i].distanza = INFINITY;
        distance[i].predecessor = INFINITY;
        heap[i].weight = graph[i];
        heap[i].node = i;
        visited[i] = 0;
    }
    distance[START_NODE].distanza = 0;
    distance[START_NODE].predecessor = 0;
    heap[START_NODE].weight = 0;

    int heap_size = nNodi;
    build_min_heap(heap, heap_size);

    while (heap_size > 0) {
        int u = -1;
        int min_val = heap_extract_min(heap, heap_size, &u);
        visited[u] = 1;
        heap_size--;
        if (min_val == (int) -1) {
            break;
        } else {
            for (int v = 0; v < nNodi; v++) {
                if(u!=v) { //ignoro autoanelli
                    int w = 0;
                    if(graph[u*nNodi+v]==0) {
                        w = INFINITY;
                    } else {
                        w = graph[u * nNodi + v];
                    }
                    if(w < INFINITY) {
                        if (w != (int) -1 && visited[v] != 1) {
                            relax(heap, distance, u, v, w, heap_size);
                        }
                    }
                }
            }
        }
    }
    int sumOfpath = 0;
    for (int i = 0; i < nNodi; i++) {
        if(distance[i].distanza < INFINITY) {
            sumOfpath = sumOfpath + distance[i].distanza;
        }
    }

    free(distance);
    free(heap);
    free(visited);

    return sumOfpath;
}

void insert_max_heap(struct graph *array, int id_grafo, int camMin, int * size, int maxSize) {

    if (*size == 0) {
        (array[0]).camMin = camMin;
        (array[0]).id_grafo = id_grafo;
        *size = *size + 1;
    } else if(*size == maxSize) {
        if((array[0]).camMin < camMin) {
            return; //caso in cui nuovo sia già maggiore del max
        } else if((array[0]).camMin > camMin) { // caso in cui è minore del max ma ho già riempito tutto l'heap
            (array[0]).camMin = camMin;
            (array[0]).id_grafo = id_grafo;
            int i = 0;
            int l = HEAP_LEFT(i) + 1;
            int r = HEAP_RIGHT(i) + 1;
            //procedura detta heap-increase-key
            while((l <= maxSize && r <= maxSize) && ((array[i]).camMin < (array[l]).camMin || (array[i]).camMin < (array[r]).camMin)) {
                if((array[l]).camMin > (array[r]).camMin) {
                    swap(&array[i], &array[l]);
                    i = l;
                    l = HEAP_LEFT(i) + 1;
                    r = HEAP_RIGHT(i) + 1;
                } else if( (array[r]).camMin > (array[l]).camMin) {
                    swap(&array[i], &array[r]);
                    i = r;
                    l = HEAP_LEFT(i) + 1;
                    r = HEAP_RIGHT(i) + 1;
                }
            }

        }
    } else {
        (array[*size]).camMin = camMin;
        (array[*size]).id_grafo = id_grafo;
        *size = *size + 1;
        if(array[*size-1].camMin > array[*size-2].camMin) {
            for (int i = *size / 2 - 1; i >= 0; i--) {
                max_heapify(array, size, i);
            }
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
            int l = HEAP_LEFT(i) + 1;
            int r = HEAP_RIGHT(i) + 1;
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


