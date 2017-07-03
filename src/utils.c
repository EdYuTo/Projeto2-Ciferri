#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include "myregex.h"
#include "../include/utils.h"

char *readLine(FILE *fp, char delim) {

    char c;
    char *str = NULL;
    int counter = 0;

    do {
        c = fgetc(fp);
        str = (char *) realloc (str, sizeof(char)*(counter+1));
        str[counter++] = c;
    } while (c != EOF && c != '\n' && c != delim);

    str[counter-1] = '\0';

    return str;
}

/* copia a string para o array */
void strtoarr(char *string, char array[], int size) {

    int i;
    int n;

    /* n = menor tamanho da string */
    n = strlen(string) > size ? size : strlen(string)+1;

    for (i = 0; i < n-1; i++) {
        array[i] = string[i];
    }
    array[i] = '\0';
}

void tira_acento(char *string) {

    int i;

    for (i = 0; i < strlen(string); i++) {
        if ((string[i] >= -62 && string[i] <= -58) || string[i] == -64)
            string[i] = 'A';
        else if (string[i] == -57)
            string[i] = 'C';
        else if (string[i] >= -56 && string[i] <= -53)
            string[i] = 'E';
        else if (string[i] >= -52 && string[i] <= -49)
            string[i] = 'I';
        else if (string[i] >= -46 && string[i] <= -42)
            string[i] = 'O';
        else if (string[i] >= -39 && string[i] <= -36)
            string[i] = 'U';
        else if (string[i] >= -32 && string[i] <= -26)
            string[i] = 'a';
        else if (string[i] == -25)
            string[i] = 'c';
        else if (string[i] >= -24 && string[i] <= -21)
            string[i] = 'e';
        else if (string[i] >= -20 && string[i] <= -17)
            string[i] = 'i';
        else if (string[i] >= -14 && string[i] <= -10)
            string[i] = 'o';
        else if (string[i] >= -7 && string[i] <= -4)
            string[i] = 'u';
        else if (string[i] == -70)
            string[i] = 'o';
    }
}

void tira_acento_terminal(char *string){
    if(string != NULL){
        int i;
        for(i = 0; i < strlen(string); i++){
            //Retira o flag de acento
            if(string[i] == -61){
                int j;
                int k = i;
                //Desloca toda a string para a esquerda
                for(j = i+1; j < strlen(string)+1; j++){
                    string[k] = string[j];
                    k++;
                }
                string = (char*) realloc(string, sizeof(char) * strlen(string));
            }

            //Trata os casos de '°', 'º', 'ª'
            if(string[i] == -62){
                int j;
                int k = i+1;
                if(string[i+1] == -86) //tratando o 'ª'
                    string[i] = 'a';
                else //tratando o '°' e 'º'
                    string[i] = 'o';

                //Desloca toda a string para a esquerda, pois removeu o flag -62
                for(j = i+2; j < strlen(string)+1; j++){
                    string[k] = string[j];
                    k++;
                }

                string = (char*) realloc(string, sizeof(char) * strlen(string));

            }

            //Trata os acentos do 'a'
            if(string[i] <= -93 && string[i] >= -96)
                string[i] = 'a';

            //Trata os acentos do 'e'
            if(string[i] <= -86 && string[i] >= -88)
                string[i] = 'e';

            //Trata os acentos do 'i'
            if(string[i] <= -82 && string[i] >= -84)
                string[i] = 'i';

            //Trata os acentos do 'o'
            if(string[i] <= -75 && string[i] >= -78)
                string[i] = 'o';

            //Trata os acentos do 'u'
            if(string[i] <= -69 && string[i] >= -71)
                string[i] = 'u';

            //Trata o 'ç'
            if(string[i] == -89)
                string[i] = 'c';

            //Trata os acentos do 'A'
            if(string[i] <= -125 && string[i] >= -128)
                string[i] = 'A';

            //Trata os acentos do 'E'
            if(string[i] <= -118 && string[i] >= -120)
                string[i] = 'E';

            //Trata os acentos do 'I'
            if(string[i] <= -114 && string[i] >= -116)
                string[i] = 'I';

            //Trata os acentos do 'O'
            if(string[i] <= -107 && string[i] >= -110)
                string[i] = 'O';

            //Trata os acentos do 'U'
            if(string[i] <= -101 && string[i] >= -103)
                string[i] = 'U';

            //Trata o 'Ç'
            if(string[i] == -121)
                string[i] = 'C';
        }
        string = (char *) realloc (string, sizeof(char)*(strlen(string)+1));
    }
}

