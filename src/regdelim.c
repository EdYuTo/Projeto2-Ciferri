#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>
#include <limits.h>

#define FIRST_FIT 0
#define BEST_FIT 1
#define WORST_FIT 2

/* realiza a leitura do arquivo csv e escrita no
arquivo de saida */
int read_csv_delim() {

    FILE *fpin = fopen("turmaA-dadosDominios.csv", "r+");
    FILE *fpout1 = fopen("best.bin", "w+");
    FILE *fpout2 = fopen("worst.bin", "w+");
    FILE *fpout3 = fopen("first.bin", "w+");
    char *dominio;
    char *documento;
    char doc[20];
    char *nome;
    char *cidade;
    char *UF;
    char *cadastro;
    char *atualiza;
    char DHC[20];
    char DHA[20];
    int ticket, tam;
    int tamfixo = 20;
    char delim = '#';
    int filesize;
    int head = -1;

    if (!fpin) {
        if (fpout1)
            fclose(fpout1);
        if (fpout2)
            fclose(fpout2);
        if (fpout3)
            fclose(fpout3);
        return -1;
    }

    fseek(fpin, 0, SEEK_END);
    filesize = ftell(fpin);
    fseek(fpin, 0, SEEK_SET);

    fwrite(&head, sizeof(int), 1, fpout1);
    fwrite(&head, sizeof(int), 1, fpout2);
    fwrite(&head, sizeof(int), 1, fpout3);

    while (ftell(fpin) != filesize) {

        /* leitura de cada um dos campos do resgistro */
        dominio = readLine(fpin, ';');
        documento = readLine(fpin,';');
        nome = readLine(fpin, ';');
        UF = readLine(fpin, ';');
        cidade = readLine(fpin, ';');
        cadastro = readLine(fpin, ';');
        atualiza = readLine(fpin, ';');
        fscanf(fpin, "%d", &ticket);
        fscanf(fpin, "\n");

        /* escreve documento no arquivo ou vazio se
        "null" no csv */
        tira_acento(documento);
        if (!strcmp(documento, "null")) doc[0] = '\0';
        else strtoarr(documento,doc, tamfixo);
        fwrite(doc, sizeof(char), tamfixo, fpout1);
        fwrite(doc, sizeof(char), tamfixo, fpout2);
        fwrite(doc, sizeof(char), tamfixo, fpout3);

        /* escrita de dataHoraCadastro e tratamento
        caso "null" */
        tira_acento(cadastro);
        if (!strcmp(cadastro, "null"))  DHC[0] = '\0';
        else strtoarr(cadastro, DHC, tamfixo);
        fwrite(DHC, sizeof(char), tamfixo, fpout1);
        fwrite(DHC, sizeof(char), tamfixo, fpout2);
        fwrite(DHC, sizeof(char), tamfixo, fpout3);

        /* escrita de dataHoraAtualiza */
        tira_acento(atualiza);
        if (!strcmp(atualiza, "null"))  DHA[0] = '\0';
        else strtoarr(atualiza, DHA, tamfixo);
        fwrite(DHA, sizeof(char), tamfixo, fpout1);
        fwrite(DHA, sizeof(char), tamfixo, fpout2);
        fwrite(DHA, sizeof(char), tamfixo, fpout3);

        /* escrita do valor do ticket */
        fwrite(&ticket, sizeof(int), 1, fpout1);
        fwrite(&ticket, sizeof(int), 1, fpout2);
        fwrite(&ticket, sizeof(int), 1, fpout3);

        /* escrita do tamanho de dominio e
        seu conteudo */
        if (!strcmp(dominio, "null"))   dominio[0] = '\0';
        tira_acento(dominio);
        tam = strlen(dominio)+1;
        fwrite(&tam, sizeof(int), 1, fpout1);
        fwrite(&tam, sizeof(int), 1, fpout2);
        fwrite(&tam, sizeof(int), 1, fpout3);
        fwrite(dominio, sizeof(char), tam, fpout1);
        fwrite(dominio, sizeof(char), tam, fpout2);
        fwrite(dominio, sizeof(char), tam, fpout3);

        /* escrita do tamanho do nome e seu conteudo */
        if (!strcmp(nome, "null"))  nome[0] = '\0';
        tira_acento(nome);
        tam = strlen(nome)+1;
        fwrite(&tam, sizeof(int), 1, fpout1);
        fwrite(&tam, sizeof(int), 1, fpout2);
        fwrite(&tam, sizeof(int), 1, fpout3);
        fwrite(nome, sizeof(char), tam, fpout1);
        fwrite(nome, sizeof(char), tam, fpout2);
        fwrite(nome, sizeof(char), tam, fpout3);

        /* tamanho e conteudo de cidade */
        if (!strcmp(cidade, "null"))    cidade[0] = '\0';
        tira_acento(cidade);
        tam = strlen(cidade)+1;
        fwrite(&tam, sizeof(int), 1, fpout1);
        fwrite(&tam, sizeof(int), 1, fpout2);
        fwrite(&tam, sizeof(int), 1, fpout3);

        fwrite(cidade, sizeof(char), tam, fpout1);
        fwrite(cidade, sizeof(char), tam, fpout2);
        fwrite(cidade, sizeof(char), tam, fpout3);

        /* escrita de UF */
        if (!strcmp(UF, "null"))    UF[0] = '\0';
        tira_acento(UF);
        tam = strlen(UF)+1;
        fwrite(&tam, sizeof(int), 1, fpout1);
        fwrite(&tam, sizeof(int), 1, fpout2);
        fwrite(&tam, sizeof(int), 1, fpout3);

        fwrite(UF, sizeof(char), tam, fpout1);
        fwrite(UF, sizeof(char), tam, fpout2);
        fwrite(UF, sizeof(char), tam, fpout3);

        /* escreve o delimitador (!) */
        fwrite(&delim, sizeof(char), 1, fpout1);
        fwrite(&delim, sizeof(char), 1, fpout1);
        
        fwrite(&delim, sizeof(char), 1, fpout2);
        fwrite(&delim, sizeof(char), 1, fpout2);
        
        fwrite(&delim, sizeof(char), 1, fpout3);
        fwrite(&delim, sizeof(char), 1, fpout3);

        /* liberando strings lidas
        apos passadas ao arquivo */
        free(dominio);
        free(documento);
        free(nome);
        free(cidade);
        free(UF);
        free(cadastro);
        free(atualiza);
    }

    fclose (fpin);
    fclose (fpout1);
    fclose (fpout2);
    fclose (fpout3);

    return 1;
}

