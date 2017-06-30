#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <heap.h>

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

INDEX **read_index_file(char *filename, int *nIndex){
	if(filename != NULL) {
		filename = makeIndex(filename);

		FILE *fp = fopen(filename, "r+");
		int status, filesize, counter = 0;
		INDEX **indices = NULL;

		if(fp != NULL){
			fread(&status, sizeof(int), 1, fp);

			if(status != 0){
				create_index(filename);
			}

			fseek(fp, 0, SEEK_END);
			filesize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			status = 1;
			fwrite(&status, sizeof(int), 1, fp);

			while(ftell(fp) < filesize){
				indices = realloc(indices, sizeof(INDEX*) * (counter + 1));
				indices[counter] = criar_index();

				fread(&(indices[counter]->ticket), sizeof(int), 1, fp);
				fread(&(indices[counter]->byteOffset), sizeof(int), 1, fp);

				counter++;
			}
		}
		*nIndex = counter;
		return indices;
	}
	return NULL;
}