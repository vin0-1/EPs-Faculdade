// arvore.h

#ifndef ARVORE_H
#define ARVORE_H

#include <stdlib.h> // Para malloc
#include <string.h> // Para strcmp

// --- DEFINIÇÕES DE TIPOS E CONSTANTES ---
#define FALSE 0
#define TRUE 1

typedef int Boolean;

// --- ESTRUTURAS DE DADOS ---

// Estrutura para armazenar a linha e o número de vezes que a palavra apareceu nela
typedef struct _ocorrencia_
{
    int linha_num;
    int contador;
    struct _ocorrencia_ *prox;
} Ocorrencia;

typedef struct _no_arvore_
{
    char *palavra;           // A string real da palavra (AGORA É A CHAVE PRIMÁRIA)
    Ocorrencia *ocorrencias; // Lista encadeada de ocorrências
    struct _no_arvore_ *esq;
    struct _no_arvore_ *dir;

} No;

typedef struct
{
    No *raiz;
    int palavras_unicas; // Total de palavras unicas indexadas
} Arvore;

// --- PROTÓTIPOS DE FUNÇÕES DA ABB ---

Arvore *cria_arvore();
No *busca_bin(Arvore *arvore, char *palavra);
Boolean insere_ord(Arvore *arvore, char *palavra, int linha_atual);

void adiciona_ocorrencia(No *no, int linha_atual);
int calcula_altura(No *no);
void libera_arvore(No *no);
void libera_ocorrencias(Ocorrencia *oc);

#endif // ARVORE_H
