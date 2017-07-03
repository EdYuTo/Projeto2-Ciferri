#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>
#include <limits.h>
#include <heap.h>

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

    int pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp);
    fseek(fp, pos, SEEK_SET);

   fread(&delim, sizeof(char), 1, fp);

   /*se o registro for marcado como removido ele eh pulado*/
   while ((delim == '*' || delim == '!') && ftell(fp) < filesize) {
      while (n_delim < 2) {
         fread(&delim, sizeof(char), 1, fp);

         if(delim == '#')
            n_delim ++;
         else if (n_delim > 0)
            n_delim = 0;
      }
      fread(&delim, sizeof(char), 1, fp);
       n_delim = 0;
   }

   r = criar_registro();
   fseek(fp, -1, SEEK_CUR);

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
    
    printf("Digite ENTER para comecar a impressão ou ctrl+D para sair\n");
    while (ftell(fp) < filesize && fgetc(stdin) != EOF) {
        r = read_register(fp);
        imprimir_registro(r);
        apagar_registro(&r);

        printf("Digite ENTER para continuar a impressão ou ctrl+D para sair\n");
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
        int regSize, offset_atual = offset;

        fread(&c, sizeof(char), 1, fp);
        fread(&offset, sizeof(int), 1, fp);
        fread(&regSize, sizeof(int), 1, fp);

        fseek(fp, offset, SEEK_SET);

        printf("Byte Offset: %d\nTamanho do registro: %d\nProximo da lista (byte offset): %d\n",offset_atual, regSize, offset);
        printf("-------------------------------------\n");
        printf("Digite ENTER para continuar a impressão ou ctrl+D para sair\n");
    }
    fclose(fp);
}

void writeReg(FILE *fp, REG *reg) {
   char delim = '#';
   int tam, tamfixo = 20;
   char doc[20], DHC[20], DHA[20];

   /* escreve documento no arquivo ou vazio se
      "null" no csv */
   if (!strcmp(reg->doc, "null")) doc[0] = '\0';
   else strtoarr(reg->doc, doc, tamfixo);
   fwrite(doc, sizeof(char), tamfixo, fp);

   /* escrita de dataHoraCadastro e tratamento
      caso "null" */
   if (!strcmp(reg->dataHoraCadastro, "null"))  DHC[0] = '\0';
   else strtoarr(reg->dataHoraCadastro, DHC, tamfixo);
   fwrite(DHC, sizeof(char), tamfixo, fp);

   /* escrita de dataHoraAtualiza */
   if (!strcmp(reg->dataHoraAtualiza, "null"))  DHA[0] = '\0';
   else strtoarr(reg->dataHoraAtualiza, DHA, tamfixo);
   fwrite(DHA, sizeof(char), tamfixo, fp);

   /* escrita do valor do ticket */
   fwrite(&reg->ticket, sizeof(int), 1, fp);

   /* escrita do tamanho de dominio e
      seu conteudo */
   if (!strcmp(reg->dominio, "null"))   reg->dominio[0] = '\0';
   tam = strlen(reg->dominio)+1;
   fwrite(&tam, sizeof(int), 1, fp);
   fwrite(reg->dominio, sizeof(char), tam, fp);

   /* escrita do tamanho do nome e seu conteudo */
   if (!strcmp(reg->nome, "null"))  reg->nome[0] = '\0';
   tam = strlen(reg->nome)+1;
   fwrite(&tam, sizeof(int), 1, fp);
   fwrite(reg->nome, sizeof(char), tam, fp);

   /* tamanho e conteudo de cidade */
   if (!strcmp(reg->cidade, "null"))    reg->cidade[0] = '\0';
   tam = strlen(reg->cidade)+1;
   fwrite(&tam, sizeof(int), 1, fp);
   fwrite(reg->cidade, sizeof(char), tam, fp);

   /* escrita de UF */
   if (!strcmp(reg->uf, "null"))    reg->uf[0] = '\0';
   tam = strlen(reg->uf)+1;
   fwrite(&tam, sizeof(int), 1, fp);
   fwrite(reg->uf, sizeof(char), tam, fp);

   /* escreve o delimitador (!) */
   fwrite(&delim, sizeof(char), 1, fp);
   fwrite(&delim, sizeof(char), 1, fp);
}

