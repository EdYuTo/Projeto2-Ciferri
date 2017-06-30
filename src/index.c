#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <heap.h>
#include "utils.h"

int create_index(char *filename) {

	FILE *fpin = fopen(filename, "r+");
	FILE *fpout;
	INDEX **indices = NULL;
	int filesize, counter = 0;
	int n_delim, i;
	char c;
	char *idx_file;

	idx_file = makeIndex(filename);
	fpout = fopen(idx_file, "w+");

	if (fpin == NULL) {
		printf("Erro! Não foi possível abrir o arquivo de dados\n");
		return 0;
	}


	fseek(fpin, 0, SEEK_END);
	filesize = ftell(fpin);
	fseek(fpin, 0, SEEK_SET);

	while (ftell(fpin) != filesize) {
		indices = realloc(indices, sizeof(INDEX*) * (counter + 1));
		indices[counter] = criar_index();

		indices[counter]->byteOffset = ftell(fpin);
		fseek(fpin, 60, SEEK_CUR);

		fread(&(indices[counter]->ticket), sizeof(int), 1, fpin);
		counter++;


		while (n_delim < 2 && ftell(fpin) < filesize) {
            fread(&c, sizeof(char), 1, fpin);

            if(c == '#')
                n_delim ++;
            else if (n_delim > 0)
                n_delim = 0;
        }
		n_delim = 0;
	}

	//Ordena os indices
    heap_sort(indices, counter);

	//Escreve registro de cabeçalho
	int status = 0;
	fwrite(&status, sizeof(int), 1, fpout);

	//Escreve indices
	for(i = 0; i < counter; i++) {
		fwrite(&(indices[i]->ticket), sizeof(int), 1, fpout);

		fwrite(&(indices[i]->byteOffset), sizeof(int), 1, fpout);
	}
	free(idx_file);
	fclose(fpin);
	fclose(fpout);

	return 1;
}