REG *read_register(FILE *fp){
    REG *r;
    int tam, ticket;
    char aux[500];
    int tamfixo = 20;
    char delim;

    int n_delim = 0;

    r = criar_registro();

    while(n_delim < 2) {
        
        /* lendo o documento e salvando em r.documento */
        fread(&aux, sizeof(char), tamfixo, fp);
        memcpy(r->doc, aux, tamfixo);

        /* lendo dataHoraCadastro e salvando na struct r */
        fread(&aux, sizeof(char), tamfixo, fp);
        memcpy(r->dataHoraCadastro, aux, tamfixo);

        /* lendo dataHoraAtualiza e salvando na struct r */
        fread(&aux, sizeof(char), tamfixo, fp);
        memcpy(r->dataHoraAtualiza, aux, tamfixo);

        /* lendo o numero do ticket */
        fread(&ticket, sizeof(int), 1, fp);
        r->ticket = ticket;

        /* lendo e salvando o dominio */
        fread(&tam, sizeof(int), 1, fp);
        fread(&aux, sizeof(char), tam, fp);
        r->dominio = (char *) malloc(sizeof(char) * tam);
        strcpy(r->dominio, aux);

        /* lendo e salvando o nome */
        fread(&tam, sizeof(int), 1, fp);
        fread(&aux, sizeof(char), tam, fp);
        r->nome = (char *) malloc(sizeof(char) * tam);
        strcpy(r->nome, aux);

        /* lendo e salvando a cidade */
        fread(&tam, sizeof(int), 1, fp);
        fread(&aux, sizeof(char), tam, fp);
        r->cidade = (char *) malloc(sizeof(char) * tam);
        strcpy(r->cidade, aux);

        /* lendo e salvando a uf */
        fread(&tam, sizeof(int), 1, fp);
        fread(&aux, sizeof(char), tam, fp);
        r->uf = (char *) malloc(sizeof(char) * tam);
        strcpy(r->uf, aux);

        /* leitura do delimitador */
        fread(&delim, sizeof(char), 1, fp);
        n_delim++;
        fread(&delim, sizeof(char), 1, fp);
        n_delim++;
    }
    return r;

}

