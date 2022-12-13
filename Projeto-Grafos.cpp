#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    int vertOut, vertIn, weight;
} ARESTAS;

typedef struct No
{
    int N;
    int weight;
    struct No *next;
} No;

typedef struct
{
    No *first, *last;
    int size;
} List;

typedef struct
{
    int numVertices, numArestas, valued;
    char type;
    ARESTAS *arestas;

    int **matrixAdj;
    List *listAdj;

} GRAFO;

typedef struct
{
    int *items;
    int front;
    int rear;
} Queue;

int cmpWeight(const void *a, const void *b)
{
    int l = ((ARESTAS *)a)->weight;
    int r = ((ARESTAS *)b)->weight;
    return (l - r);
}

void clearScreen();
GRAFO openFile(GRAFO graph);
void createFile();

GRAFO createMatrix_Adj(GRAFO graph);
GRAFO convertMatrix_List(GRAFO graph);
void convertList_Matrix(GRAFO graph);
void calcDegrees(GRAFO graph);
void kruskal(GRAFO graph);
void dijkstra(GRAFO graph);

void dfs(GRAFO graph);
int dfsVisit(GRAFO graph, int vert, int cor[], int desc[], int fin[], int father[], int time);

void bfs(GRAFO graph);

void printList(GRAFO graph);
void printMatrix(int dimension, int **matrix);
void PrintPath(GRAFO graph, int s, int v, int father[]);

int find(int subset[], int i);
void join(int subset[], int v1, int v2);
int minDistance(int dist[], bool sptSet[], int size);
void enqueue(Queue *q, int value, GRAFO graph);
int dequeue(Queue *q);

int main(void)
{
    GRAFO graph;
    graph.listAdj = NULL;

    int opc;
    do
    {
        printf("1: Abrir o Arquivo                           2: Criar Arquivo\n");
        printf("3: Converter Matriz para Lista               4: Converter Matriz para Lista\n");
        printf("5: Calcular graus                            6: Arvore Geradora Minima\n");
        printf("7: Caminho mais Curto                        8: Busca Largura\n");
        printf("9: Busca Profundidade                        0: Sair\n");
        scanf("%d", &opc);
        switch (opc)
        {
        case 1:
            graph = openFile(graph);
            graph = createMatrix_Adj(graph);
            break;
        case 2:
            createFile();
            break;
        case 3:
            clearScreen();
            graph = convertMatrix_List(graph);
            break;
        case 4:
            clearScreen();
            convertList_Matrix(graph);
            break;
        case 5:
            clearScreen();
            if (graph.listAdj == NULL)
            {
                graph = convertMatrix_List(graph);
            }
            calcDegrees(graph);
            break;
        case 6:
            clearScreen();
            kruskal(graph);
            break;
        case 7:
            clearScreen();
            dijkstra(graph);
            break;
        case 8:
            clearScreen();
            bfs(graph);
            break;
        case 9:
            clearScreen();
            dfs(graph);
            break;
        case 0:

            printf("Encerrando Sistema...");
            break;
        default:
            printf("Valor invalido\n");
        }

    } while (opc != 0);
}

GRAFO openFile(GRAFO graph)
{
    FILE *in;
    int rep = 0;
    char fileName[50];
    printf("Qual o nome do arquivo a ser aberto\n");

    do
    {
        scanf("%s", fileName);
        strcat(fileName, ".txt");
        if ((in = fopen(fileName, "r+")) == NULL)
        {
            printf("Arquivo %s nao encontrado\n", fileName);
        }
        else
        {
            rep = 1;
        }
    } while (rep == 0);
    
    fscanf(in, "%d %d %c %d", &graph.numVertices, &graph.numArestas, &graph.type, &graph.valued);
    graph.arestas = (ARESTAS *)malloc(sizeof(ARESTAS) * graph.numArestas);
    for (int i = 0; i < graph.numArestas; i++)
    {
        if (graph.valued == 1)
        {
            fscanf(in, "%d %d %d", &graph.arestas[i].vertIn, &graph.arestas[i].vertOut, &graph.arestas[i].weight);
        }
        else
        {
            fscanf(in, "%d %d", &graph.arestas[i].vertIn, &graph.arestas[i].vertOut);
        }
    }
    clearScreen();
    printf("------------------------");
    printf("Arquivo aberto com exito");
    printf("------------------------\n");
    fclose(in);
    return graph;
}

