#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include "grafo.h"

void InicializarMatriz(int v, int m[][v]);
void InsereArestaMatriz(int v, int m[][v], int v1, int n);
void PrintMatriz(int v, int m[][v]);

int main () {
    int v1;
    int n;
    int v; 
    printf("Digite o numero de vertices: "); 
    scanf("%d", &v);

    int m[v][v];
    vertice *g = (vertice*)malloc((size_t)v * sizeof(vertice));
    
    Inicializar(v, g);
    InicializarMatriz(v, m);
    printf("\nSeu grafo foi inicializado\n");
    printf("\n");

    int esc;
    do{
        printf("\nOpcões: \n");
        printf("Adicionar Aresta lista ligada[1]\n");
        printf("Remover Aresta lista ligada[2]\n");
        printf("Printar grafo lista ligada[3]\n");
        printf("Quantos laços tem?[4]\n");
        printf("Acidiona aresta Matriz[5]\n");
        printf("Remove Matriz[6]\n");
        printf("Print Matriz[7]\n");
        printf("Remover todos os laços[8]\n");
        printf("Grafo transposto[9]\n");
        printf("Sair do programa [0]\n");
        printf("\n");

        printf("Digite uma das opções: ");
        scanf("%d", &esc);
        printf("\n");

        switch (esc) {
            case 1:
                printf("Indique o vertice: ");
                scanf("%d", &v1);
                printf("\nIndique para onde ele ira apontar: ");
                scanf("%d", &n);

                if(v1 >= 0 && v1 < v && n >= 0 && n < v) {
                    InsereAresta(g, v1, n);
                    printf("\nAresta adicionada com sucesso!\n");
                } else {
                    printf("\nVértice inválido! Use valores entre 0 e %d\n", v-1);
                }
                break;

            case 2:
                printf("Indique o vertice: ");
                scanf("%d", &v1);
                printf("\nIndique a adjacencia que será removida: ");
                scanf("%d", &n);

                if(v1 >= 0 && v1 < v && n >= 0 && n < v) {
                    if(RemoveAresta(g, v1, n)) {  
                        printf("\nAresta removida com sucesso!\n");
                    } else {
                        printf("\nAresta não encontrada!\n");
                    }
                } else {
                    printf("\nVértice inválido! Use valores entre 0 e %d\n", v-1);
                }
                break;

            case 3:
                printf("\n=== LISTA DE ADJACÊNCIA ===\n");
                PrintGrafo(g, v);

                break;
                
            case 4:  
                ContadorLacos(g, v);
                break;

            case 5:  
                printf("Indique o vertice: ");
                scanf("%d", &v1);
                printf("\nIndique para onde ele ira apontar: ");
                scanf("%d", &n);

                InsereArestaMatriz(v, m, v1, n);
                break;

            case 6:  
                printf("Indique o vertice: ");
                scanf("%d", &v1);
                printf("\nIndique a adjacencia que será removida: ");
                scanf("%d", &n);
                
                m[v1][n] = 0;

                break;

            case 7:  
                printf("\n=== LISTA DE MATRIZ ===\n");
                PrintMatriz(v, m);

                break;

            case 8:
                if(RemoveLacos(g, v)){
                    printf("\nLaços removidos!\n");
                }else{
                    printf("erro ao remover laços.......");
                }

                break;

            case 9:
                GrafoTransposto(g, v);
                break;

            case 0:
                printf("\nSaindo do programa...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
                break;
        }

    } while(esc != 0);

    for(int i = 0; i < v; i++) {
        no* atual = g[i].inicio;
        while(atual != NULL) {
            no* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    
    free(g);
    return 0;
}

void Inicializar (int v, vertice* g) {
    
    for(int i = 0; i < v; i++){
        g[i].inicio = NULL;
    }

    if(g == NULL){
        printf("erro ao inicializar");
    }
    else {
        printf("inicializado com sucesso!\n");
    }

}

void InicializarMatriz(int v,int m[v][v]){
    int i,j;
    for(i = 0; i < v; i++){
        for(j = 0; j < v; j++){
            m[i][j] = 0;
        }
    }
    printf("Matriz inicializada com sucesso!\n");
}

void InsereAresta(vertice* g, int v1, int n){
    
    if(ArestaExiste(g, v1, n)){
        printf("\nAresta já existe!\n");
        return;  
    } 

    no* novo = (no*)malloc(sizeof(no));

    novo->adj = n;
    novo->prox = g[v1].inicio;
    g[v1].inicio = novo;

}

void InsereArestaMatriz(int v, int m[v][v], int v1, int n){
    if(m[v1][n] == 1){
        printf("A aresta já existe na matriz!\n");
        return;
    } 

    m[v1][n] = 1;
    printf("Aresta adicionada na matriz!\n");
}

bool RemoveAresta(vertice* g, int v1, int n){
    no* p = g[v1].inicio;
    no* ant = NULL;

    while(p){
        if(p->adj == n) break;

        ant = p;
        p = p->prox;
    }

    if(!p) return false;

    if(ant) ant->prox = p->prox;    
    else g[v1].inicio = p->prox;

    free(p);
    return true;
}

bool ArestaExiste(vertice* g, int v1, int n){
    no* p = g[v1].inicio;

    while(p){
        if(p->adj == n) return true;

        p = p->prox;
    }

    return false;
}

void PrintGrafo(vertice* g, int v){

    for(int i = 0;i < v; i++){
        
        no* p = g[i].inicio;
        printf("%d: ", i);

        if(p){
            printf("%d", p->adj);
            p = p->prox;
            while(p){
                printf(", %d", p->adj);
                p = p->prox;
            }
        }
        printf("\n");
    } 
}

void PrintMatriz(int v, int m[v][v]){
    printf("\nMatriz de Adjacência %dx%d:\n", v, v);
    
    // Linha superior
    printf("    ");
    for (int j = 0; j < v; j++) {
        printf(" %2d ", j);
    }
    printf("\n");
    
    printf("   ");
    for (int j = 0; j <= v; j++) {
        printf("----");
    }
    printf("\n");
    
    // Linhas da matriz
    for (int i = 0; i < v; i++) {
        printf("%2d |", i);
        for (int j = 0; j < v; j++) {
            printf(" %2d ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void ContadorLacos(vertice* g, int v){
    int cont = 0;
    for(int i = 0; i < v; i++){
        no* p = g[i].inicio;

        while(p){
            if(i == p->adj){
                cont++;
            }
            p = p->prox;
        }
   }

    if(cont == 0){
        printf("\nNão tem laços\n");
    }else{
        printf("\nNúmero de laços: %d\n", cont);
    }
}

bool RemoveLacos(vertice* g, int v){
    int cont = 0;
    for(int i = 0; i < v; i++){
        no* p = g[i].inicio;
        no* ant = NULL;

        while(p){
            if(p->adj == i) break;

            ant = p;
            p = p->prox;
        }

        if(!p) continue;

        if(ant) ant->prox = p->prox;
        else g[i].inicio = p->prox;

        cont++;
        free(p);
        
    }

    if(cont != 0) return true;
    else return false;


}

void DestruirGrafo(vertice* g, int v){

    for(int i = 0; i < v; i++){
        no* p = g[i].inicio;

        while(p){
            no* aux = p;
            p = p->prox;

            free(aux);
        }

        g[i].inicio = NULL;

    }

    printf("\ndestruido...\n");
}

void GrafoTransposto(vertice* g, int v){
    vertice* trans = (vertice*)malloc((size_t)v * sizeof(vertice));
    Inicializar(v, trans);

    for(int i = 0; i < v; i++){
        no* p = g[i].inicio;

        while(p){
            InsereAresta(trans, p->adj, i);
            p = p->prox;
        }
    }
    printf("\n=== GRAFO TRANSPOSTO ===\n");
    PrintGrafo(trans, v);
}