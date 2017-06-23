#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>

/* realiza a leitura do arquivo csv e escrita no
arquivo de saida */
int read_csv_delim() {

    FILE *fpin = fopen("turmaA-dadosDominios.csv", "r+");
    FILE *fpout = fopen("regdelim.bin", "w+");
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

    if (!fpin)
        return -1;

    fseek(fpin, 0, SEEK_END);
    filesize = ftell(fpin);
    fseek(fpin, 0, SEEK_SET);

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
        fwrite(doc, sizeof(char), tamfixo, fpout);

        /* escrita de dataHoraCadastro e tratamento
        caso "null" */
        tira_acento(cadastro);
        if (!strcmp(cadastro, "null"))  DHC[0] = '\0';
        else strtoarr(cadastro, DHC, tamfixo);
        fwrite(DHC, sizeof(char), tamfixo, fpout);

        /* escrita de dataHoraAtualiza */
        tira_acento(atualiza);
        if (!strcmp(atualiza, "null"))  DHA[0] = '\0';
        else strtoarr(atualiza, DHA, tamfixo);
        fwrite(DHA, sizeof(char), tamfixo, fpout);

        /* escrita do valor do ticket */
        fwrite(&ticket, sizeof(int), 1, fpout);

        /* escrita do tamanho de dominio e
        seu conteudo */
        if (!strcmp(dominio, "null"))   dominio[0] = '\0';
        tira_acento(dominio);
        tam = strlen(dominio)+1;
        fwrite(&tam, sizeof(int), 1, fpout);
        fwrite(dominio, sizeof(char), tam, fpout);

        /* escrita do tamanho do nome e seu conteudo */
        if (!strcmp(nome, "null"))  nome[0] = '\0';
        tira_acento(nome);
        tam = strlen(nome)+1;
        fwrite(&tam, sizeof(int), 1, fpout);
        fwrite(nome, sizeof(char), tam, fpout);

        /* tamanho e conteudo de cidade */
        if (!strcmp(cidade, "null"))    cidade[0] = '\0';
        tira_acento(cidade);
        tam = strlen(cidade)+1;
        fwrite(&tam, sizeof(int), 1, fpout);
        fwrite(cidade, sizeof(char), tam, fpout);

        /* escrita de UF */
        if (!strcmp(UF, "null"))    UF[0] = '\0';
        tira_acento(UF);
        tam = strlen(UF)+1;
        fwrite(&tam, sizeof(int), 1, fpout);
        fwrite(UF, sizeof(char), tam, fpout);

        /* escreve o delimitador (!) */
        fwrite(&delim, sizeof(char), 1, fpout);

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
    fclose (fpout);

    return 1;
}