void createFile()
{
    FILE *out;
    GRAFO newGraph;

    char fileName[50];
    printf("Salvar o Arquivo como: ");
    scanf("%s", fileName);
    strcat(fileName, ".txt");
    out = fopen(fileName, "w+");

    printf("Digite o numero de vertices ");
    scanf("%d", &newGraph.numVertices);
    printf("Digite o numero de arestas ");
    scanf("%d", &newGraph.numArestas);

    do
    {
        printf("Digite o tipo de grafo\nG para grafo nao dirigido e D para grafo dirigido\n");
        scanf(" %c", &newGraph.type);
    } while (newGraph.type != 'G' && newGraph.type != 'D');

    do
    {
        printf("Digite 1 para grafo valorado e 0 para grafo nao valorado\n");
        scanf("%d", &newGraph.valued);
    } while (newGraph.valued != 0 && newGraph.valued != 1);

    fprintf(out, "%d %d %c %d\n", newGraph.numVertices, newGraph.numArestas, newGraph.type, newGraph.valued);

    newGraph.arestas = (ARESTAS *)malloc(sizeof(ARESTAS) * newGraph.numArestas);
    clearScreen();
    for (int i = 0; i < newGraph.numArestas; i++)
    {
        if (newGraph.valued == 1)
        {
            printf("Digite a %d aresta\n<VerticeEntrada> <VerticeSaida> <Peso>\n", i + 1);
            scanf("%d %d %d", &newGraph.arestas[i].vertIn, &newGraph.arestas[i].vertOut, &newGraph.arestas[i].weight);
            fprintf(out, "%d %d %d \n", newGraph.arestas[i].vertIn, newGraph.arestas[i].vertOut, newGraph.arestas[i].weight);
        }
        else
        {
            printf("Digite a %d aresta\n<VerticeEntrada> <VerticeSaida>\n", i + 1);
            scanf("%d %d", &newGraph.arestas[i].vertIn, &newGraph.arestas[i].vertOut);
            fprintf(out, "%d %d \n", newGraph.arestas[i].vertIn, newGraph.arestas[i].vertOut);
        }
    }
    clearScreen();
    printf("------------------------");
    printf("Arquivo criado com exito");
    printf("------------------------\n");
    fclose(out);
    free(newGraph.arestas);
}

GRAFO createMatrix_Adj(GRAFO graph)
{

    graph.matrixAdj = (int **)malloc((graph.numVertices) * sizeof(int *));
    for (int i = 0; i < graph.numVertices; i++)
        graph.matrixAdj[i] = (int *)malloc((graph.numVertices) * sizeof(int));

    for (int i = 0; i < graph.numVertices; i++)
    {
        for (int j = 0; j < graph.numVertices; j++)
        {
            graph.matrixAdj[i][j] = 0;
        }
    }

    for (int i = 0; i < graph.numArestas; i++)
    {
        if (graph.type == 'G' && graph.valued == 0)
        {
            graph.matrixAdj[graph.arestas[i].vertIn - 1][graph.arestas[i].vertOut - 1] += 1;
            graph.matrixAdj[graph.arestas[i].vertOut - 1][graph.arestas[i].vertIn - 1] += 1;
        }
        else if (graph.type == 'G' && graph.valued == 1)
        {
            graph.matrixAdj[graph.arestas[i].vertIn - 1][graph.arestas[i].vertOut - 1] = graph.arestas[i].weight;
            graph.matrixAdj[graph.arestas[i].vertOut - 1][graph.arestas[i].vertIn - 1] = graph.arestas[i].weight;
        }
        else if (graph.type == 'D' && graph.valued == 0)
        {
            graph.matrixAdj[graph.arestas[i].vertIn - 1][graph.arestas[i].vertOut - 1] += 1;
        }
        else
        {
            graph.matrixAdj[graph.arestas[i].vertIn - 1][graph.arestas[i].vertOut - 1] = graph.arestas[i].weight;
        }
    }
    return graph;
}

