#ifndef PROJETO1_CIFERRI_UTILS_H
#define PROJETO1_CIFERRI_UTILS_H
#include <stdio.h>

/* struct do registro com todos
os campos, na ordem de leitura */
typedef struct {
    char *dominio;
    char doc[20];
    char *nome;
    char *cidade;
    char *uf;
    char dataHoraCadastro[20];
    char dataHoraAtualiza[20];
    int ticket;
} REG;

/**
 * Struct do registro dos arquivos de indice
 */
typedef struct {
    int ticket;
    int byteOffset;
} INDEX;

/**
 * Cria um registro com os campos vazios
 * @return Ponteiro para o registro criado
 */
REG *criar_registro();

/**
 * Apaga da memória um registro
 * @param reg - Endereço do ponteiro do registro
 */
void apagar_registro(REG **reg);

/**
 * Imprime um registro na tela
 * @param reg - Ponteiro para o registro a ser impresso
 */
void imprimir_registro(REG *reg);
void imprimir_vetor_registro(REG *reg, int size);

void strtoarr(char *string, char array[], int size);

/**
 * Função que tira os acentos de uma string lida de um arquivo .csv
 * @param string
 */
void tira_acento(char *string);

/**
 * Função que tira os acentos de uma string lida pelo terminal
 * @param string
 */
void tira_acento_terminal(char *string);

/**
 * Função que le uma linha (ou até um delimitador) de um arquivo
 * @param fp - Ponteiro para o arquivo
 * @param delim - delimitador
 * @return  - String lida
 */
char *readLine(FILE *fp, char delim);

/**
 * Função que separa uma string a partir de um delimitador
 * @param string - String a ser separada
 * @param delim - Delimitador
 * @param posI - Posição inicial para realizar a separação
 * @param posFim - Endereço do inteiro que recebera a posição após o delimitador
 * @return - String separada
 */
char *stringTok(char *string, char delim, int posI, int *posFim);

/**
 * Função que deixa uma string em caixa alta
 * @param string - String a ser modificada
 */
void strToupper(char *string);
/**
 * Função que deixa uma string em caixa baixa
 * @param string - String a ser modificada
 */
void strTolower(char *string);

/**
* Função que gera uma string de arquivo .idx
* @param string - nome do arquivo .bin
*/
char *makeIndex(char *string);

/**
 * Funçao que cria um indice
 */
INDEX *criar_index();

void apagar_index(INDEX **index);

int binary_search(INDEX **vector, int key, int start, int end);

#endif //PROJETO1_CIFERRI_UTILS_H
