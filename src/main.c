//Bruno Flavio - 9791330
//Edson Yudi - 9791305
//Luca Porto - 9778943
//Murilo Baldi - 9790990

/*
															Turma A

										Projeto 1 - Dados de Dominios de Órgãos Públicos
*/


#include <regdelim.h>	  //Delimitador

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <stdio.h>


/* Função de impressão das possíveis operações*/
void printOpt(){
	printf("\nEscolha a operação que deseja realizar\n");
	printf("G - Gravação dos dados do arquivo de entrada no arquivo de saída \n");
	printf("R - Para selecionar o tipo de recuperação de dados\n");
	printf("S - Para sair do programa\n\n");
}

/*Função de impressão após gravar o arquivo de saída*/
void printOptR(){
	printf("\nEscolha a operação que deseja realizar\n");
	printf("R - Para selecionar o tipo de recuperação de dados\n");
	printf("S - Para sair do programa\n\n");
}

/*Função de impressão para seleção de recuperação de dados*/
void printR(){
	printf("\nA - Recuperação dos dados de todos os registros, um por vez  \n");
	printf("B - Recuperação de todos os registros a partir de um critério(domínio, ticket...), um  por vez \n");
	printf("C - Recuperação dos dados de um registro pelo seu número (0, 1, 2, 3...)  \n");
	printf("D - Recuperação de um campo de um registro específico, de acordo com seus números  \n");
	printf("S - Para sair do programa\n\n");
}

/*Função de impressão dos critérios de recuperação*/
void printCrit(){
	printf("\nEscolha o critério a ser usado na busca:\n");
	printf("1 - Domínio\n");
	printf("2 - Documento\n");
	printf("3 - Nome\n");
	printf("4 - Cidade\n");
	printf("5 - Unidade da Federação (UF)\n");
	printf("6 - Ticket\n");
}

