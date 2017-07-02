#ifndef _REGDELIM_H
#define _REGDELIM_H

#include <utils.h>

int read_csv_delim();
void read_out_delim(char *);

int remove_record_no_sort(int ticket, char *file_bin, INDEX ***vector, int *size);
int remove_record_ascending_sort(int ticket, char *file_bin, INDEX ***vector, int *size);
int remove_record_descending_sort(int ticket, char *file_bin, INDEX ***vector, int *size);

int insert_worstFit(char *, INDEX ***, int *, REG *);
int insert_reg_first_fit(char *filename, REG *reg, INDEX ***index, int* nIndex);

void show_list(char *filename);

void insert_best_fit(REG *r, INDEX ***indices, int *indSize, char *filename);

#endif
