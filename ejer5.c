#include <stdio.h>
#include <stdlib.h>

#define vertex int

typedef struct graph *Graph;

typedef struct node *link;

struct graph {
    int V;
    int A;
    link *adj;
};

struct node {
    vertex w;
    link next;
};

static link NEWnode( vertex w, link next) {
    link a = (link) malloc( sizeof (struct node));
    a->w = w;
    a->next = next;
    return a;
}

Graph GRAPHinit( int V) {
    Graph G = (Graph) malloc( sizeof *G);
    G->V = V;
    G->A = 0;
    G->adj = (link *) malloc( V * sizeof (link));
    for (vertex v = 0; v < V; ++v)
        G->adj[v] = NULL;
    return G;
}

void GRAPHinsertArc(Graph G, vertex v, vertex w) {
    for (link a = G->adj[v]; a != NULL; a = a->next)
        if (a->w == w) return;  
        
    G->adj[v] = NEWnode(w, G->adj[v]);
    G->adj[w] = NEWnode(v, G->adj[w]);
    G->A++;
}

void GRAPHshow(Graph G) {
    for (vertex v = 0; v < G->V; v++) {
        printf("Vertice %d :", v);
        for (link a = G->adj[v]; a != NULL; a = a->next)
            printf("%d--", a->w);
        printf("\n");
    }
}

void GRAPHdestroy(Graph G) {
    if (G == NULL) return;
    for (vertex v = 0; v < G->V; v++) {
        link a = G->adj[v];
        while (a != NULL) {
            link next = a->next;
            free(a);
            a = next;
        }
    }


    free(G->adj);
    free(G);
}


static int cnt;
int pre[1000];
static int indent = 0;

static void printIndent() {
    for (int i = 0; i < indent; i++)
        printf("  ");
}

static void dfsR(Graph G, vertex v) {
    pre[v] = cnt++;
    printIndent();
    printf("%d dfsR(G,%d)\n", v, v);
    indent++;

    for (link a = G->adj[v]; a != NULL; a = a->next) {
        vertex w = a->w;
        if (pre[w] == -1) {
            printIndent();
            printf("%d-%d dfsR(G,%d)\n", v, w, w);
            dfsR(G, w);
        }
    }

    indent--;
    printIndent();
    printf("%d\n", v);
}

void GRAPHdfs( Graph G){ //llamada general
    cnt = 0;
    for (vertex v = 0; v < G->V; ++v)
        pre[v] = -1;
    for (vertex v = 0; v < G->V; ++v)
        if (pre[v] == -1)
            dfsR( G, v); // comienza nueva etapa

}

Graph GRAPHreadFromFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: no se pudo abrir el archivo '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    int V;
    if (fscanf(fp, "%d", &V) != 1 || V <= 0) {
        fprintf(stderr, "Error: formato inválido en el archivo.\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    Graph G = GRAPHinit(V);
    int v, w;
    for (int i = 0; i < V; i++) {
        if (fscanf(fp, "%d", &v) != 1) break;
        while (fscanf(fp, "%d", &w) == 1)
            GRAPHinsertArc(G, v, w);
        clearerr(fp);
    }

    fclose(fp);
    return G;
}

int main(void) {
    char filename[100];
    printf("Ingrese el nombre del archivo de adyacencia: ");
    scanf("%s", filename);

    Graph G = GRAPHreadFromFile(filename);

    printf("\nRastreo DFS:\n");
    GRAPHdfs(G);

    GRAPHdestroy(G);

    return 0;
}