char *stringTok(char *string, char delim, int posI, int *posFim){
    if(string != NULL){
        int i = posI;
        int counter = 0;
        char *ret = NULL;

        while(i < strlen(string)){
            ret = (char*) realloc(ret, sizeof(char) * (counter+1));

            if(string[i] == delim)
                break;
            ret[counter] = string[i];
            counter++;
            i++;
        }
        ret[counter] = '\0';
        *posFim = i+1; //posFim indica o proximo caracter apos o delimitador

        return ret;

    }
    return NULL;
}

void strToupper(char *string){
    int i;

	for(i = 0; i < strlen(string); i++){
        string[i] = toupper(string[i]);
    }

}

void strTolower(char *string){
    int i;

	for(i = 0; i < strlen(string); i++){
        string[i] = tolower(string[i]);
    }

}

REG *criar_registro(){
    REG *reg = (REG*) malloc(sizeof(REG));

    if(reg != NULL){
        int i;
        for(i = 0; i < 20; i++) {
            reg->doc[0] = '\0';
            reg->dataHoraCadastro[0] = '\0';
            reg->dataHoraAtualiza[0] = '\0';
        }

        reg->ticket = 0;
        reg->dominio = NULL;
        reg->nome = NULL;
        reg->cidade = NULL;
        reg->uf = NULL;
    }
    return reg;
}

void apagar_registro(REG **reg){
    if(reg != NULL && *reg != NULL){
        free((*reg)->dominio);
        free((*reg)->nome);
        free((*reg)->cidade);
        free((*reg)->uf);

        free(*reg);
        *reg = NULL;
    }
}

void imprimir_registro(REG *reg){
    if(reg != NULL) {
        printf("-----------------------------\n");
        printf("Dominio: %s\n", reg->dominio);
        printf("Documento: %s\n", reg->doc);
        printf("Nome: %s\n", reg->nome);
        printf("Cidade: %s\n", reg->cidade);
        printf("UF: %s\n", reg->uf);
        printf("Data/hora do cadastro: %s\n", reg->dataHoraCadastro);
        printf("Data/hora de atualizacao: %s\n", reg->dataHoraAtualiza);
        printf("Ticket: %d\n", reg->ticket);
        printf("-----------------------------\n");
    }
}

void imprimir_vetor_registro(REG *reg, int size){

    int i;

    for (i = 0; i < size; i++) {
        imprimir_registro(&reg[i]);
        if (i != size-1) {
            printf("Digite ENTER para continuar a impressão");
            char *aux = readLine(stdin, '\n'); //espera o ENTER
            free(aux);
        }
    }
}

/* pega a string do arquivo.bin e  */
char *makeIndex_filename(char *string) {

    char **s = match(string, "^(.*).bin$", 2);
    char *idx;

    idx = (char *) malloc (sizeof(char)*(strlen(s[1])+5));
    strcpy(idx, s[1]);

    strcat(idx,".idx");

    free(s[0]);
    free(s[1]);
    free(s);

    return idx;
}

INDEX *criar_index(){
    INDEX *index = (INDEX*) malloc(sizeof(INDEX));

    if(index != NULL){
        index->ticket = 0;
        index->byteOffset = 0;
    }

    return index;
}

void apagar_index(INDEX **index){
    if(index != NULL && (*index) != NULL){
        free(*index);
        *index = NULL;
    }
}

int binary_search(INDEX **vector, int key, int start, int end) {

    int middle = (start+end)/2;

    if (start > end) return -1;

    if (key == vector[middle]->ticket)
        return middle;

    else if (key < vector[middle]->ticket)
        return binary_search(vector, key, start, middle-1);

    else
        return binary_search(vector, key, middle+1, end);
}

REG *le_registro() {

    REG *r = criar_registro();

    printf("Digite o domínio: ");
    r->dominio = readLine(stdin, '\n');

    printf("Digite o documento: ");
    char *doc = readLine(stdin, '\n');
    strtoarr(doc, r->doc, 20);

    printf("Digite o nome: ");
    r->nome = readLine(stdin, '\n');
    tira_acento_terminal(r->nome);

    printf("Digite a cidade: ");
    r->cidade = readLine(stdin, '\n');
    tira_acento_terminal(r->cidade);

    printf("Digite a UF: ");
    r->uf = readLine(stdin, '\n');
    tira_acento_terminal(r->uf);

    printf("Digite a DataHoraCadastro: ");
    char *dataHoraCadastro = readLine(stdin, '\n');
    strtoarr(dataHoraCadastro, r->dataHoraCadastro, 20);

    printf("Digite a DataHoraAtualiza: ");
    char *dataHoraAtualiza = readLine(stdin, '\n');
    strtoarr(dataHoraAtualiza, r->dataHoraAtualiza, 20);

    printf("Digite o ticket: ");
    scanf("%d", &(r->ticket));

    return r;
}