int reg_Size(REG *reg) {
   if (reg) {
      int tam = 0;
      tam += strlen(reg->dominio)+1;//|
      tam += strlen(reg->nome)+1;   //|
      tam += strlen(reg->cidade)+1; //|
      tam += strlen(reg->uf)+1;     //|_-> + caracter '\0'
      tam += sizeof(int);//ticket
       tam += 20 * sizeof(char); //tam fixo
       tam += 20 * sizeof(char); //tam fixo
       tam += 20 * sizeof(char); //tam fixo
      return tam;
   }
   return -1;//erro
}

void add_to_index(INDEX ***vector, int *size, int ticket, int byteOffset) {
   *vector = (INDEX **) realloc (*vector, sizeof(INDEX *)*((*size)+1));
   (*vector)[(*size)] = criar_index();
   (*vector)[(*size)]->ticket = ticket;
   (*vector)[(*size)++]->byteOffset = byteOffset;
   heap_sort(*vector, *size);
   return;
}

int insert_worstFit(char *file_bin, INDEX ***index, int *indSize, REG *newreg) {
   FILE *fp = fopen(file_bin, "r+");
   if (fp && newreg != NULL) {//se forem validos
      int offset;
      fread(&offset, sizeof(int), 1, fp);//ler cabecalho
      /*se o cabecario for negativo, insere-se no final*/
      if (offset < 0) {
         fseek(fp, 0, SEEK_END);//vai ate o final
         add_to_index(index, indSize, newreg->ticket, ftell(fp));//adiciona um indice
         writeReg(fp, newreg);//escreve no final
      } else {
         fseek(fp, offset, SEEK_SET);
         char c;
         int regSize, fit = 0;

         /*por definicao, se ha um * existe ao menos 9 bytes, entao existe byteoffset e tam*/
         fread(&c, sizeof(char), 1, fp);
         if (c == '*')
            fit = 1;//pode ser inserido nessa posicao
         fread(&offset, sizeof(int), 1, fp);
         fread(&regSize, sizeof(int), 1, fp);

         /*se couber insere-se nessa posicao*/
         if (fit && regSize > reg_Size(newreg)+18) {//indicadores de tamanho!
            int pos = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            fwrite(&offset, sizeof(int), 1, fp);
            fseek(fp, pos, SEEK_SET);

            fseek(fp, -9, SEEK_CUR);//volta os 9 bytes lidos acima
            add_to_index(index, indSize, newreg->ticket, ftell(fp));//adiciona o indice
            writeReg(fp, newreg);//escreve na posicao
            int newOffset = ftell(fp);//pos atual
            char useless = '!', mark = '*';//chars predeterminados
            int rest = regSize - reg_Size(newreg)-18;//tamanho restante ate o prox registro
            int ant = -1;
            /*se o espaco restante for suficiente para armazenar char + int + int*/
            if (rest > 8) {
               /*Procura a posiçao de inserçao na lista*/
               while (offset != -1 && rest < regSize){
                  fseek(fp, offset, SEEK_SET);
                  ant = ftell(fp);
                  fread(&c, sizeof(char), 1, fp);
                  fread(&offset, sizeof(int), 1, fp);
                  fread(&regSize, sizeof(int), 1, fp);
               }

               fseek(fp, newOffset, SEEK_SET);

               /* marca como removido, marca o offset do
                  ultimo registro removido e o tamanho do registro
                  recem removido */
               fwrite(&mark, sizeof(char), 1, fp);
               fwrite(&offset, sizeof(int), 1, fp);
               fwrite(&rest, sizeof(int), 1, fp);

               /*Insere o registro na lista de registros removidos*/
               if(ant != -1) {
                  fseek(fp, ant, SEEK_SET);
                  fread(&c, sizeof(char), 1, fp);
                  fwrite(&newOffset, sizeof(int), 1, fp);
               }
               /* escreve o offset no cabeçalho se necessario*/
               else{
                  fseek(fp, 0, SEEK_SET);
                  fwrite(&newOffset, sizeof(int), 1, fp);
               }

            /*se o espaco nao for suficiente, insere ! para indicar*/
            } else
               fwrite(&useless, sizeof(char), 1, fp);

         /*se nao couber vai para o final*/
         } else {
            fseek(fp, 0, SEEK_END);//vai para o fim
            add_to_index(index, indSize, newreg->ticket, ftell(fp));//adiciona nos indices
            writeReg(fp, newreg);//escreve no fim
         }
      }
      fclose(fp);
      return 1;//sucesso
   }
   return 0;//erro
}

