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


void aggiungiGrafo(int nNodi, int k, int id_grafo, struct graph *heap, int *size);
void topKheap(struct graph *heap, int k);

void insert_max_heap(struct graph *array, int id_grafo, int camMin, int * size, int maxSize);
void max_heapify(struct graph *array, int *size, int i);
void swap(struct graph *a, struct graph *b);

int dijkstra_sum_path(int * graph, int nNodi);

void relax( int heap[][2],  int distance[][2],int u,  int v,  int w,  int n);
void min_heapify( int A[][2],  int i,  int n);
void heap_decrease_key( int heap[][2],  int u,  int weight,  int n);
int heap_extract_min( int A[][2],  int n,  int *node_index);
void build_min_heap( int A[][2],  int n);

int main() {
    int d,k;
    int check = 0;
    int id_grafo=0;
    char *riga = malloc(sizeof(char) * R);

    if(fscanf(stdin, "%d %d\n", &d, &k)!=0) { // salvo d = nnodi e k=lunghezza del rank
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
        //insert(heap, id_grafo, sumOfPath, size, k);
        insert_max_heap(heap,id_grafo,sumOfPath,size,k);
        free(read);
        free(matrix);
    }
}


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
        //swap
        int temp1 = A[i - 1][0];
        int temp2 = A[i - 1][1];

        A[i - 1][0] = A[smallest - 1][0];
        A[i - 1][1] = A[smallest - 1][1];
        A[smallest - 1][0] = temp1;
        A[smallest - 1][1] = temp2;

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


int dijkstra_sum_path(int * graph, int nNodi) {

    int heap[nNodi][2];
    int visited[nNodi];
    int distance[nNodi][2];

    for (int i = 0; i < nNodi; i++) {
        distance[i][0] = INFINITY;
        distance[i][1] = INFINITY;
        heap[i][0] = graph[i];
        heap[i][1] = i;
        visited[i] = 0;
    }

    distance[START_NODE][0] = 0;
    distance[START_NODE][1] = 0;
    heap[START_NODE][0] = 0;
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
        if(distance[i][0] < INFINITY) {
            sumOfpath = sumOfpath + distance[i][0];
        }
    }
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
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            //procedura detta heap-increase-key
            while((l <= maxSize && r <= maxSize) && ((array[i]).camMin < (array[l]).camMin || (array[i]).camMin < (array[r]).camMin)) {
                if((array[l]).camMin > (array[r]).camMin) {
                    swap(&array[i], &array[l]);
                    i = l;
                    l = 2 * i + 1;
                    r = 2 * i + 2;
                } else if( (array[r]).camMin > (array[l]).camMin) {
                    swap(&array[i], &array[r]);
                    i = r;
                    l = 2 * i + 1;
                    r = 2 * i + 2;
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


