#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

int create_index(char *filename) {

	FILE *fpin = fopen(filename, "r+");
	FILE *fpout;
	int filesize;
	int offset;
	int ticket;
	int n_delim;
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

		offset = ftell(fpin);
		fseek(fpin, 60, SEEK_CUR);

		fread(&ticket, sizeof(int), 1, fpin);

		fwrite(&ticket, sizeof(int), 1, fpout);
		
		fwrite(&offset, sizeof(int), 1, fpout);

		while (n_delim < 2 && ftell(fpin) < filesize) {
            fread(&c, sizeof(char), 1, fpin);

            if(c == '#')
                n_delim ++;
            else if (n_delim > 0)
                n_delim = 0;
        }
		n_delim = 0;
	}

	free(idx_file);
	fclose(fpin);
	fclose(fpout);

	return 1;
}