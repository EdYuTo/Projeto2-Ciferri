#ifndef _REGDELIM_H
#define _REGDELIM_H

#include <utils.h>

int read_csv_delim();
void read_out_delim(char *);

void busca_documento_delim(char *chave, int *elem);
void busca_dominio_delim(char *chave);
void busca_cidade_delim(char *chave, int *elem);
void busca_uf_delim(char *chave, int *elem);
void busca_ticket_delim(int chave);
void busca_nome_delim(char *chave, int *elem);
void busca_n_delim(int n);
void busca_campo_registro_delim(int campo, int reg);

int remove_record(int ticket, char *file_bin, INDEX ***vector, int *size);

#endif
