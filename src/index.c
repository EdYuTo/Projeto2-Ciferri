#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <heap.h>

int create_index_file(char *filename) {

	FILE *fpin = fopen(filename, "r+");
	FILE *fpout;
	INDEX **indices = NULL;
	int filesize, counter = 0;
	int n_delim = 0, i;
	char c;
	char *idx_file;
	int head;

	idx_file = makeIndex_filename(filename);
	fpout = fopen(idx_file, "w+");

	if (fpin == NULL) {
		printf("Erro! Não foi possível abrir o arquivo de dados\n");
		if (fpout)
			fclose(fpout);
		return 0;
	}

	fseek(fpin, 0, SEEK_END);
	filesize = ftell(fpin);
	fseek(fpin, 0, SEEK_SET);

	fread(&head, sizeof(int), 1, fpin);

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
		
		free(indices[i]);
	}
	free(indices);
	free(idx_file);
	fclose(fpin);
	fclose(fpout);

	return 1;
}

INDEX **read_index_file(char *filename, int *nIndex){
   if(filename != NULL) {
      char *name = makeIndex_filename(filename);

      FILE *fp = fopen(name, "r+");
      int status, filesize, counter = 0;
      INDEX **indices = NULL;

      if(fp != NULL){
         fread(&status, sizeof(int), 1, fp);

         if(status != 0){
            create_index_file(filename);
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
         fclose(fp);
      }
      *nIndex = counter;
      free(name);
      return indices;
   }
   return NULL;
}

void write_index_file(INDEX ***indices, int *nIndices, char *filename){
	if(indices != NULL && *indices != NULL && filename != NULL){
		int i, status = 0;
		FILE *fp = fopen(filename, "w+");

		//Escreve registro de cabeçalho
		fwrite(&status, sizeof(int), 1, fp);

		for(i = 0; i < *nIndices; i++){
			fwrite(&((*indices)[i]->ticket), sizeof(int), 1, fp);
			fwrite(&((*indices)[i]->byteOffset), sizeof(int), 1, fp);
		}

		//Libera o vetor de indices da memoria
		for(i = 0; i < *nIndices; i++){
			apagar_index(&(*indices)[i]);
		}
		free(*indices);
		*indices = NULL;
		*nIndices = 0;


		fclose(fp);
	}
}

void show_indices(INDEX **indicesF, INDEX **indicesB, INDEX **indicesW, int nf, int nb, int nw){
	if(indicesF != NULL && indicesB != NULL && indicesW != NULL){
		printf("First Fit\tBest Fit\tWorst Fit\n");
		printf("Quantidade: %d\tQuantidade: %d\tQuantidade: %d\n", nf, nb, nw);
		printf("-------------------------------------------------\n\n");
		int i;

		printf("Digite ENTER para começar a impressão ou ctrl+D para sair\n");
		for(i = 0; i < nb && fgetc(stdin) != EOF; i++){
			printf("First Fit\t\tBest Fit\t\tWorst Fit\n");
			printf("Ticket: %d\t\tTicket: %d\t\tTicket: %d\n", indicesF[i]->ticket, indicesB[i]->ticket, indicesW[i]->ticket);
			printf("Byte Offset: %d\tByte Offset: %d\tByte Offset: %d\n", indicesF[i]->byteOffset, indicesB[i]->byteOffset, indicesW[i]->byteOffset);
			printf("-------------------------------------------------\n\n");
			printf("Digite ENTER para continuar a impressão ou ctrl+D para sair\n");
		}

	}

}