GRAFO convertMatrix_List(GRAFO graph)
{

    graph.listAdj = (List *)malloc((graph.numVertices) * sizeof(List));

    for (int i = 0; i < graph.numVertices; i++)
    {
        graph.listAdj[i].first = NULL;
        graph.listAdj[i].last = NULL;
        graph.listAdj[i].size = 0;
    }

    for (int i = 0; i < graph.numVertices; i++)
    {
        for (int j = 0; j < graph.numVertices; j++)
        {
            if (graph.matrixAdj[i][j] != 0)
            {

                if (graph.valued == 0)
                {
                    for (int k = 0; k < graph.matrixAdj[i][j]; k++)
                    {
                        No *new_Element = (No *)malloc(sizeof(No));
                        new_Element->N = j + 1;
                        new_Element->next = NULL;

                        if (graph.listAdj[i].first == NULL)
                        {
                            graph.listAdj[i].first = new_Element;
                            graph.listAdj[i].last = new_Element;
                        }
                        else
                        {
                            graph.listAdj[i].last->next = new_Element;
                            graph.listAdj[i].last = new_Element;
                        }

                        graph.listAdj[i].size = graph.listAdj[i].size + 1;
                    }
                }
                else
                {
                    No *new_Element = (No *)malloc(sizeof(No));
                    new_Element->N = j + 1;
                    new_Element->weight = graph.matrixAdj[i][j];
                    new_Element->next = NULL;

                    if (graph.listAdj[i].first == NULL)
                    {
                        graph.listAdj[i].first = new_Element;
                        graph.listAdj[i].last = new_Element;
                    }
                    else
                    {
                        graph.listAdj[i].last->next = new_Element;
                        graph.listAdj[i].last = new_Element;
                    }

                    graph.listAdj[i].size = graph.listAdj[i].size + 1;
                }
            }
        }
    }
    printMatrix(graph.numVertices, graph.matrixAdj);
    printList(graph);
    return graph;
}

void convertList_Matrix(GRAFO graph)
{

    int **convertMatrix;
    convertMatrix = (int **)malloc((graph.numVertices) * sizeof(int *));
    for (int i = 0; i < graph.numVertices; i++)
        convertMatrix[i] = (int *)malloc((graph.numVertices) * sizeof(int));

    for (int i = 0; i < graph.numVertices; i++)
    {
        for (int j = 0; j < graph.numVertices; j++)
        {
            convertMatrix[i][j] = 0;
        }
    }
    for (int i = 0; i < graph.numVertices; i++)
    {
        No *list = graph.listAdj[i].first;

        for (int j = 0; j < graph.listAdj[i].size; j++)
        {
            if (graph.valued == 0)
            {
                convertMatrix[i][list->N - 1] += 1;
            }
            else
            {
                convertMatrix[i][list->N - 1] += list->weight;
            }
            list = list->next;
        }
    }
    printList(graph);
    printMatrix(graph.numVertices, convertMatrix);
}

void printList(GRAFO graph)
{
    printf("--------LISTA DE ADJACENCIAS--------\n");

    for (int i = 0; i < graph.numVertices; i++)
    {
        No *list = graph.listAdj[i].first;
        printf("[%d] -> ", i + 1);
        for (int j = 0; j < graph.listAdj[i].size; j++)
        {
            if (list->next == NULL)
            {
                if (graph.type == 0)
                {
                    printf("[%d] -> ", list->N);
                    printf("[NULL]\n");
                }
                else
                {
                    printf("[%d| %2d] -> ", list->N, list->weight);
                    printf("[NULL]\n");
                }
            }
            else
            {
                if (graph.type == 0)
                    printf("[%d] -> ", list->N);
                else
                    printf("[%d| %2d] -> ", list->N, list->weight);
                list = list->next;
            }
        }
    }
    printf("------------------------------------\n");
}

void printMatrix(int dimension, int **matrix)
{
    printf("--------MATRIZ DE ADJACENCIAS--------\n");
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------------------\n");
}

