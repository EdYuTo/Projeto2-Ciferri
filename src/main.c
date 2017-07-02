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

void printEst(){
	printf("\nEscolha qual arquivo mostrar\n");
	printf("   F - Arquivo first.bin\n");
	printf("   B - Arquivo best.bin\n");
	printf("   W - Arquivo worst.bin\n");
}

int main(int argc, char *argv[]){
	int i, j;
	int ticket;
	int nf, nb, nw;
	char opt;
	FILE *fileIn = fopen("turmaA-dadosDominios.csv", "r");
	INDEX **indexF = NULL, **indexB = NULL, **indexW = NULL;

	printOpt();

	while(opt != 'S'){
		scanf("%c", &opt);
		opt = toupper(opt);
		getchar();

		switch(opt){
			case 'G': //Gravação dos dados nos 3 arquivos de saída com seus respectivos índices
				if(fopen("best.bin", "rb") == NULL){ //Checa se os arquivos já foram criados
					read_csv_delim();
					create_index_file("first.bin") ? printf("Indice FIRST criado com sucesso!\n") : printf("ERRO: Indice FIRST não foi criado\n");
					create_index_file("best.bin") ? printf("Indice BEST criado com sucesso!\n") : printf("ERRO: Indice BEST não foi criado\n"); 
					create_index_file("worst.bin") ? printf("Indice WORST criado com sucesso!\n") : printf("ERRO: Indice WORST não foi criado\n");

					indexF = read_index_file("first.bin", &nf);
					indexB = read_index_file("best.bin", &nb);
					indexW = read_index_file("worst.bin", &nw);

					printf("\n** Todos os dados foram gravados **\n");
				}else{
					printf("\nOs arquivos de saída e de índice já foram criados\n");
				}
				printOpt();
				break;

			case 'R': //Remoção de registros
				if(indexB == NULL)
					indexB = read_index_file("best.bin", &nb);
				if(indexF == NULL)
					indexF = read_index_file("first.bin", &nf);
				if(indexW == NULL)
					indexW = read_index_file("worst.bin", &nw);


				printf("\nIndique o valor do Ticket correspondente ao registro que deseja remover\n");
				ticket = -1;
				while(ticket < 0){
					scanf("%d", &ticket);
					getchar();
					if(ticket < 0) printf("\nERRO: Digite um valor positivo\n");
				}

				remove_record_no_sort(ticket, "first.bin", &indexF, &nf) ? printf("Removido de FIRST com sucesso!\n") : printf("ERRO na remoção de FIRST\n");
				remove_record_ascending_sort(ticket, "best.bin", &indexB, &nb)  ? printf("Removido de BEST com sucesso!\n") : printf("ERRO na remoção de BEST\n");;
				remove_record_descending_sort(ticket, "worst.bin", &indexW, &nw)  ? printf("Removido de WORST com sucesso!\n") : printf("ERRO na remoção de WORST\n");;

				printOpt();
				break;
			case 'I': //Inserção de registros
				if(indexB == NULL)
					indexB = read_index_file("best.bin", &nb);
				if(indexF == NULL)
					indexF = read_index_file("first.bin", &nf);
				if(indexW == NULL)
					indexW = read_index_file("worst.bin", &nw);
				REG *reg = le_registro();

				insert_reg_first_fit("first.bin", reg, &indexF, &nf);
				insert_worstFit("worst.bin", &indexW, &nw, reg);
			//...
				printOpt();
				break;
			case 'V': //Estatísticas dos índices
				if(indexB == NULL)
					indexB = read_index_file("best.bin", &nb);
				if(indexF == NULL)
					indexF = read_index_file("first.bin", &nf);
				if(indexW == NULL)
					indexW = read_index_file("worst.bin", &nw);

			//...
				printOpt();
				break;
			case 'E': //Estatísticas dos registros
				printEst();

				char opt2;
				scanf("%c", &opt2);
				opt2 = toupper(opt2);
				getchar();

				if(opt2 == 'F')
					show_list("first.bin");
				else if(opt2 == 'B')
					show_list("best.bin");
				else if(opt2 == 'W')
					show_list("worst.bin");

				getchar();
				printOpt();
				break;

			default: //Erro
					if(opt == 'S'){
						write_index_file(&indexB, &nb, "best.idx");
						write_index_file(&indexF, &nf, "first.idx");
						write_index_file(&indexW, &nw, "worst.idx");
						break;
					}
					printf("\nERRO: Digite uma operação válida\n");
					printOpt();
					break;
		}
	}
	return 0;
}