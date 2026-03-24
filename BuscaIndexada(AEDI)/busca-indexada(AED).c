#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "arvore.h"

#define TAMANHO 10000
#define BLOCO_MEMORIA 10

long long comparacoes = 0;
long long comparacoes_carga = 0;



typedef struct EntradaIndice{
    char *palavra;
    int *linhas;          
    int num_linhas;       
    int capacidade;        
}EntradaIndice;

typedef struct {
    EntradaIndice *entradas;  
    int tamanho;              
    int capacidade;           
}IndiceLista;



IndiceLista* criar_indice() {
    IndiceLista *indice = malloc(sizeof(IndiceLista));
    indice->entradas = malloc(100 * sizeof(EntradaIndice));
    indice->tamanho = 0;
    indice->capacidade = 100;
    return indice;
}


void expandir_indice(IndiceLista *indice) {
    indice->capacidade *= 2;
    indice->entradas = realloc(indice->entradas, 
                              indice->capacidade * sizeof(EntradaIndice));
}

int buscar_posicao(IndiceLista *indice, const char *palavra, int *encontrou) {
    int inicio = 0;
    int fim = indice->tamanho - 1;
    *encontrou = 0;
    
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(indice->entradas[meio].palavra, palavra);
        comparacoes++;

        if (cmp == 0) {
            *encontrou = 1;
            return meio;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return inicio;  
}


Boolean inserir_palavra_indice(IndiceLista *indice, const char *palavra, int linha_num) {
    int encontrou;
    int pos = buscar_posicao(indice, palavra, &encontrou);
    
    if (encontrou) {
       
        EntradaIndice *ent = &indice->entradas[pos];
        
        
        for (int i = 0; i < ent->num_linhas; i++) {
            if (ent->linhas[i] == linha_num) {
                return FALSE; 
            }
        }
        
        
        if (ent->num_linhas >= ent->capacidade) {
            ent->capacidade *= 2;
            ent->linhas = realloc(ent->linhas, ent->capacidade * sizeof(int));
        }
        
        // Insere linha ordenada
        int i = ent->num_linhas - 1;
        while (i >= 0 && ent->linhas[i] > linha_num) {
            ent->linhas[i + 1] = ent->linhas[i];
            i--;
        }
        ent->linhas[i + 1] = linha_num;
        ent->num_linhas++;
        return TRUE;
    } else {
        
        if (indice->tamanho >= indice->capacidade) {
            expandir_indice(indice);
        }
        
        
        for (int i = indice->tamanho; i > pos; i--) {
            indice->entradas[i] = indice->entradas[i - 1];
        }
        
        
        EntradaIndice *nova = &indice->entradas[pos];
        nova->palavra = strdup(palavra);
        nova->linhas = malloc(10 * sizeof(int));
        nova->linhas[0] = linha_num;
        nova->num_linhas = 1;
        nova->capacidade = 10;
        
        indice->tamanho++;
        return TRUE;
    }
}


EntradaIndice* buscar_palavra_indice(IndiceLista *indice, const char *palavra, long long *comparacoes) {
    int encontrou;
    *comparacoes = 0;
    int inicio = 0;
    int fim = indice->tamanho - 1;
    
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(indice->entradas[meio].palavra, palavra);
        
        if (cmp == 0) {
            return &indice->entradas[meio];
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return NULL;
}


void liberar_indice(IndiceLista *indice) {
    if (indice == NULL) return;
    
    for (int i = 0; i < indice->tamanho; i++) {
        free(indice->entradas[i].palavra);
        free(indice->entradas[i].linhas);
    }
    free(indice->entradas);
    free(indice);
}


Arvore *cria_arvore()
{
    Arvore *arvore = (Arvore *)malloc(sizeof(Arvore));
    arvore->raiz = NULL;
    arvore->palavras_unicas = 0;
    return arvore;
}

int calcula_altura(No *no)
{
    if (no == NULL)
    {
        return -1;
    }
    int esq = calcula_altura(no->esq);
    int dir = calcula_altura(no->dir);
    return (esq > dir ? esq : dir) + 1;
}

void adiciona_ocorrencia(No *no, int linha_atual)
{
    Ocorrencia *atual = no->ocorrencias;
    Ocorrencia *anterior = NULL;

    while (atual != NULL && atual->linha_num <= linha_atual)
    {
        if (atual->linha_num == linha_atual)
        {
            atual->contador++;
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }

    Ocorrencia *nova_ocorrencia = (Ocorrencia *)malloc(sizeof(Ocorrencia));
    nova_ocorrencia->linha_num = linha_atual;
    nova_ocorrencia->contador = 1;
    nova_ocorrencia->prox = atual;

    if (anterior == NULL)
    {
        no->ocorrencias = nova_ocorrencia;
    }
    else
    {
        anterior->prox = nova_ocorrencia;
    }
}

No *busca_bin_rec(No *no, char *palavra)
{
    if (no)
    {
        comparacoes++;
        int cmp = strcmp(palavra, no->palavra);

        if (cmp == 0)
        {
            return no;
        }
        if (cmp < 0)
            return busca_bin_rec(no->esq, palavra);

        return busca_bin_rec(no->dir, palavra);
    }

    return NULL;
}

No *busca_bin(Arvore *arvore, char *palavra)
{
    comparacoes = 0;
    return busca_bin_rec(arvore->raiz, palavra);
}

Boolean insere_ord_rec(No *raiz, No *novo)
{
    comparacoes++;
    int cmp = strcmp(novo->palavra, raiz->palavra);

    if (cmp < 0)
    {
        if (raiz->esq)
            return insere_ord_rec(raiz->esq, novo);
        else
            raiz->esq = novo;
    }
    else
    {
        if (raiz->dir)
            return insere_ord_rec(raiz->dir, novo);
        else
            raiz->dir = novo;
    }

    return TRUE;
}

Boolean insere_ord(Arvore *arvore, char *palavra, int linha_atual)
{
    No *no_existente = busca_bin(arvore, palavra);
    comparacoes = 0;

    if (no_existente)
    {
        adiciona_ocorrencia(no_existente, linha_atual);
        return FALSE;
    }

    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL)
        return FALSE;

    novo->palavra = strdup(palavra);
    novo->ocorrencias = NULL;
    novo->esq = novo->dir = NULL;

    adiciona_ocorrencia(novo, linha_atual);
    arvore->palavras_unicas++;

    if (arvore->raiz)
        return insere_ord_rec(arvore->raiz, novo);

    arvore->raiz = novo;
    return TRUE;
}

void libera_ocorrencias(Ocorrencia *oc)
{
    Ocorrencia *temp;
    while (oc != NULL)
    {
        temp = oc;
        oc = oc->prox;
        free(temp);
    }
}

void libera_arvore(No *no)
{
    if (no == NULL)
        return;
    libera_arvore(no->esq);
    libera_arvore(no->dir);
    free(no->palavra);
    libera_ocorrencias(no->ocorrencias);
    free(no);
}


void converte_minusculas(char *s)
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        s[i] = tolower((unsigned char)s[i]);
    }
}