int insert_reg_first_fit(char *filename, REG *reg, INDEX ***index, int* nIndex){
    if(filename != NULL && reg != NULL){
        FILE *fp = fopen(filename, "r+");
        int pos, offset, remSize = 0, regSize, head, auxOff;
        char aux;

        fseek(fp, 0, SEEK_END);
        int fim = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        int search = binary_search(*index, reg->ticket, 0, *nIndex);
        if(search != -1){
            printf("Ticket ja existe no aquivo de dados!\n");
            return 0;
        }

        /*Tamanho do registro*/
        regSize = reg_Size(reg);

        /*Leitura do cabeçalho*/
        fread(&head, sizeof(int), 1, fp);
        offset = head;

        if(offset != -1){
            fseek(fp, head, SEEK_SET);
            fread(&aux, sizeof(char), 1, fp);
            fread(&offset, sizeof(int), 1, fp);
            fread(&remSize, sizeof(int), 1, fp);

            pos = head;
            auxOff = offset;
            offset = head;
            fseek(fp, head, SEEK_SET);
        }
        else{
            pos = fim;
        }

        /*Laço que acha a posiçao de inserçao*/
        while(offset != -1 && regSize+3*sizeof(char) > remSize){
            if(regSize == remSize)
                break;

            pos = offset;

            fread(&aux, sizeof(char), 1, fp);
            fread(&offset, sizeof(int), 1, fp);
            fread(&remSize, sizeof(int), 1, fp);

            if(offset != -1)
                fseek(fp, offset, SEEK_SET);
        }

        if(regSize+3*sizeof(char) > remSize)
            pos = fim;

        fseek(fp, pos, SEEK_SET);
        writeReg(fp, reg);

        int size = remSize - regSize;
        char remChar = '*';
        char delim = '#';

        if(size >= (3*sizeof(char) + 2*sizeof(int)) && size > 0){
            if(offset == head)
                offset = auxOff;

            if(head == pos)
                head = offset;

            int newpos = ftell(fp);
            fwrite(&remChar, sizeof(char), 1, fp);
            fwrite(&head, sizeof(int), 1, fp);
            fwrite(&size, sizeof(int), 1, fp);

            fseek(fp, (size - 3*sizeof(char) - 2*sizeof(int)), SEEK_CUR);
            fwrite(&delim, sizeof(char), 1, fp);
            fwrite(&delim, sizeof(char), 1, fp);

            fseek(fp, 0 ,SEEK_SET);
            fwrite(&newpos, sizeof(int), 1, fp);
        }
        else if(size > 0){

            fwrite(&remChar, sizeof(char), 1,fp);
            fseek(fp, size - 2, SEEK_CUR);
            fwrite(&delim, sizeof(char), 1, fp);
            fwrite(&delim, sizeof(char), 1, fp);
        }

        /*Inserçao no indice*/
        add_to_index(index, nIndex,reg->ticket, pos);

        fclose(fp);

        return 1;
    }
    return 0;
}

/* BEST FIT */
/********************************************/

