//Bruno Flavio - 9791330
//Edson Yudi - 9791305
//Luca Porto - 9778943
//Murilo Baldi - 9790990

/*
															Turma A
										Projeto 2 - Dados de Dominios de Órgãos Públicos
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <heap.h>
#include <myregex.h>
#include <index.h>
#include <regdelim.h>
#include <utils.h>

void printOpt(){
	printf("\nEscolha a operação que deseja realizar\n");
	if((fopen("best.bin", "rb") == NULL)) printf("   G - Gravação dos dados do arquivo de entrada nos 3 arquivos de saída \n");
	printf("   R - Remoção de registro\n");
	printf("   I - Inserção de registro\n");
	printf("   V - Visualização de estatísticas sobre os arquivos de índice\n");
	printf("   E - Visualização de estatísticas sobre as listas de registros removidos\n");
	printf("   S - Para finalizar o programa\n\n");
}

int main(int argc, char *argv[]){
	int i, j;
	int ticket;
	char opt;
	FILE *fileIn = fopen("turmaA-dadosDominios.csv", "r");

	printOpt();

	while(opt != 'S'){
		scanf("%c", &opt);
		getchar();

		switch(opt){
			case 'G': //Gravação dos dados nos 3 arquivos de saída com seus respectivos índices
				if(fopen("best.bin", "rb") == NULL){ //Checa se os arquivos já foram criados
					read_csv_delim();
					create_index_file("best.bin");
					create_index_file("worst.bin");
					create_index_file("first.bin");

					printf("\nDados gravados\n");
				}else{
					printf("\nOs arquivos de saída já foram criados\n");
				}
				printOpt();
				break;

			case 'R': //Remoção de registros
				printf("\nIndique o valor do Ticket correspondente ao registro que deseja remover\n");
				ticket = -1;
				while(ticket < 0){
					scanf("%d", &ticket);
					getchar();
					if(ticket < 0) printf("\nERRO: Digite um valor positivo\n");
				}

				//remove_record_no_sort(int ticket, char *file_bin, INDEX **vector, int size);
				printOpt();
				break;
			case 'I': //Inserção de registros

			//...
				printOpt();
				break;
			case 'V': //Estatísticas dos índices

			//...
				printOpt();
				break;
			case 'E': //Estatísticas dos registros

			//...
				printOpt();
				break;

			default: //Erro
					if(opt == 'S') break;
					printf("\nERRO: Digite uma operação válida\n");
					printOpt();
					break;
		}
	}
	return 0;
}