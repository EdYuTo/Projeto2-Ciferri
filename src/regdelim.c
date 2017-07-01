#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>

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
    char *str;
    char aux[500];
    int tamfixo = 20;
    char delim;
    int filesize;

    int n_delim = 0;

    r = criar_registro();

    while(n_delim < 2) {
        /* ignorando o cabecalho */
        fread(&ticket, sizeof(int), 1, fp);
        
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
    
    printf("Digite ENTER para comecar a impressão ou ctrl+D para sair");
    while (ftell(fp) < filesize && fgetc(stdin) != EOF) {
        r = read_register(fp);
        imprimir_registro(r);
        apagar_registro(&r);

        printf("Digite ENTER para continuar a impressão ou ctrl+D para sair");
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

    while (ftell(fp) < filesize) {

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

            r = read_register(fp);
            imprimir_registro(r);
            apagar_registro(&r);

            printf("Digite ENTER para continuar a impressão");
            char *aux = readLine(stdin, '\n'); //espera o ENTER
            free(aux);
            cont++;

        }
        else {
            int n_delim = 0;

            /* caso o campo lido seja diferente do
            buscado, o ponteiro avança até o fim do
            registro '#' */
            while (n_delim < 2 && ftell(fp) < filesize) {
                fread(&c, sizeof(char), 1, fp);

                if(c == '#')
                    n_delim ++;
                else if (n_delim > 0)
                    n_delim = 0;
            }
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

    while (ftell(fp) < filesize) {

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

            REG *r = read_register(fp);
            fclose(fp);

            imprimir_registro(r);
            apagar_registro(&r);
            return;
        }
        else {
            int n_delim = 0;

            /* anda ate encontrar o '#' */
            while (n_delim < 2 && ftell(fp) < filesize) {
                fread(&c, sizeof(char), 1, fp);
                if (c == '#')
                    n_delim++;
                else if(n_delim > 0)
                    n_delim = 0;
            }
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

    while (ftell(fp) < filesize) {

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

            r = read_register(fp);
            imprimir_registro(r);
            apagar_registro(&r);

            printf("Digite ENTER para continuar a impressão");
            char *aux = readLine(stdin, '\n'); //espera o ENTER
            free(aux);
            cont++;

        }
        else {
            int n_delim = 0;

            /* caso o campo lido seja diferente do
            buscado, o ponteiro avança até o fim do
            registro '#' */
            while (n_delim < 2 && ftell(fp) < filesize) {
                int aux = ftell(fp);
                fread(&c, sizeof(char), 1, fp);

                if (c == '#')
                    n_delim++;
                else if(n_delim > 0)
                    n_delim = 0;
            }
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

    while (ftell(fp) < filesize) {

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

            r = read_register(fp);
            imprimir_registro(r);
            apagar_registro(&r);

            printf("Digite ENTER para continuar a impressão");
            char *aux = readLine(stdin, '\n'); //espera o ENTER
            free(aux);
            cont++;

        }
        else {
            int n_delim = 0;
            /* caso o campo lido seja diferente do
            buscado, o ponteiro avança até o fim do
            registro '#' */
            while (n_delim < 2 && ftell(fp) < filesize) {
                fread(&c, sizeof(char), 1, fp);

                if (c == '#')
                    n_delim++;
                else if(n_delim > 0)
                    n_delim = 0;
            }
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

    while (ftell(fp) < filesize) {

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

            REG *r = read_register(fp);
            fclose(fp);

            imprimir_registro(r);
            apagar_registro(&r);
            return;
        }

        else {
            int n_delim = 0;
            /* anda ate encontrar o '#' */
            while (n_delim < 2 && ftell(fp) < filesize) {
                fread(&c, sizeof(char), 1, fp);

                if (c == '#')
                    n_delim++;
                else if(n_delim > 0)
                    n_delim = 0;
            }
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

    while (ftell(fp) < filesize) {

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

            r = read_register(fp);
            imprimir_registro(r);
            apagar_registro(&r);

            printf("Digite ENTER para continuar a impressão");
            char *aux = readLine(stdin, '\n'); //espera o ENTER
            free(aux);
            cont++;

        }
        else {

            int n_delim = 0;
            /* caso o campo lido seja diferente do
            buscado, o ponteiro avança até o fim do
            registro '#' */
            while (n_delim < 2 && ftell(fp) < filesize) {
                fread(&c, sizeof(char), 1, fp);

                if (c == '#')
                    n_delim++;
                else if (n_delim > 0)
                    n_delim = 0;
            }
            c = 0;
            free(try);
        }
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
    int n_delim = 0;
    while (delim != n) {
        fread(&c, sizeof(char), 1, fp);

        if (c == '#') n_delim++;
        else if(n_delim > 0) n_delim = 0;

        if(n_delim >= 2){
            delim++;
            n_delim = 0;
        }
        if (ftell(fp) == filesize) break;
    }
    if (ftell(fp) == filesize) {
        fclose(fp);
        printf("\nRegistro não encontrado.\n");
        return;
    }

    REG *r = read_register(fp);
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
    int n_delim = 0;
    while (delim != reg) {
        fread(&c, sizeof(char), 1, fp);

        if (c == '#') n_delim++;
        else if(n_delim > 0) n_delim = 0;

        if(n_delim >= 2){
            n_delim = 0;
            delim++;
        }
        if (ftell(fp) == filesize) break;
    }
    if (ftell(fp) == filesize) {
        fclose(fp);
        printf("\nRegistro não encontrado\n");
        return;
    }

    REG *r = read_register(fp);

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

/* retorna o tamanho do registro
que inicia em 'offset' */
int record_size(FILE *fp, int offset) {

    char c;
    int n_delim = 0;
    int filesize, k = 0;
    int counter = 0;

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

void remove_from_index(INDEX **vector, int size, int k) {

    int i;

    for (i = k; i < size-1; i++) {
        vector[i] = vector[i+1];
    }
    *vector = (INDEX *) realloc (*vector, sizeof(INDEX)*(size-1));
    return;
}

int remove_record(int ticket, char *file_bin, INDEX **vector, int size) {

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
    result = binary_search(*vector, ticket, 0, size-1);

    if (result == -1) {
        printf("Ticket não encontrado.\n");
        fclose(fp_bin);
        return 0;
    }

    /* tamanho do registro a ser removido */
    rec_size = record_size(fp_bin, (*vector)[result].byteOffset);

    /* marca como removido, marca o offset do
    ultimo registro removido e o tamanho do registro
    recem removido */
    fwrite(&removed, sizeof(char), 1, fp_bin);
    fwrite(&head, sizeof(int), 1, fp_bin);
    fwrite(&rec_size, sizeof(int), 1, fp_bin);


    /* escreve o offset no cabeçalho */
    fseek(fp_bin, 0, SEEK_SET);
    fwrite(&(*vector)[result].byteOffset, sizeof(int), 1, fp_bin);

    /* remove do vetor de indices */
    remove_from_index(vector, size, result);

    fclose(fp_bin);

    return 1;
}