void rearrange(FILE *fp, int old_offset, int new_offset) {

    int temp = ftell(fp);
    int head;
    int next;

    fseek(fp, 0, SEEK_SET);
    fread(&head, sizeof(int), 1, fp);

    next = head;

    while (next != -1) {
        printf("Next: %d\n", next);
        fseek(fp, next, SEEK_SET);
        fseek(fp, 1, SEEK_CUR);
        fread(&next, sizeof(char), 1, fp);
        if (next == old_offset) {
            fseek(fp, -(sizeof(int)), SEEK_CUR);
            fwrite(&new_offset, sizeof(int), 1, fp);
        }
    }

    fseek(fp, temp, SEEK_SET);
}

int insert_best_fit(char *file_bin, INDEX ***index, int *indSize, REG *newreg) {

   FILE *fp = fopen(file_bin, "r+");
   if (fp && newreg != NULL) {//se forem validos
      int offset;
      fread(&offset, sizeof(int), 1, fp);//ler cabecalho
      /*se o cabecario for negativo, insere-se no final*/
      if (offset < 0) {
         fseek(fp, 0, SEEK_END);//vai ate o final
         add_to_index(index, indSize, newreg->ticket, ftell(fp));//adiciona um indice
         writeReg(fp, newreg);//escreve no final
      } else {
         fseek(fp, offset, SEEK_SET);
         char c;
         int regSize, fit = 0;

         /*por definicao, se ha um * existe ao menos 9 bytes, entao existe byteoffset e tam*/
         fread(&c, sizeof(char), 1, fp);
         if (c == '*')
            fit = 1;//pode ser inserido nessa posicao
         fread(&offset, sizeof(int), 1, fp);
         fread(&regSize, sizeof(int), 1, fp);

         /*se couber insere-se nessa posicao*/
         if (fit && regSize > reg_Size(newreg)+18) {//indicadores de tamanho!
            int pos = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            fwrite(&offset, sizeof(int), 1, fp);
            fseek(fp, pos, SEEK_SET);

            fseek(fp, -9, SEEK_CUR);//volta os 9 bytes lidos acima
            add_to_index(index, indSize, newreg->ticket, ftell(fp));//adiciona o indice
            writeReg(fp, newreg);//escreve na posicao
            int newOffset = ftell(fp);//pos atual
            char useless = '!', mark = '*';//chars predeterminados
            int rest = regSize - reg_Size(newreg)-18;//tamanho restante ate o prox registro
            int ant = -1;
            /*se o espaco restante for suficiente para armazenar char + int + int*/
            if (rest > 8) {
               /*Procura a posiçao de inserçao na lista*/
               while (offset != -1 && rest > regSize){
                  fseek(fp, offset, SEEK_SET);
                  ant = ftell(fp);
                  fread(&c, sizeof(char), 1, fp);
                  fread(&offset, sizeof(int), 1, fp);
                  fread(&regSize, sizeof(int), 1, fp);
               }

               fseek(fp, newOffset, SEEK_SET);

               /* marca como removido, marca o offset do
                  ultimo registro removido e o tamanho do registro
                  recem removido */
               fwrite(&mark, sizeof(char), 1, fp);
               fwrite(&offset, sizeof(int), 1, fp);
               fwrite(&rest, sizeof(int), 1, fp);

               /*Insere o registro na lista de registros removidos*/
               if(ant != -1) {
                  fseek(fp, ant, SEEK_SET);
                  fread(&c, sizeof(char), 1, fp);
                  fwrite(&newOffset, sizeof(int), 1, fp);
               }
               /* escreve o offset no cabeçalho se necessario*/
               else{
                  fseek(fp, 0, SEEK_SET);
                  fwrite(&newOffset, sizeof(int), 1, fp);
               }

            /*se o espaco nao for suficiente, insere ! para indicar*/
            } else
               fwrite(&useless, sizeof(char), 1, fp);

         /*se nao couber vai para o final*/
         } else {
            fseek(fp, 0, SEEK_END);//vai para o fim
            add_to_index(index, indSize, newreg->ticket, ftell(fp));//adiciona nos indices
            writeReg(fp, newreg);//escreve no fim
         }
      }
      fclose(fp);
      return 1;//sucesso
   }
   return 0;//erro
}