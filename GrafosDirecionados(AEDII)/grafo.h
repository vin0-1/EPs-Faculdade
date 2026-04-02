#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct s
{
    int adj;
    struct s* prox;
} no;

typedef struct
{
    no* inicio;
} vertice;

void Inicializar(int v, vertice* g);
void InsereAresta(vertice* g, int v1, int n);
bool RemoveAresta(vertice* g, int v1, int n);
bool ArestaExiste(vertice* g, int v1, int n);
void PrintGrafo(vertice* g, int n);
void ContadorLacos(vertice* g, int v);
bool RemoveLacos(vertice* g, int v);
void DestruirGrafo(vertice* g, int v);
void GrafoTransposto(vertice* g, int v);


