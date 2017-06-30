#include <stdlib.h>
#include <stdio.h>

int create_index() {

	FILE *fpin = fopen("first.bin", "r+");
	FILE *fpout1 = fopen("best.idx", "w+");
	FILE *fpout2 = fopen("worst.idx", "w+");
	FILE *fpout3 = fopen("first.idx", "w+");
	int filesize;
	int offset;
	int ticket;
	int n_delim;
	char c;
	const char delim = '#';

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

		fwrite(&ticket, sizeof(int), 1, fpout1);
		fwrite(&ticket, sizeof(int), 1, fpout2);
		fwrite(&ticket, sizeof(int), 1, fpout3);
		
		fwrite(&offset, sizeof(int), 1, fpout1);
		fwrite(&offset, sizeof(int), 1, fpout2);
		fwrite(&offset, sizeof(int), 1, fpout3);

		fwrite(&delim, sizeof(char), 1, fpout1);
		fwrite(&delim, sizeof(char), 1, fpout1);

		fwrite(&delim, sizeof(char), 1, fpout2);
		fwrite(&delim, sizeof(char), 1, fpout2);
		
		fwrite(&delim, sizeof(char), 1, fpout3);
		fwrite(&delim, sizeof(char), 1, fpout3);

		while (n_delim < 2 && ftell(fpin) < filesize) {
            fread(&c, sizeof(char), 1, fpin);

            if(c == '#')
                n_delim ++;
            else if (n_delim > 0)
                n_delim = 0;
        }
		n_delim = 0;
	}

	fclose(fpin);
	fclose(fpout1);
	fclose(fpout2);
	fclose(fpout3);

	return 1;
}