void calcDegrees(GRAFO graph)
{
    int degree = 0;
    printf("---------GRAUS DOS VERTICES---------\n");

    if (graph.type == 'G')
    {
        for (int i = 0; i < graph.numVertices; i++)
        {
            printf("Grau do vetice %d = %d\n", i + 1, graph.listAdj[i].size);
        }
    }
    else
    {
        for (int i = 0; i < graph.numVertices; i++)
        {
            for (int j = 0; j < graph.numVertices; j++)
            {
                if (graph.matrixAdj[i][j] != 0)
                {
                    degree++;
                }
            }
            printf("Grau de saida vetice %d = %d\n", i + 1, degree);
            degree = 0;
        }

        for (int j = 0; j < graph.numVertices; j++)
        {
            for (int i = 0; i < graph.numVertices; i++)
            {
                if (graph.matrixAdj[i][j] != 0)
                {
                    degree++;
                }
            }
            printf("Grau de entrada vetice %d = %d\n", j + 1, degree);
            degree = 0;
        }
    }
    printf("------------------------------------\n");
}

void kruskal(GRAFO graph)
{
    int sizeTree = 0;
    if (graph.type != 'G' || graph.valued == 0)
    {
        printf("Nao é possivel realizar essa operacao nesse grafo\n");
    }
    else
    {
        ARESTAS *tree;
        tree = (ARESTAS *)malloc(sizeof(ARESTAS));

        qsort(graph.arestas, graph.numArestas, sizeof(ARESTAS), cmpWeight);

        int subset[graph.numArestas];

        memset(subset, -1, sizeof(int) * graph.numArestas);
        for (int i = 0; i < graph.numArestas; i++)
        {
            int v1 = find(subset, graph.arestas[i].vertIn);
            int v2 = find(subset, graph.arestas[i].vertOut);

            if (v1 != v2)
            {
                sizeTree++;
                tree = (ARESTAS *)realloc(tree, sizeof(ARESTAS) * sizeTree);
                tree[sizeTree - 1] = graph.arestas[i];
                join(subset, v1, v2);
            }
        }

        int resultado = 0;

        for (int i = 0; i < sizeTree; i++)
        {
            printf("(%d, %d) = %d\n", tree[i].vertIn, tree[i].vertOut, tree[i].weight);
            resultado += tree[i].weight;
        }
        printf("Peso total: %d\n", resultado);
    }
}

int find(int subset[], int i)
{
    if (subset[i] == -1)
        return i;
    return find(subset, subset[i]);
}

void join(int subset[], int v1, int v2)
{
    int v1_set = find(subset, v1);

    int v2_set = find(subset, v2);
    subset[v1_set] = v2_set;
}

void dijkstra(GRAFO graph)
{
    int src, dest;
    int dist[graph.numVertices];
    bool sptSet[graph.numVertices];

    for (int i = 0; i < graph.numVertices; i++)
    {
        dist[i] = 100000;
        sptSet[i] = false;
    }

    printf("Qual vertice sera utilizado como fonte\n");
    scanf("%d", &src);
    printf("Qual vertice destino\n");
    scanf("%d", &dest);
    src--;
    dest--;
    dist[src] = 0;

    for (int i = 0; i < graph.numVertices - 1; i++)
    {
        int u = minDistance(dist, sptSet, graph.numVertices);
        sptSet[u] = true;

        for (int j = 0; j < graph.numVertices; j++)
            if (!sptSet[j] && graph.matrixAdj[u][j] && dist[u] != 100000 && dist[u] + graph.matrixAdj[u][j] < dist[j])
                dist[j] = dist[u] + graph.matrixAdj[u][j];
    }
    printf("O caminho mais curto de %d ate %d tem custo: %d\n", src + 1, dest + 1, dist[dest]);
}

int minDistance(int dist[], bool sptSet[], int size)
{
    // Initialize min value
    int min = 100000, min_index;

    for (int i = 0; i < size; i++)
        if (sptSet[i] == false && dist[i] <= min)
        {
            min = dist[i];
            min_index = i;
        }

    return min_index;
}