void limpa_e_separa_pontuacao(char *linha)
{
    if (linha == NULL)
        return;

    for (int i = 0; linha[i] != '\0'; i++)
    {
        if (!isalnum((unsigned char)linha[i]))
        {
            linha[i] = ' ';
        }
    }
}

void remove_caracteres_nao_alfabeticos(char *palavra)
{
    if (palavra == NULL)
        return;

    char *leitura = palavra;
    char *escrita = palavra;

    while (*leitura)
    {
        if (isalnum((unsigned char)*leitura))
        {
            *escrita++ = *leitura;
        }
        leitura++;
    }
    *escrita = '\0';
}


int main(int argc, char **argv)
{
    FILE *in;
    char *linha;
    char *copia_ponteiro_linha;
    char *quebra_de_linha;
    char *palavra;
    int contador_linha;

    char **lista_linhas = NULL;
    int capacidade_lista = 0;
    int num_linhas_lidas = 0;
    IndiceLista *indice_lista = NULL;  
    Arvore *abb = NULL;

    char modo_operacao[10] = "";
    long long comparacoes_carga = 0;

    if (argc >= 3)
    {
        if (strcmp(argv[2], "lista") == 0)
        {
            strcpy(modo_operacao, "lista");
            indice_lista = criar_indice();  
        }
        else if (strcmp(argv[2], "arvore") == 0)
        {
            strcpy(modo_operacao, "arvore");
            abb = cria_arvore();
        }
        else
        {
            fprintf(stderr, "Tipo de indice invalido. Use 'lista' ou 'arvore'.\n");
            return 1;
        }

        in = fopen(argv[1], "r");

        if (in == NULL)
        {
            fprintf(stderr, "Erro ao abrir o arquivo: %s\n", argv[1]);
            return 1;
        }

        linha = (char *)malloc((TAMANHO + 1) * sizeof(char));
        if (linha == NULL)
        {
            fprintf(stderr, "Erro de alocação de memória.\n");
            fclose(in);
            return 1;
        }

        comparacoes = 0;

        while (fgets(linha, TAMANHO, in))
        {
            if ((quebra_de_linha = strrchr(linha, '\n')))
                *quebra_de_linha = 0;

            if (num_linhas_lidas >= capacidade_lista)
            {
                capacidade_lista += BLOCO_MEMORIA;
                char **novo_array = (char **)realloc(lista_linhas, capacidade_lista * sizeof(char *));
                lista_linhas = novo_array;
            }
            char *linha_copiada = strdup(linha);
            lista_linhas[num_linhas_lidas] = linha_copiada;

            
            if (indice_lista != NULL)
            {
                char *linha_para_indexar = strdup(linha);
                converte_minusculas(linha_para_indexar);
                limpa_e_separa_pontuacao(linha_para_indexar);

                copia_ponteiro_linha = linha_para_indexar;

                while ((palavra = strsep(&copia_ponteiro_linha, " ")))
                {
                    if (palavra[0] != '\0')
                    {
                        
                        inserir_palavra_indice(indice_lista, palavra, num_linhas_lidas + 1);
                    }
                }
                free(linha_para_indexar);
            }

            
            if (abb != NULL)
            {
                char *linha_para_indexar = strdup(linha);
                converte_minusculas(linha_para_indexar);
                limpa_e_separa_pontuacao(linha_para_indexar);

                copia_ponteiro_linha = linha_para_indexar;

                while ((palavra = strsep(&copia_ponteiro_linha, " ")))
                {
                    if (palavra[0] != '\0')
                    {
                        insere_ord(abb, palavra, num_linhas_lidas + 1);
                    }
                }
                free(linha_para_indexar);
            }

            contador_linha++;
            num_linhas_lidas++;
        }

        comparacoes_carga = comparacoes;
        free(linha);
        fclose(in);

        printf("Arquivo: '%s'\n", argv[1]);
        printf("Tipo de indice: '%s'\n", modo_operacao);
        printf("Numero de linhas no arquivo: %d\n", num_linhas_lidas);

        if (strcmp(modo_operacao, "arvore") == 0)
        {
            printf("Total de palavras unicas indexadas: %d\n", abb->palavras_unicas);
            printf("Altura da arvore: %d\n", calcula_altura(abb->raiz));
        }
        else if (strcmp(modo_operacao, "lista") == 0)
        {
            printf("Total de palavras unicas indexadas: %d\n", indice_lista->tamanho);
        }

        printf("Numero de comparacoes realizadas para a construcao do indice: %lld\n", comparacoes_carga);

        char linha_entrada[TAMANHO];
        char comando_acao[TAMANHO];
        char palavra_busca[TAMANHO];

        while (1)
        {
            printf("> ");

            
            if (fgets(linha_entrada, TAMANHO, stdin) == NULL)
                break;

            if ((quebra_de_linha = strrchr(linha_entrada, '\n')))
                *quebra_de_linha = 0;

            if (sscanf(linha_entrada, "%s %s", comando_acao, palavra_busca) == 2 && strcmp(comando_acao, "busca") == 0)
            { 
                converte_minusculas(palavra_busca);
                remove_caracteres_nao_alfabeticos(palavra_busca);

                if (palavra_busca[0] == '\0')
                {
                    printf("Palavra de busca invalida ou vazia.\n");
                    printf("Numero de comparacoes: 0\n");
                    continue;
                }

                if (strcmp(modo_operacao, "lista") == 0)
                {
                    long long comparacoes_busca = 0;
                    EntradaIndice *resultado = buscar_palavra_indice(indice_lista, palavra_busca, &comparacoes_busca);
                    
                    if (resultado != NULL)
                    {
                        printf("Existem %d ocorrencias da palavra '%s' na(s) seguinte(s) linha(s):\n",
                               resultado->num_linhas, palavra_busca);
                        
                        for (int i = 0; i < resultado->num_linhas; i++)
                        {
                            int linha_num = resultado->linhas[i];
                            printf("%05d: %s\n",
                                   linha_num,
                                   lista_linhas[linha_num - 1]);
                        }
                    }
                    else
                    {
                        printf("Palavra '%s' nao encontrada.\n", palavra_busca);
                    }

                    printf("Numero de comparacoes: %lld\n", comparacoes_busca);
                    continue;
                }

                if (abb != NULL)
                {
                    No *resultado = busca_bin(abb, palavra_busca);

                    if (resultado)
                    {
                        Ocorrencia *oc = resultado->ocorrencias;
                        int total_ocorrencias = 0;

                        while (oc != NULL)
                        {
                            total_ocorrencias += oc->contador;
                            oc = oc->prox;
                        }

                        printf("Existem %d ocorrencias da palavra '%s' na(s) seguinte(s) linha(s):\n",
                               total_ocorrencias, palavra_busca);

                        oc = resultado->ocorrencias;
                        while (oc != NULL)
                        {
                            printf("%05d: %s\n",
                                   oc->linha_num,
                                   lista_linhas[oc->linha_num - 1]);
                            oc = oc->prox;
                        }
                    }
                    else
                    {
                        printf("Palavra '%s' nao encontrada.\n", palavra_busca);
                    }
                }
            }
            else if (strcmp(linha_entrada, "fim") == 0 || strcmp(comando_acao, "fim") == 0)
            {
                break;
            }
            else if (linha_entrada[0] != '\0')
            {
                printf("Opcao invalida!\n");
            }
        }

        
        for (int i = 0; i < num_linhas_lidas; i++)
        {
            free(lista_linhas[i]);
        }
        free(lista_linhas);

        if (abb != NULL)
        {
            libera_arvore(abb->raiz);
            free(abb);
        }
        
        if (indice_lista != NULL)
        {
            liberar_indice(indice_lista);
        }

        return 0;
    }
    else
    {
        printf("quantidade de argumentos incorreta! Uso: <programa> <arquivo> lista | arvore\n");
    }

    return 1;
}