void read_out_delim() {

    FILE *fp = fopen("regdelim.bin", "r+");
    REG *r;
    int tam, ticket;
    char *str;
    char aux[500];
    int tamfixo = 20;
    char delim;
    int filesize;

    if (fp == NULL) {
        printf("\nErro. Nao foi possivel abrir o arquivo.\n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    while (ftell(fp) != filesize) {

        r = criar_registro();

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
        r->dominio = (char *) malloc (sizeof(char)*tam);
        strcpy(r->dominio, aux);

        /* lendo e salvando o nome */
        fread(&tam, sizeof(int), 1, fp);
        fread(&aux, sizeof(char), tam, fp);
        r->nome = (char *) malloc (sizeof(char)*tam);
        strcpy(r->nome, aux);

        /* lendo e salvando a cidade */
        fread(&tam, sizeof(int), 1, fp);
        fread(&aux, sizeof(char), tam, fp);
        r->cidade = (char *) malloc (sizeof(char)*tam);
        strcpy(r->cidade, aux);

        /* lendo e salvando a uf */
        fread(&tam, sizeof(int), 1, fp);
        fread(&aux, sizeof(char), tam, fp);
        r->uf = (char *) malloc (sizeof(char)*tam);
        strcpy(r->uf, aux);

        /* leitura do delimitador */
        fread(&delim, sizeof(char), 1, fp);

        imprimir_registro(r);
        apagar_registro(&r);
        printf("Digite ENTER para continuar a impressão");
        char *aux = readLine(stdin, '\n'); //espera o ENTER
        free(aux);
    }

    fclose(fp);
}

void busca_documento_delim(char *chave) {

    FILE *fp = fopen("regdelim.bin", "r+");
    char *try;
    char c = 0;
    int tam;
    int tamfixo = 20;
    int filesize;
    char delim;
    int cont = 0;

    REG *r;

    if (fp == NULL) {
        printf("\nErro. Nao foi possivel abrir o arquivo.\n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    while (ftell(fp) != filesize) {

        /* aloca e le o documento do arquivo */
        try = (char *) malloc (sizeof(char)*tamfixo);
        fread(try, sizeof(char), tamfixo, fp);

        /* compara o valor lido do arquivo
        com a chave buscada */
        if (!strcmp(chave, try)) {

            free(try);

            while (c != '#' && ftell(fp) != 1) {
                fseek(fp, -2, SEEK_CUR);
                fread(&c, sizeof(char), 1, fp);
            }
            if (ftell(fp) == 1) fseek(fp, 0, SEEK_SET);
            c = 0;

            r = criar_registro();

            /* leitrua de todos os campos fixos e
            armazenamento na struct registro */
            fread(r->doc, sizeof(char), tamfixo, fp);
            fread(r->dataHoraCadastro, sizeof(char), tamfixo, fp);
            fread(r->dataHoraAtualiza, sizeof(char), tamfixo, fp);
            fread(&r->ticket, sizeof(int), 1, fp);

            /* leitura do tamanho do dominio e
            seu conteúdo */
            fread(&tam, sizeof(int), 1, fp);
            r->dominio = (char *) malloc (sizeof(char)*tam);
            fread(r->dominio, sizeof(char), tam, fp);

            /* leitura do nome do registro */
            fread(&tam, sizeof(int), 1, fp);
            r->nome = (char *) malloc (sizeof(char)*tam);
            fread(r->nome, sizeof(char), tam, fp);

            /* leitura do nome da cidade */
            fread(&tam, sizeof(int), 1, fp);
            r->cidade = (char *) malloc (sizeof(char)*tam);
            fread(r->cidade, sizeof(char), tam, fp);

            /* leitura da UF */
            fread(&tam, sizeof(int), 1, fp);
            r->uf = (char *) malloc (sizeof(char)*tam);
            fread(r->uf, sizeof(char), tam, fp);

            fread(&delim, sizeof(char), 1, fp);

            imprimir_registro(r);
            apagar_registro(&r);
            printf("Digite ENTER para continuar a impressão");
            char *aux = readLine(stdin, '\n'); //espera o ENTER
            free(aux);
            cont++;

        }
        else {
            /* caso o campo lido seja diferente do
            buscado, o ponteiro avança até o fim do
            registro '#' */
            while (c != '#' && ftell(fp) != filesize)
                fread(&c, sizeof(char), 1, fp);
            c = 0;
            free(try);
        }
    }
    fclose(fp);

    if (cont == 0) printf("\nRegistro não encontrado.\n");
}

void busca_dominio_delim(char *chave) {

    FILE *fp = fopen("regdelim.bin", "r+");
    char *try;
    char c = 0;
    int tam;
    int tamfixo = 20;
    int pos = 3*tamfixo + sizeof(int);  //tamanho total dos campos fixos
    int filesize;
    char delim;

    /* deixa a chave em maiusculas e sem acentos */
    tira_acento_terminal(chave);
    strToupper(chave);

    if (fp == NULL) {
        printf("\nErro. Não foi possível abrir o arquivo.\n");
    }

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    while (ftell(fp) != filesize) {

        fseek(fp, pos, SEEK_CUR);

        fread(&tam, sizeof(int), 1, fp);
        try = (char *) malloc (sizeof(char)*tam);
        fread(try, sizeof(char), tam, fp);

        /* passa para maiuscula para comparacao */
        strToupper(try);

        if (!strcmp(chave, try)) {

            free(try);

            while (c != '#' && ftell(fp) != 1) {
                fseek(fp, -2, SEEK_CUR);
                fread(&c, sizeof(char), 1, fp);
            }
            if (ftell(fp) == 1) fseek(fp, 0, SEEK_SET);

            REG *r = criar_registro();

            /* leitrua de todos os campos fixos e
            armazenamento na struct registro */
            fread(r->doc, sizeof(char), tamfixo, fp);
            fread(r->dataHoraCadastro, sizeof(char), tamfixo, fp);
            fread(r->dataHoraAtualiza, sizeof(char), tamfixo, fp);
            fread(&r->ticket, sizeof(int), 1, fp);

            /* leitura do tamanho do dominio e
            seu conteúdo */
            fread(&tam, sizeof(int), 1, fp);
            r->dominio = (char *) malloc (sizeof(char)*tam);
            fread(r->dominio, sizeof(char), tam, fp);

            /* leitura do nome do registro */
            fread(&tam, sizeof(int), 1, fp);
            r->nome = (char *) malloc (sizeof(char)*tam);
            fread(r->nome, sizeof(char), tam, fp);

            /* leitura do nome da cidade */
            fread(&tam, sizeof(int), 1, fp);
            r->cidade = (char *) malloc (sizeof(char)*tam);
            fread(r->cidade, sizeof(char), tam, fp);

            /* leitura da UF */
            fread(&tam, sizeof(int), 1, fp);
            r->uf = (char *) malloc (sizeof(char)*tam);
            fread(r->uf, sizeof(char), tam, fp);

            fread(&delim, sizeof(char), 1, fp);

            fclose(fp);

            imprimir_registro(r);
            apagar_registro(&r);
            return;
        }
        else {
            /* anda ate encontrar o '#' */
            while (c != '#' && ftell(fp) != filesize)
                fread(&c, sizeof(char), 1, fp);
            c = 0;
            free(try);
        }
    }

    fclose(fp);

    printf("\nRegistro não encontrado.\n");
    return;
}

void busca_cidade_delim(char *chave) {

    FILE *fp = fopen("regdelim.bin", "r+");
    char *try;
    char c = 0;
    int tam;
    int i;
    int tamfixo = 20;
    int pos = (3*tamfixo)+sizeof(int);
    int filesize;
    char delim;
    int cont = 0;
    REG *r;

    tira_acento_terminal(chave);
    strToupper(chave);

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (fp == NULL) {
        printf("\nErro. Nao foi possivel abrir o arquivo.\n");
        return;
    }

    while (ftell(fp) != filesize) {

        fseek(fp, pos, SEEK_CUR);

        for (i = 0; i < 2; i++) {
            fread(&tam, sizeof(int), 1, fp);
            fseek(fp, tam, SEEK_CUR);
        }

        /* aloca e le o documento do arquivo */
        fread(&tam, sizeof(int), 1, fp);
        try = (char *) malloc (sizeof(char)*tam);
        fread(try, sizeof(char), tam, fp);

        strToupper(try);

        /* compara o valor lido do arquivo
        com a chave buscada */
        if (!strcmp(chave, try)) {

            free(try);

            while (c != '#' && ftell(fp) != 1) {
                fseek(fp, -2, SEEK_CUR);
                fread(&c, sizeof(char), 1, fp);
            }
            if (ftell(fp) == 1) fseek(fp, 0, SEEK_SET);
            c = 0;

            r = criar_registro();

            /* leitrua de todos os campos fixos e
            armazenamento na struct registro */
            fread(r->doc, sizeof(char), tamfixo, fp);
            fread(r->dataHoraCadastro, sizeof(char), tamfixo, fp);
            fread(r->dataHoraAtualiza, sizeof(char), tamfixo, fp);
            fread(&r->ticket, sizeof(int), 1, fp);

            /* leitura do tamanho do dominio e
            seu conteúdo */
            fread(&tam, sizeof(int), 1, fp);
            r->dominio = (char *) malloc (sizeof(char)*tam);
            fread(r->dominio, sizeof(char), tam, fp);

            /* leitura do nome do registro */
            fread(&tam, sizeof(int), 1, fp);
            r->nome = (char *) malloc (sizeof(char)*tam);
            fread(r->nome, sizeof(char), tam, fp);

            /* leitura do nome da cidade */
            fread(&tam, sizeof(int), 1, fp);
            r->cidade = (char *) malloc (sizeof(char)*tam);
            fread(r->cidade, sizeof(char), tam, fp);

            /* leitura da UF */
            fread(&tam, sizeof(int), 1, fp);
            r->uf = (char *) malloc (sizeof(char)*tam);
            fread(r->uf, sizeof(char), tam, fp);

            fread(&delim, sizeof(char), 1, fp);

            imprimir_registro(r);
            apagar_registro(&r);
            printf("Digite ENTER para continuar a impressão");
            char *aux = readLine(stdin, '\n'); //espera o ENTER
            free(aux);
            cont++;

        }
        else {
            /* caso o campo lido seja diferente do
            buscado, o ponteiro avança até o fim do
            registro '#' */
            while (c != '#' && ftell(fp) != filesize)
                fread(&c, sizeof(char), 1, fp);
            c = 0;
            free(try);
        }
    }
    fclose(fp);

    if (cont == 0) printf("\nRegistro não encontrado\n");
}

void busca_uf_delim(char *chave) {

    FILE *fp = fopen("regdelim.bin", "r+");
    char *try;
    char c = 0;
    int tam;
    int i;
    int tamfixo = 20;
    int pos = (3*tamfixo)+sizeof(int);
    int filesize;
    int cont = 0;
    char delim;

    REG *r;

    tira_acento_terminal(chave);
    strToupper(chave);

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (fp == NULL) {
        printf("\nErro. Nao foi possivel abrir o arquivo.\n");
        return;
    }

    while (ftell(fp) != filesize) {

        fseek(fp, pos, SEEK_CUR);

        for (i = 0; i < 3; i++) {
            fread(&tam, sizeof(int), 1, fp);
            fseek(fp, tam, SEEK_CUR);
        }

        /* aloca e le o documento do arquivo */
        fread(&tam, sizeof(int), 1, fp);
        try = (char *) malloc (sizeof(char)*tam);
        fread(try, sizeof(char), tam, fp);

        strToupper(try);

        /* compara o valor lido do arquivo
        com a chave buscada */
        if (!strcmp(chave, try)) {

            free(try);

            while (c != '#' && ftell(fp) != 1) {
                fseek(fp, -2, SEEK_CUR);
                fread(&c, sizeof(char), 1, fp);
            }
            if (ftell(fp) == 1) fseek(fp, 0, SEEK_SET);
            c = 0;

            r = criar_registro();

            /* leitrua de todos os campos fixos e
            armazenamento na struct registro */
            fread(r->doc, sizeof(char), tamfixo, fp);
            fread(r->dataHoraCadastro, sizeof(char), tamfixo, fp);
            fread(r->dataHoraAtualiza, sizeof(char), tamfixo, fp);
            fread(&r->ticket, sizeof(int), 1, fp);

            /* leitura do tamanho do dominio e
            seu conteúdo */
            fread(&tam, sizeof(int), 1, fp);
            r->dominio = (char *) malloc (sizeof(char)*tam);
            fread(r->dominio, sizeof(char), tam, fp);

            /* leitura do nome do registro */
            fread(&tam, sizeof(int), 1, fp);
            r->nome = (char *) malloc (sizeof(char)*tam);
            fread(r->nome, sizeof(char), tam, fp);

            /* leitura do nome da cidade */
            fread(&tam, sizeof(int), 1, fp);
            r->cidade = (char *) malloc (sizeof(char)*tam);
            fread(r->cidade, sizeof(char), tam, fp);

            /* leitura da UF */
            fread(&tam, sizeof(int), 1, fp);
            r->uf = (char *) malloc (sizeof(char)*tam);
            fread(r->uf, sizeof(char), tam, fp);

            fread(&delim, sizeof(char), 1, fp);

            imprimir_registro(r);
            apagar_registro(&r);
            printf("Digite ENTER para continuar a impressão");
            char *aux = readLine(stdin, '\n'); //espera o ENTER
            free(aux);
            cont++;

        }
        else {
            /* caso o campo lido seja diferente do
            buscado, o ponteiro avança até o fim do
            registro '#' */
            while (c != '#' && ftell(fp) != filesize)
                fread(&c, sizeof(char), 1, fp);
            c = 0;
            free(try);
        }
    }
    fclose(fp);

    if (cont == 0) printf("\nRegistro não encontrado.\n");
}

void busca_ticket_delim(int chave) {

    FILE *fp = fopen("regdelim.bin", "r+");
    int try;
    char c = 0;
    int tam;
    int tamfixo = 20;
    int pos = 3*tamfixo;  //tamanho total dos campos fixos
    int filesize;
    char delim;

    if (fp == NULL) {
        printf("\nErro. Nao foi possivel abrir o arquivo.\n");
    }

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    while (ftell(fp) != filesize) {

        fseek(fp, pos, SEEK_CUR);

        /* leitura do ticket do registro para
        comparacao */
        fread(&try, sizeof(int), 1, fp);

        if (chave == try) {

            while (c != '#' && ftell(fp) != 1) {
                fseek(fp, -2, SEEK_CUR);
                fread(&c, sizeof(char), 1, fp);
            }
            if (ftell(fp) == 1) fseek(fp, 0, SEEK_SET);

            REG *r = criar_registro();

            /* leitrua de todos os campos fixos e
            armazenamento na struct registro */
            fread(r->doc, sizeof(char), tamfixo, fp);
            fread(r->dataHoraCadastro, sizeof(char), tamfixo, fp);
            fread(r->dataHoraAtualiza, sizeof(char), tamfixo, fp);
            r->ticket = try;
            fseek(fp, sizeof(int), SEEK_CUR);

            /* leitura do tamanho do dominio e
            seu conteúdo */
            fread(&tam, sizeof(int), 1, fp);
            r->dominio = (char *) malloc (sizeof(char)*tam);
            fread(r->dominio, sizeof(char), tam, fp);

            /* leitura do nome do registro */
            fread(&tam, sizeof(int), 1, fp);
            r->nome = (char *) malloc (sizeof(char)*tam);
            fread(r->nome, sizeof(char), tam, fp);

            /* leitura do nome da cidade */
            fread(&tam, sizeof(int), 1, fp);
            r->cidade = (char *) malloc (sizeof(char)*tam);
            fread(r->cidade, sizeof(char), tam, fp);

            /* leitura da UF */
            fread(&tam, sizeof(int), 1, fp);
            r->uf = (char *) malloc (sizeof(char)*tam);
            fread(r->uf, sizeof(char), tam, fp);

            fread(&delim, sizeof(char), 1, fp);

            fclose(fp);

            imprimir_registro(r);
            apagar_registro(&r);
            return;
        }

        else {
            /* anda ate encontrar o '#' */
            while (c != '#' && ftell(fp) != filesize)
                fread(&c, sizeof(char), 1, fp);
            c = 0;
        }
    }

    fclose(fp);
    printf("\nRegistro não encontrado.\n");
}

void busca_nome_delim(char *chave) {

    FILE *fp = fopen("regdelim.bin", "r+");
    char *try;
    char c = 0;
    int tam;
    int tamfixo = 20;
    int pos = (3*tamfixo)+sizeof(int);
    int filesize;
    int cont = 0;
    char delim;

    REG *r = NULL;

    tira_acento_terminal(chave);
    strToupper(chave);

    if (fp == NULL) {
        printf("\nErro. Nao foi possivel abrir o arquivo.\n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    while (ftell(fp) != filesize) {

        /* passa o ponteiro pelos campos fixos  */
        fseek(fp, pos, SEEK_CUR);

        /* passa pelo dominio */
        fread(&tam, sizeof(int), 1, fp);
        fseek(fp, tam, SEEK_CUR);

        /* le o nome */
        fread(&tam, sizeof(int), 1, fp);
        try = (char *) malloc (sizeof(char)*tam);
        fread(try, sizeof(char), tam, fp);

        strToupper(try);

        /* compara o valor lido do arquivo
        com a chave buscada */
        if (!strcmp(chave, try)) {

            free(try);

            while (c != '#' && ftell(fp) != 1) {
                fseek(fp, -2, SEEK_CUR);
                fread(&c, sizeof(char), 1, fp);
            }
            if (ftell(fp) == 1) fseek(fp, 0, SEEK_SET);
            c = 0;

            r = criar_registro();

            /* leitrua de todos os campos fixos e
            armazenamento na struct registro */
            fread(r->doc, sizeof(char), tamfixo, fp);
            fread(r->dataHoraCadastro, sizeof(char), tamfixo, fp);
            fread(r->dataHoraAtualiza, sizeof(char), tamfixo, fp);
            fread(&r->ticket, sizeof(int), 1, fp);

            /* leitura do tamanho do dominio e
            seu conteúdo */
            fread(&tam, sizeof(int), 1, fp);
            r->dominio = (char *) malloc (sizeof(char)*tam);
            fread(r->dominio, sizeof(char), tam, fp);

            /* leitura do nome do registro */
            fread(&tam, sizeof(int), 1, fp);
            r->nome = (char *) malloc (sizeof(char)*tam);
            fread(r->nome, sizeof(char), tam, fp);

            /* leitura do nome da cidade */
            fread(&tam, sizeof(int), 1, fp);
            r->cidade = (char *) malloc (sizeof(char)*tam);
            fread(r->cidade, sizeof(char), tam, fp);

            /* leitura da UF */
            fread(&tam, sizeof(int), 1, fp);
            r->uf = (char *) malloc (sizeof(char)*tam);
            fread(r->uf, sizeof(char), tam, fp);

            fread(&delim, sizeof(char), 1, fp);

            imprimir_registro(r);
            apagar_registro(&r);
            printf("Digite ENTER para continuar a impressão");
            char *aux = readLine(stdin, '\n'); //espera o ENTER
            free(aux);
            cont++;

        }

        /* caso o campo lido seja diferente do
        buscado, o ponteiro avança até o fim do
        registro '#' */
        while (c != '#' && ftell(fp) != filesize)
            fread(&c, sizeof(char), 1, fp);
        c = 0;
        free(try);
    }
    fclose(fp);

    if (cont == 0) printf("\nRegistro não encontrado.\n");
}

void busca_n_delim(int n) {

    FILE *fp = fopen("regdelim.bin", "r+");
    int filesize;
    int delim = 1;
    char c = 0;
    int tam;
    int tamfixo = 20;

    if (fp == NULL) {
        printf("\nErro. Não foi possível abrir o arquivo.\n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    /* anda ate o numero de delimitadores
    encontrados for igual ao numero do
    registro buscado */
    while (delim != n) {
        fread(&c, sizeof(char), 1, fp);
        if (c == '#') delim++;
        if (ftell(fp) == filesize) break;
    }
    if (ftell(fp) == filesize) {
        fclose(fp);
        printf("\nRegistro não encontrado.\n");
        return;
    }

    /* salva o registro e retorna */
    REG *r = criar_registro();

    fread(r->doc, sizeof(char), tamfixo, fp);
    fread(r->dataHoraCadastro, sizeof(char), tamfixo, fp);
    fread(r->dataHoraAtualiza, sizeof(char), tamfixo, fp);
    fread(&r->ticket, sizeof(int), 1, fp);

    fread(&tam, sizeof(int), 1, fp);
    r->dominio = (char *) malloc (sizeof(char)*tam);
    fread(r->dominio, sizeof(char), tam, fp);

    fread(&tam, sizeof(int), 1, fp);
    r->nome = (char *) malloc (sizeof(char)*tam);
    fread(r->nome, sizeof(char), tam, fp);

    fread(&tam, sizeof(int), 1, fp);
    r->cidade = (char *) malloc (sizeof(char)*tam);
    fread(r->cidade, sizeof(char), tam, fp);

    fread(&tam, sizeof(int), 1, fp);
    r->uf = (char *) malloc (sizeof(char)*tam);
    fread(r->uf, sizeof(char), tam, fp);

    fclose(fp);

    imprimir_registro(r);
    apagar_registro(&r);
}

void busca_campo_registro_delim(int campo, int reg) {

    FILE *fp = fopen("regdelim.bin", "r+");
    int filesize;
    int delim = 0;
    char c = 0;
    int tam;
    int tamfixo = 20;

    if (fp == NULL) {
        printf("\nErro. Não foi possível abrir o arquivo.\n");
        return;
    }

    if (campo < 1 && campo > 8) {
        printf("\nCampo inválido.\n");
        return;
    }

    if (reg < 0) {
        printf("\nRegistro inválido\n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    /* anda ate o numero de delimitadores
    encontrados for igual ao numero do
    registro buscado */
    while (delim != reg) {
        fread(&c, sizeof(char), 1, fp);
        if (c == '#') delim++;
        if (ftell(fp) == filesize) break;
    }
    if (ftell(fp) == filesize) {
        fclose(fp);
        printf("\nRegistro não encontrado\n");
        return;
    }

    /* salva o registro e retorna */
    REG *r = criar_registro();

    fread(r->doc, sizeof(char), tamfixo, fp);
    fread(r->dataHoraCadastro, sizeof(char), tamfixo, fp);
    fread(r->dataHoraAtualiza, sizeof(char), tamfixo, fp);
    fread(&r->ticket, sizeof(int), 1, fp);

    fread(&tam, sizeof(int), 1, fp);
    r->dominio = (char *) malloc (sizeof(char)*tam);
    fread(r->dominio, sizeof(char), tam, fp);

    fread(&tam, sizeof(int), 1, fp);
    r->nome = (char *) malloc (sizeof(char)*tam);
    fread(r->nome, sizeof(char), tam, fp);

    fread(&tam, sizeof(int), 1, fp);
    r->cidade = (char *) malloc (sizeof(char)*tam);
    fread(r->cidade, sizeof(char), tam, fp);

    fread(&tam, sizeof(int), 1, fp);
    r->uf = (char *) malloc (sizeof(char)*tam);
    fread(r->uf, sizeof(char), tam, fp);

    switch(campo) {
        case 1:
            printf("\nCampo %d do registro %d: %s\n", campo, reg, r->dominio);
            break;
        case 2:
            printf("\nCampo %d do registro %d: %s\n", campo, reg, r->doc);
            break;
        case 3:
            printf("\nCampo %d do registro %d: %s\n", campo, reg, r->nome);
            break;
        case 4:
            printf("\nCampo %d do registro %d: %s\n", campo, reg, r->cidade);
            break;
        case 5:
            printf("\nCampo %d do registro %d: %s\n", campo, reg, r->uf);
            break;
        case 6:
            printf("\nCampo %d do registro %d: %s\n", campo, reg, r->dataHoraCadastro);
            break;
        case 7:
            printf("\nCampo %d do registro %d: %s\n", campo, reg, r->dataHoraAtualiza);
            break;
        case 8:
            printf("\nCampo %d do registro %d: %d\n", campo, reg, r->ticket);
            break;
    }

    fclose(fp);
}
