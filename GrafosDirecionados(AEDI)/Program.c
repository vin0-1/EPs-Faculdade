#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include "grafo.h"

int main () {
    
    int v; 
    printf("Digite o numero de vertices: "); 
    scanf("%d", &v);

    vertice *g = (vertice*)malloc((size_t)v * sizeof(vertice));
    

    Inicilizar(v, g);
    printf("\nSeu grafo foi inicializado\n");
    printf("\n");

    int esc;
    do{
        printf("\nOpcões: \n");
        printf("Adicionar Aresta [1]\n");
        printf("Remover Aresta [2]\n");
        printf("Ver se existe Aresta[3]\n");
        printf("Printar as vértices e seus caminhos[4]\n");
        printf("Sair do programa [0]\n");
        printf("\n");

        printf("Digite uma das opções: ");
        scanf("%d", &esc);
        printf("\n");

        switch (esc) {
            case 1:

            int v1;
            int n;

            printf("Indique o vertice: ");
            scanf("%d", &v1);
            printf("\nIndique para onde ele ira apontar: ");
            scanf("%d", &n);

            //poderia ter uma verificação aq
            InsereAresta(g, v1, n);
            break;

            case 4:
            
            PrintGrafo(g, v);

            break;

        }

    }while(esc != 0);

    
    free(g);
    return 0;
}

void Inicilizar (int v, vertice* g) {
    
    for(int i = 0; i < v; i++){
        g[i].inicio = NULL;
    }

    if(g == NULL){
        printf("erro ao inicializar");
    }
    else {
        printf("inicializado com sucesso!");
    }

}

void InsereAresta(vertice* g, int v1, int n){
    
    if(ArestaExiste(g, v1, n)){
        printf("\nAresta já exite!\n");
        return;  
    } 

    no* novo = (no*)malloc(sizeof(no));

    novo->adj = n;
    novo->prox = g[v1].inicio;
    g[v1].inicio = novo;

    if(ArestaExiste(g, v1, n)){
        printf("\naresta adicionada!\n");
    }else{
        printf("\nerro ao adicionar aresta!\n");
    }
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
        printf("\n%d: ", i);

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

