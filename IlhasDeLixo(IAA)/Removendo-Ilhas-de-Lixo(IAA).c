#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int altura;
    bool visitado;
} celula;

typedef struct {
    bool existente;
    bool pertence;
} item;

int buscaAdjacente(celula **matriz, int i, int j, int linhas, int colunas) {
    if(i < 0 || i >= linhas || j < 0 || j >= colunas) {
        return 0;
    }
    if(matriz[i][j].visitado || matriz[i][j].altura <= 0) {
        return 0;
    }

    matriz[i][j].visitado = true;

    return matriz[i][j].altura + buscaAdjacente(matriz, i+1, j, linhas, colunas) +
           buscaAdjacente(matriz, i-1, j, linhas, colunas) +
           buscaAdjacente(matriz, i, j+1, linhas, colunas) +
           buscaAdjacente(matriz, i, j-1, linhas, colunas);
}

void mochila(int capacidade, int* ilhas, int n) {
    
    item **matriz = (item **)malloc((n + 1) * sizeof(item *));
    for (int i = 0; i <= n; i++) {
        matriz[i] = (item *)malloc((capacidade + 1) * sizeof(item));
    }
    for (int i = 0; i <= n; i++) {
        matriz[i][0].existente = true;
        matriz[i][0].pertence = false;  
    }
    for (int j = 1; j <= capacidade; j++) {
        matriz[0][j].existente = false;
        matriz[0][j].pertence = false;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= capacidade; j++) {
            matriz[i][j].existente = false;
            matriz[i][j].pertence = false;

            if(matriz[i-1][j].existente) {
                matriz[i][j].existente = true;
                matriz[i][j].pertence = false;
            }else if (ilhas[i-1] <= j && matriz[i-1][j - ilhas[i-1]].existente) {
                matriz[i][j].existente = true;
                matriz[i][j].pertence = true;
            }
        }
    }


    if (!matriz[n][capacidade].existente) {
        printf("Nao ha resposta valida!\n");
    } else {
        int capacidade_restante = capacidade;
            for (int i = n; i > 0; i--) {
            if (matriz[i][capacidade_restante].pertence) {
                printf("%d ", ilhas[i-1]);
                capacidade_restante -= ilhas[i-1];
            }
        }
        printf("\n");
    }
    for (int i = 0; i <= n; i++) {
        free(matriz[i]);
    }
    free(matriz);
}


celula** recebe_txt(const char *arquivo, int linhas, int colunas) {  

    FILE *file; 
    
    file = fopen(arquivo, "r"); // Abre o arquivo em modo de leitura
    
    if(file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    celula ** matriz = (celula **) malloc(linhas * sizeof(celula *));

    for (int i = 0; i < linhas; i++) {
        matriz[i] = (celula *) malloc(colunas * sizeof(celula));
    }

    if (matriz == NULL) {
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (fscanf(file, "%d", &matriz[i][j].altura) != 1) {
                printf("Erro ao ler elemento na posição %d, %d.\n", i, j);
                fclose(file);
                return NULL;
            }
            matriz[i][j].visitado = false;
        }
    }

    return matriz;
}

int main(int argc, char *argv[]) {
    int profundidade, linhas, colunas;
    int quantIlhas = 0;
    char nomeArquivo[255];
    celula **matriz;  
    int *alturaIlhas;  
   
     if(argc < 5) {
        printf("Uso: %s <profundidade> <linhas> <colunas> <arquivo>\n", argv[0]);
        return 1;
    }

    profundidade = atoi(argv[1]);
    linhas = atoi(argv[2]);
    colunas = atoi(argv[3]);
    strcpy(nomeArquivo, argv[4]);

    matriz = recebe_txt(nomeArquivo, linhas, colunas); 
    if(matriz == NULL) {
        return 0;
    }

    alturaIlhas = (int *)malloc(linhas * colunas * sizeof(int));  
    if(alturaIlhas == NULL) {
        printf("Erro de alocacao de memoria\n");
        for(int i = 0; i < linhas; i++) {
            free(matriz[i]);
        }
        free(matriz);
        return 1;
    }

     for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (matriz[i][j].altura > 0 && !matriz[i][j].visitado) {
                quantIlhas++;
                alturaIlhas[quantIlhas - 1] = (buscaAdjacente(matriz, i, j, linhas, colunas))/6;
                if(alturaIlhas[quantIlhas - 1] == 0) {
                    alturaIlhas[quantIlhas - 1] = 1;
                }
            }
        }
    }

    printf("%d\n", quantIlhas);

    for(int i = 0; i < quantIlhas; i++) {
        printf("%d ", alturaIlhas[i]);
    }
    printf("\n");

    mochila(profundidade, alturaIlhas, quantIlhas);

    for(int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
    free(alturaIlhas);
    
    return 0;
}