void read_out_delim(char *name) {

    FILE *fp = fopen(name, "r+");
    REG *r;
    int ticket;
    int filesize;

    if (fp == NULL) {
        printf("\nErro. Nao foi possivel abrir o arquivo.\n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    /* ignorando o cabecalho */
    fread(&ticket, sizeof(int), 1, fp);
    
    printf("Digite ENTER para comecar a impressão ou ctrl+D para sair");
    while (ftell(fp) < filesize && fgetc(stdin) != EOF) {
        r = read_register(fp);
        imprimir_registro(r);
        apagar_registro(&r);

        printf("Digite ENTER para continuar a impressão ou ctrl+D para sair");
    }

    fclose(fp);
}

/* retorna o tamanho do registro
que inicia em 'offset' */
int record_size(FILE *fp, int offset) {

    char c;
    int n_delim = 0;
    int filesize, k = 0;

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, offset, SEEK_SET);

    while (n_delim < 2 && ftell(fp) < filesize) {
        fread(&c, sizeof(char), 1, fp);

        if(c == '#')
            n_delim ++;
        else if (n_delim > 0)
            n_delim = 0;
        k++;
    }

    fseek(fp, -k, SEEK_CUR);
    k -= 2;

    return k;
}

void remove_from_index(INDEX ***vector, int *size, int k) {

    int i;
    apagar_index(&((*vector)[k]));
    for (i = k; i < (*size)-1; i++) {
        (*vector)[i] = (*vector)[i+1];
    }
    *vector = (INDEX **) realloc (*vector, sizeof(INDEX *)*((*size)-1));
    (*size)--;
    return;
}

int remove_record_no_sort(int ticket, char *file_bin, INDEX ***vector, int *size) {

    FILE *fp_bin = fopen(file_bin, "r+");
    int result;
    int rec_size;
    int head;
    const char removed = '*';

    if (!fp_bin) {
        printf("Erro ao abrir o arquivo de dados.\n");
        return 0;
    }

    fread(&head, sizeof(int), 1, fp_bin);

    /* busca binaria no vetor de indices */
    result = binary_search(*vector, ticket, 0, (*size)-1);

    if (result == -1) {
        printf("Ticket não encontrado.\n");
        fclose(fp_bin);
        return 0;
    }
    
    printf("Resultado-> Ticket: %d\t Offset: %d\n", (*vector)[result]->ticket, (*vector)[result]->byteOffset);

    /* tamanho do registro a ser removido */
    rec_size = record_size(fp_bin, (*vector)[result]->byteOffset);

    fseek(fp_bin, (*vector)[result]->byteOffset, SEEK_SET);

    /* marca como removido, marca o offset do
    ultimo registro removido e o tamanho do registro
    recem removido */
    fwrite(&removed, sizeof(char), 1, fp_bin);
    fwrite(&head, sizeof(int), 1, fp_bin);
    fwrite(&rec_size, sizeof(int), 1, fp_bin);


    /* escreve o offset no cabeçalho */
    fseek(fp_bin, 0, SEEK_SET);
    fwrite(&(*vector)[result]->byteOffset, sizeof(int), 1, fp_bin);

    /* remove do vetor de indices */
    remove_from_index(vector, size, result);

    fclose(fp_bin);

    return 1;
}

int remove_record_ascending_sort(int ticket, char *file_bin, INDEX ***vector, int *size) {

    FILE *fp_bin = fopen(file_bin, "r+");
    int result;
    int rec_size;
    int head;
    const char removed = '*';

    if (!fp_bin) {
        printf("Erro ao abrir o arquivo de dados.\n");
        return 0;
    }

    fread(&head, sizeof(int), 1, fp_bin);

    /* busca binaria no vetor de indices */
    result = binary_search(*vector, ticket, 0, (*size)-1);

    if (result == -1) {
        printf("Ticket não encontrado.\n");
        fclose(fp_bin);
        return 0;
    }

    printf("Resultado-> Ticket: %d\t Offset: %d\n", (*vector)[result]->ticket, (*vector)[result]->byteOffset);
    
    /* tamanho do registro a ser removido */
    rec_size = record_size(fp_bin, (*vector)[result]->byteOffset);

    int regSize = -1, i = 1;
    int offset = head;
    char c;
    int ant = -1;
    if(head != -1){
        fseek(fp_bin, head, SEEK_SET);
        fread(&c, sizeof(char), 1, fp_bin);
        fread(&offset, sizeof(int), 1, fp_bin);
        fread(&regSize, sizeof(int), 1, fp_bin);
        fseek(fp_bin, head, SEEK_SET);
        offset = head;
    }

    /*Procura a posiçao de inserçao na lista*/
    while(head != -1 && rec_size > regSize){
        ant = ftell(fp_bin);
        fread(&c, sizeof(char), 1, fp_bin);
        fread(&offset, sizeof(int), 1, fp_bin);
        fread(&regSize, sizeof(int), 1, fp_bin);

        if(offset != -1)
            fseek(fp_bin, offset, SEEK_SET);
        else break;
    }

    fseek(fp_bin, (*vector)[result]->byteOffset, SEEK_SET);

    /* marca como removido, marca o offset do
    ultimo registro removido e o tamanho do registro
    recem removido */
    fwrite(&removed, sizeof(char), 1, fp_bin);
    fwrite(&offset, sizeof(int), 1, fp_bin);
    fwrite(&rec_size, sizeof(int), 1, fp_bin);

    /*Insere o registro na lista de registros removidos*/
    if(ant != -1) {
        fseek(fp_bin, ant, SEEK_SET);
        fread(&c, sizeof(char), 1, fp_bin);
        fwrite(&(*vector)[result]->byteOffset, sizeof(int), 1, fp_bin);
    }
    /* escreve o offset no cabeçalho se necessario*/
    else{
        fseek(fp_bin, 0, SEEK_SET);
        fwrite(&(*vector)[result]->byteOffset, sizeof(int), 1, fp_bin);
    }

    /* remove do vetor de indices */
    remove_from_index(vector, size, result);

    fclose(fp_bin);

    return 1;
}

int remove_record_descending_sort(int ticket, char *file_bin, INDEX ***vector, int *size) {

   FILE *fp_bin = fopen(file_bin, "r+");
   int result;
   int rec_size;
   int head;
   const char removed = '*';

   if (!fp_bin) {
      printf("Erro ao abrir o arquivo de dados.\n");
      return 0;
   }

   fread(&head, sizeof(int), 1, fp_bin);

   /* busca binaria no vetor de indices */
   result = binary_search(*vector, ticket, 0, (*size)-1);

   if (result == -1) {
      printf("Ticket não encontrado.\n");
      fclose(fp_bin);
      return 0;
   }

   printf("Resultado-> Ticket: %d\t Offset: %d\n", (*vector)[result]->ticket, (*vector)[result]->byteOffset);

   /* tamanho do registro a ser removido */
   rec_size = record_size(fp_bin, (*vector)[result]->byteOffset);

   int regSize = -1;
   int offset = head;
   char c;
   int ant = -1;
   if(head != -1){
      fseek(fp_bin, head, SEEK_SET);
      fread(&c, sizeof(char), 1, fp_bin);
      fread(&offset, sizeof(int), 1, fp_bin);
      fread(&regSize, sizeof(int), 1, fp_bin);
      fseek(fp_bin, head, SEEK_SET);
      offset = head;
   }

   /*Procura a posiçao de inserçao na lista*/
   while(head != -1 && rec_size < regSize){
      ant = ftell(fp_bin);
      fread(&c, sizeof(char), 1, fp_bin);
      fread(&offset, sizeof(int), 1, fp_bin);
      fread(&regSize, sizeof(int), 1, fp_bin);

      if(offset != -1)
         fseek(fp_bin, offset, SEEK_SET);
      else break;
   }

   fseek(fp_bin, (*vector)[result]->byteOffset, SEEK_SET);

   /* marca como removido, marca o offset do
      ultimo registro removido e o tamanho do registro
      recem removido */
   fwrite(&removed, sizeof(char), 1, fp_bin);
   fwrite(&offset, sizeof(int), 1, fp_bin);
   fwrite(&rec_size, sizeof(int), 1, fp_bin);

   /*Insere o registro na lista de registros removidos*/
   if(ant != -1) {
      fseek(fp_bin, ant, SEEK_SET);
      fread(&c, sizeof(char), 1, fp_bin);
      fwrite(&(*vector)[result]->byteOffset, sizeof(int), 1, fp_bin);
   }
   /* escreve o offset no cabeçalho se necessario*/
   else{
      fseek(fp_bin, 0, SEEK_SET);
      fwrite(&(*vector)[result]->byteOffset, sizeof(int), 1, fp_bin);
   }

   /* remove do vetor de indices */
   remove_from_index(vector, size, result);

   fclose(fp_bin);

   return 1;
}

void show_list(char *filename){
    FILE *fp = fopen(filename, "r");
    int offset;

    if (!fp) {
        printf("Arquivo invalido!\n");
        return;
    }
    
    fread(&offset, sizeof(int), 1, fp);
    printf("Cabeça da lista (byte offset): %d\n", offset);
    printf("-------------------------------------\n");

    if (offset != -1)
        fseek(fp, offset, SEEK_SET);

    printf("Digite ENTER para continuar a impressão ou ctrl+D para sair\n");
    while (offset != -1 && fgetc(stdin) != EOF) {
        char c;
        int regSize;

        fread(&c, sizeof(char), 1, fp);
        fread(&offset, sizeof(int), 1, fp);
        fread(&regSize, sizeof(int), 1, fp);

        fseek(fp, offset, SEEK_SET);

        printf("Tamanho do registro: %d\nProximo da lista (byte offset): %d\n", regSize, offset);
        printf("-------------------------------------\n");
        printf("Digite ENTER para continuar a impressão ou ctrl+D para sair\n");
    }
    fclose(fp);
}