int main(int argc, char *argv[]){
	int i, j, write, Gcount = 0;
	int regType, crit;
	char opt = 'Z', type;
	char *chave;
	int rrn, nCampo, elem, ticket;
	REG *r;
	int registro, campo;

	FILE *fileIn = fopen("turmaA-dadosDominios.csv", "r");
	FILE *fileOut = NULL;

	printf("\nDigite o tipo de registro com tamanho variável desejado\n");
	printf("1 - Com indicador de tamanho\n");
	printf("2 - Número fixo de campos\n");
	printf("3 - Delimitador entre registros\n\n");

	scanf("%d", &regType);
	getchar();

	/*
	if(regType == 1){ //Indicador de tamanho
		printOpt();

		while(opt != 'S'){
			scanf("%c", &opt);
			getchar();

			switch(opt){
				case 'G': //Gravação dos dados do arquivo de entrada no arquivo 'saídaIT.bin'
					if(Gcount == 0){
						fileOut = fopen("saidaIT.bin", "wb");
						write = escrever_arquivo_indicador_tamanho(fileIn, fileOut);

						if(write == 1){
							printf("Programa gravado em saidaIT.bin...\n\n");
						}else{
							printf("Erro, não foi possível gravar o arquivo de saída\n\n");
						}
						fclose(fileOut);
						Gcount++; //Auxiliar para avisar que o arquivo de saída ja foi criado

						printOptR();
						break;
					}else if(Gcount != 0){
						printf("Arquivo de saída ja foi criado\n\n");
					}
					break;

				case 'R': //Para selecionar o tipo de recuperação de dados
					fileOut = fopen("saidaIT.bin", "r"); //Abertura do arquivo para leitura e recuperação de dados
					if(fileOut == NULL){
						printf("ERRO: Arquivo binário de saída não criado. Digite G para criar e escrever no arquivo\n\n");
						break;
					}
					printR();
					scanf("%c", &type); //Tipo de recuperação a ser feita
					getchar();

					switch(type){
						case 'A': //Recuperação dos dados de todos os registros, um por vez
							ler_arquivo_indicador_tamanho(fileOut);
							printOptR();
							break;

						case 'B': //Recuperação dos dados de todos os registros a partir de um critério(domínio, ticket...)
							printCrit();
							scanf("%d", &crit);


							if(crit == 1) {
								printf("Digite o domínio: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');

								busca_registro_indicador_tamanho_dominio(fileOut, chave);
							}

							else if(crit == 2){
								printf("Digite o documento: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');

								busca_registro_indicador_tamanho_doc(fileOut, chave);
								free(chave);
							}

							else if(crit == 3){
								printf("Digite o nome: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');

								busca_registro_indicador_tamanho_nome(fileOut, chave);
								free(chave);
							}

							else if(crit == 4){
								printf("Digite a cidade: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');

								busca_registro_indicador_tamanho_cidade(fileOut, chave);
								free(chave);
							}

							else if(crit == 5){
								printf("Digite a UF: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');

								busca_registro_indicador_tamanho_uf(fileOut, chave);
								free(chave);
							}

							else if(crit == 6){
								printf("Digite o ticket: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								ticket = atoi(chave);

								busca_registro_indicador_tamanho_ticket(fileOut, ticket);
								free(chave);
							}

							printOptR();
							break;

						case 'C': //Recuperação dos dados de um registro pelo seu número (1º, 2ª, 3º...)
							printf("Digite o número do registro(RRN): ");
							scanf("%d", &rrn);

							busca_registro_indicador_tamanho(fileOut, rrn);
							printOptR();
							break;

						case 'D'://Recuperação de um campo de um registro específico, de acordo com seus números
							printf("Digite o número do registro(RRN): ");
							scanf("%d", &rrn);
							printf("Escolha o número do campo\n");
							printf("-->Dominio, digite 1\n");
							printf("-->Doc, digite 2\n-->Nome, digite 3\n-->Cidade, digite 4\n");
                                                        printf("-->UF, digite 5\n");
							printf("-->Data e hora do cadastro, digite 6\n");
							printf("-->Data e hora da atualizacao, digite 7\n");
							printf("-->Ticket, digite 8\n");
							scanf("%d", &nCampo);

							busca_campo_indicador_tamanho(fileOut, rrn, nCampo);
							printOptR();
							break;

						case 'S'://Finalizar programa
							printf("Programa finalizado\n");
							return 0;

						default: //Erro
							printf("Digite um tipo de recuperação válido\n");
							printR();
					}
					fclose(fileOut);
					__fpurge(stdin);
					break;

				case 'S': //Sair do programa
					printf("Programa finalizado\n");
					break;

				default: //Erro
					printf("Digite uma operação válida\n");
					if(Gcount == 0){
						printOpt();
					}else if(Gcount != 0){
						printOptR();
					}
					break;
			}
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if(regType == 2){ //Número fixo de campos
		Gcount = 0;
		printOpt();

		while(opt != 'S'){
			scanf("%c", &opt);
			getchar();

			switch(opt){
				case 'G':
					if(Gcount == 0){
						fileOut = fopen("saidaNFC.bin", "wb");

						write = escrever_arquivo_nfixo_campos(fileIn, fileOut); //Função que ira escrever os dados no arquivo de saída

						if(write == 1){
							printf("Programa gravado em saidaNFC.bin...\n\n");
						}else{
							printf("Erro, não foi possível gravar o arquivo de saída\n\n");
						}
						fclose(fileOut);
						Gcount++; //Auxiliar para avisar que o arquivo de saída ja foi criado

						printOptR();
						break;
					}else if(Gcount != 0){
						printf("Arquivo de saída ja foi criado\n\n");
					}
					break;


				case 'R': //Para selecionar o tipo de recuperação de dados
					fileOut = fopen("saidaNFC.bin", "r"); //Abertura do arquivo para leitura e recuperação de dados
					if(fileOut == NULL){
						printf("ERRO: Arquivo binário de saída não criado. Digite G para criar e escrever no arquivo\n\n");
						break;
					}
					printR();
					scanf("%c", &type); //Tipo de recuperação a ser feita
					getchar();


					switch(type){
						case 'A': //Recuperação dos dados de todos os registros, um por vez
							ler_arquivo_nfixo_campos(fileOut);
							printOptR();
							break;

						case 'B': //Recuperação dos dados de todos os registros a partir de um critério(domínio, ticket...)
							printCrit();
							scanf("%d", &i);
							if (i == 1) {
								printf("Digite o dominio desejado: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								busca_registro_nfixo_campos_dominio(fileOut, chave);
							} else if (i == 2) {
								printf("Digite o doc desejado: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								busca_registro_nfixo_campos_doc(fileOut, chave);
							} else if (i == 3) {
								printf("Digite o nome desejado: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								busca_registro_nfixo_campos_nome(fileOut, chave);
							} else if (i == 4) {
								printf("Digite a cidade desejada: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								busca_registro_nfixo_campos_cidade(fileOut, chave);
							} else if (i == 5) {
								printf("Digite o UF desejado: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								busca_registro_nfixo_campos_uf(fileOut, chave);
							} else if (i == 6) {
								printf("Digite o ticket desejado: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								j = atoi(chave);
								busca_registro_nfixo_campos_ticket(fileOut, j);
							}
							free(chave);
							printOptR();
							break;

						case 'C': //Recuperação dos dados de um registro pelo seu número (1º, 2ª, 3º...)
							printf("Digite o numero do registro desejado:\n");
							scanf("%d", &i);
							busca_registro_nfixo_campos_campo(fileOut, i, -1);
							printOptR();
							break;

						case 'D'://Recuperação de um campo de um registro específico, de acordo com seus números
							printf("Digite o número do registro(RRN): ");
							scanf("%d", &i);
							printf("Escolha o número do campo\n");
							printf("-->Dominio, digite 1\n");
							printf("-->Doc, digite 2\n-->Nome, digite 3\n-->Cidade, digite 4\n");
                                                        printf("-->UF, digite 5\n");
							printf("-->Data e hora do cadastro, digite 6\n");
							printf("-->Data e hora da atualizacao, digite 7\n");
							printf("-->Ticket, digite 8\n");
							scanf("%d", &j);
							busca_registro_nfixo_campos_campo(fileOut, i, j);
							printOptR();
							break;

						case 'S'://Finalizar programa
							printf("Programa finalizado\n");
							return 0;

						default: //Erro
							printf("Digite um tipo de recuperação válido\n");
							printOpt();
					}
					__fpurge(stdin);
					fclose(fileOut);
					break;

				case 'S': //Sair do programa
					printf("Programa finalizado\n");
					break;

				default: //Erro
					printf("Digite uma operação válida\n");
					printOpt();
					break;
			}
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else*/ if(regType == 3){ //Delimitador
		Gcount = 0;

		while(opt != 'S'){
			printOpt();
			scanf("%c", &opt);
			opt = toupper(opt);
			getchar();

			switch(opt){

				case 'G':
					if(Gcount == 0){

					write = read_csv_delim(); //Função que ira escrever os dados no arquivo de saída

					if(write == 1) {
						printf("\nPrograma gravado em regdelim.bin...\n\n");
						Gcount++; //Auxiliar para avisar que o arquivo de saída ja foi criado
					}
					else
						printf("\nErro, não foi possível gravar o arquivo de saída\n\n");
					}
					else if(Gcount != 0){
						printf("\nArquivo de saída ja foi criado\n\n");
					}
					break;

				case 'R': //Para selecionar o tipo de recuperação de dados
					fileOut = fopen("regdelim.bin", "r");
					if(fileOut == NULL){
						printf("ERRO: Arquivo binário de saída não criado. Digite G para criar e escrever no arquivo\n\n");
						break;
					}
					fclose(fileOut);

					printR();
					scanf("%c", &type); //Tipo de recuperação a ser feita
					type = toupper(type);
					getchar();

					switch(type){
						case 'A': //Recuperação dos dados de todos os registros, um por vez
							read_out_delim();
							break;

						case 'B': //Recuperação dos dados de todos os registros a partir de um critério(domínio, ticket...)
							printf("\nEscolha o critério a ser usado na busca:\n");
							printf("1 - Domínio\n");
							printf("2 - Documento\n");
							printf("3 - Nome\n");
							printf("4 - Cidade\n");
							printf("5 - Unidade da Federação (UF)\n");
							printf("6 - Ticket\n");
							scanf("%d", &crit);

							if (crit == 1) {
								printf("\nDigite o domínio: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								busca_dominio_delim(chave);
								free(chave);
							}
							else if (crit == 2) {
								printf("\nDigite o documento: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								busca_documento_delim(chave, &elem);
								free(chave);
							}
							else if (crit == 3) {
								printf("\nDigite o nome: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								busca_nome_delim(chave, &elem);
								free(chave);
							}
							else if (crit == 4) {
								printf("\nDigite a cidade: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								busca_cidade_delim(chave, &elem);
								free(chave);
							}
							else if (crit == 5) {
								printf("\nDigite a UF: ");
								__fpurge(stdin);
								chave = readLine(stdin, '\n');
								busca_uf_delim(chave, &elem);
								free(chave);
							}
							else if (crit == 6) {
								printf("\nDigite o número do ticket: ");
								scanf("%d", &ticket);
								busca_ticket_delim(ticket);
							}
							break;

						case 'C': //Recuperação dos dados de um registro pelo seu número (1º, 2ª, 3º...)
							printf("\nDigite o número do registro a ser buscado: ");
							scanf("%d", &rrn);
							busca_n_delim(rrn);
							break;

						case 'D'://Recuperação de um campo de um registro específico, de acordo com seus números
							printf("Digite o numero do registro: ");
							scanf("%d", &registro);
							printf("Digite o numero do campo: ");
							scanf("%d", &campo);
							busca_campo_registro_delim(campo,registro);
							__fpurge(stdin);
							break;

						case 'S'://Finalizar programa
							printf("Programa finalizado\n");
							return 0;

						default: //Erro
							printf("\nDigite um tipo de recuperação válido\n");
					}
					break;

				case 'S': //Sair do programa
					printf("Programa finalizado\n");
					break;

				default: //Erro
					printf("\nDigite uma operação válida\n");
					break;
			}

		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}else{
		printf("Não foi possível identificar o tipo de registro, digite um número válido\n");
	}
	if (fileIn != NULL)
		fclose(fileIn);
	return 0;
}
