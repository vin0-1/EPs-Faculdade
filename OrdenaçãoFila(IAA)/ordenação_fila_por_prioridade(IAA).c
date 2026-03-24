#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define MAX_TAMANHO 50

typedef struct Paciente{
    char cor[30];
    char preferencia[30];
    int prioridade;
    bool ehPreferencial;
    int tempEspera;
} Paciente;

void escreverCSV(char *filename, Paciente *pacientes, int quantidadePacientes){
    FILE *arquivo = fopen(filename, "w");
    if (arquivo == NULL) {
        perror("Erro ao criar o arquivo...\n");
        exit(1);
    }
    for(int i = 0; i < quantidadePacientes; i++){
        fprintf(arquivo, "%s,%s,%d\n", pacientes[i].cor, pacientes[i].preferencia, pacientes[i].tempEspera);
    }
    fclose(arquivo);
}

Paciente* openCSV(char *filename, int *quantidadePacientes){
    Paciente *pacientes = malloc(sizeof(Paciente));
    char cor[30];
    char preferencia[30];
    FILE *arquivo = fopen(filename, "r");
    
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo...\n");
        exit(1);
    }

    while(fscanf(arquivo, "%[^,], %[^,], %d\n", cor, preferencia, &pacientes[*quantidadePacientes].tempEspera) == 3){

        pacientes = realloc(pacientes, ((*quantidadePacientes + 2) * sizeof(Paciente)));
        
        if(strcmp(cor, "Vermelho") == 0){
            pacientes[*quantidadePacientes].prioridade = 4;
        } else if (strcmp(cor, "Laranja") == 0){
            pacientes[*quantidadePacientes].prioridade = 3;
        } else if (strcmp(cor, "Amarelo") == 0){
            pacientes[*quantidadePacientes].prioridade = 2;
        } else if(strcmp(cor, "Verde") == 0){
            pacientes[*quantidadePacientes].prioridade = 1;
        } else if (strcmp(cor, "Azul") == 0){
            pacientes[*quantidadePacientes].prioridade = 0;
        }
        
        if(strcmp(preferencia, "N/A") != 0){
            pacientes[*quantidadePacientes].ehPreferencial = true;
        } else {
            pacientes[*quantidadePacientes].ehPreferencial = false;
        }

        strcpy(pacientes[*quantidadePacientes].cor, cor);
        strcpy(pacientes[*quantidadePacientes].preferencia, preferencia);

        (*quantidadePacientes)++;

    }
        fclose(arquivo);
        return pacientes;
}
    
void regraEscalonamento(Paciente *pacientes, int quantidadePacientes){
    for(int i = 0; i < quantidadePacientes; i++){
         if(strcmp(pacientes[i].cor, "Laranja") == 0 && pacientes[i].tempEspera > 10){
            strcpy(pacientes[i].cor, "Vermelho");
            pacientes[i].prioridade = 4;
        } else if (strcmp(pacientes[i].cor, "Amarelo") == 0 && pacientes[i].tempEspera > 60){
            strcpy(pacientes[i].cor, "Laranja");
            pacientes[i].prioridade = 3;
        } else if(strcmp(pacientes[i].cor, "Verde") == 0 && pacientes[i].tempEspera > 120){
            strcpy(pacientes[i].cor, "Amarelo");
            pacientes[i].prioridade = 2;
        } else if (strcmp(pacientes[i].cor, "Azul") == 0 && pacientes[i].tempEspera > 240){
            strcpy(pacientes[i].cor, "Verde");
            pacientes[i].prioridade = 1;
        }
    }
}

int ehPreferencial(const char *preferencia) {
    if (strcmp(preferencia, "N/A") == 0) return 0;
    return 1; 
}

int comparePacientes(Paciente *a, Paciente *b) {
    
    if (a->prioridade != b->prioridade) {
        return b->prioridade - a->prioridade; 
    
    }
    if (a->prioridade >= 3) { 
        if (a->tempEspera != b->tempEspera) {
            return b->tempEspera - a->tempEspera;  
        }
        return 0; 
    }
    
    
    int aPreferencial = ehPreferencial(a->preferencia);
    int bPreferencial = ehPreferencial(b->preferencia);
    
    if (aPreferencial != bPreferencial) {
        return bPreferencial - aPreferencial;  
    }
    
   
    if (a->tempEspera != b->tempEspera) {
        return b->tempEspera - a->tempEspera;  
    }
    
    return 0; 
}

void merge(Paciente *pacientes, int esquerda, int meio, int direita){
    int i, j, k;
    int n1 = meio - esquerda + 1; 
    int n2 = direita - meio; 

    Paciente *L = malloc(n1 * sizeof(Paciente));
    Paciente *R = malloc(n2 * sizeof(Paciente));

    for(i = 0; i < n1; i++)
        L[i] = pacientes[esquerda + i];
    for(j = 0; j < n2; j++)
        R[j] = pacientes[meio + 1 + j];

    i = 0; // indice para L.
    j = 0; // indice para R.   
    k = esquerda;

    
    while(i < n1 && j < n2){
        if(comparePacientes(&L[i], &R[j]) > 0){ 
            pacientes[k] = R[j];
            j++;
        } else {
            pacientes[k] = L[i];
            i++;
        }
        k++;
    }

    while(i < n1){ 
        pacientes[k] = L[i];
        i++;
        k++;
    }

    while(j < n2){
        pacientes[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

void mergesort(Paciente *pacientes, int esquerda, int direita){
    if(esquerda < direita){
        int meio = esquerda + (direita - esquerda) / 2;

        mergesort(pacientes, esquerda, meio);
        mergesort(pacientes, meio + 1, direita);

        merge(pacientes, esquerda, meio, direita);
    }
}

int main(int argc, char *argv[]){
    int quantidadePacientes = 0;

    Paciente *pacientes = openCSV(argv[1], &quantidadePacientes);
    
    regraEscalonamento(pacientes, quantidadePacientes);
    
    mergesort(pacientes, 0, quantidadePacientes - 1);

    escreverCSV("OrdemDeAtendimentos.csv", pacientes, quantidadePacientes);
    
    free(pacientes);
    return 0;
}