void dfs(GRAFO graph)
{ 
    int d[graph.numVertices];   // instante de discoberta
    int f[graph.numVertices];   // instante de finalização
    int cor[graph.numVertices]; // 0 = BRANCO 1 = CINZA 2 = PRETO
    int father[graph.numVertices];
   
    for (int i = 0; i < graph.numVertices; i++)
    {
        cor[i] = 0;
        father[i] = -1;
    }
    int time = 0;

    for (int i = 0; i < graph.numVertices; i++)
    {
        if (cor[i] == 0)
        {
            time = dfsVisit(graph, i, cor, d, f, father, time);
        }
    }

    for (int i = 0; i < graph.numVertices; i++)
    {
        printf("Vertice %d %d/%d\n", i + 1, d[i], f[i]);
    }
}

int dfsVisit(GRAFO graph, int vert, int cor[], int desc[], int fin[], int father[], int time)
{
    // printf("Visitando vertice %d...\n", vert+1);
    cor[vert] = 1;
    time++;
    desc[vert] = time;

    No *listAdj = graph.listAdj[vert].first;

    for (int i = 0; i < graph.listAdj[vert].size; i++)
    {

        if (cor[listAdj->N - 1] == 0)
        {
            father[listAdj->N - 1] = vert;

            time = dfsVisit(graph, listAdj->N - 1, cor, desc, fin, father, time);
        }
        listAdj = listAdj->next;
    }

    cor[vert] = 2;
    time++;
    fin[vert] = time;

    return time;
}

void bfs(GRAFO graph)
{
    int src, dest;
    printf("Escolha o vertice inicial\n");
    scanf("%d", &src);
    printf("Escolha o vertice Destino\n");
    scanf("%d", &dest);
    src--;
    dest--;
    Queue *q = (Queue *)malloc((graph.numVertices) * sizeof(Queue));
    q->front = -1;
    q->rear = -1;

    int d[graph.numVertices];   // distancia
    int cor[graph.numVertices]; // 0 = BRANCO 1 = CINZA 2 = PRETO
    int father[graph.numVertices];

    for (int i = 0; i < graph.numVertices; i++)
    {
        cor[i] = 0;
        d[i] = 100000;
        father[i] = -1;
    }
    cor[src] = 1;
    d[src] = 0;
    father[src] = -1;

    enqueue(q, src, graph);

    while (q->rear != -1)
    {

        int u = dequeue(q);
        No *list_adj = graph.listAdj[u].first;
        for (int i = 0; i < graph.listAdj[u].size; i++)
        {
            if (cor[list_adj->N - 1] == 0)
            {
                cor[list_adj->N - 1] = 1;
                d[list_adj->N - 1] = d[u] + 1;
                father[list_adj->N - 1] = u;
                enqueue(q, list_adj->N - 1, graph);
            }
            list_adj = list_adj->next;
        }

        cor[u] = 2;
    }

    PrintPath(graph, src, dest, father);
}

void PrintPath(GRAFO graph, int s, int v, int father[])
{
    if (v == s)
    {
        printf("%d->", s + 1);
    }
    else
    {
        if (father[v] == -1)
        {
            printf("Nao existe caminho de s a v");
        }
        else
        {
            PrintPath(graph, s, father[v], father);
            printf("%d ", v + 1);
        }
    }
    printf("\n");
}

void enqueue(Queue *q, int value, GRAFO graph)
{
    if (q->rear == graph.numVertices - 1)
        printf("\nQueue is Full!!");
    else
    {
        if (q->front == -1)
            q->front = 0;
        q->rear++;
        q->items[q->rear] = value;
    }
}

int dequeue(Queue *q)
{
    int item;
    if (q->rear == -1)
    {
        printf("Fila Vazia");
        item = -1;
    }
    else
    {
        item = q->items[q->front];
        q->front++;
        if (q->front > q->rear)
        {
            q->front = q->rear = -1;
        }
    }
    return item;
}

void clearScreen()
{
#ifdef __linux__
    system("clear");
#elif _WIN32
    system("cls");
#else

#endif
}